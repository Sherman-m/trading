#ifndef DEFAULT_STORAGE_HPP
#define DEFAULT_STORAGE_HPP

#include <map>
#include <optional>
#include <trading/core/trading_platform.hpp>
#include <trading/storages/storage.hpp>
#include <unordered_map>

TRADING_NAMESPACE_BEGIN

namespace storages {

namespace details {

using CommonID = size_t;

/*------------------------------------------OrderStorage-------------------------------------------------------*/
template <typename TradingPlatformScope, typename Order>
class OrderStorage {
 public:
  using OrderType = Order;

  OrderStorage() = default;

  OrderType::ID Add(typename OrderType::DetailsType order_details);

  std::optional<OrderType> Get(OrderType::ID id) const;

  void Delete(OrderType::ID id);

  void FindMatches();

 private:
  class StorageKey;
  class OrderLocation;
  struct StorageKeyComp;

  using MapOrdersType = std::map<StorageKey, OrderType, StorageKeyComp>;
  using MapOrdersIterType = typename MapOrdersType::iterator;
  using MapOrdersReverseIterType = typename MapOrdersType::reverse_iterator;

  using TradingSideToMapOrders =
      std::unordered_map<models::TradingSide, MapOrdersType>;
  using OrderIDToLocation =
      std::unordered_map<typename OrderType::ID, OrderLocation>;

  class StorageKey {
    friend struct StorageKeyComp;

   public:
    explicit StorageKey(const OrderType& order)
        : order_id_(order.Id()),
          unit_price_(order.Details().UnitPrice()) {
    }

   private:
    OrderType::ID order_id_;
    OrderType::PaymentCurrencyType unit_price_;
  };

  struct StorageKeyComp {
    bool operator()(const StorageKey& first,
                    const StorageKey& second) const noexcept {
      if (first.unit_price_ == second.unit_price_) {
        return first.order_id_ < second.order_id_;
      }
      return first.unit_price_ < second.unit_price_;
    }
  };

  class OrderLocation {
   public:
    explicit OrderLocation(models::TradingSide side, MapOrdersIterType it)
        : side_(side),
          it_(it) {
    }

    models::TradingSide Side() const noexcept {
      return side_;
    }

    StorageKey Key() const noexcept {
      return it_->first;
    }

    OrderType Value() const noexcept {
      return it_->second;
    }

   private:
    models::TradingSide side_;
    MapOrdersIterType it_;
  };

  MapOrdersType& MapOrders(models::TradingSide side);

  void MoveMapOrderIter(MapOrdersIterType& iter);

  void MoveMapOrderReverseIter(MapOrdersReverseIterType& r_iter);

  StorageKey GenerateKey(const OrderType& order) const;

  MapOrdersIterType Delete(MapOrdersIterType iter);

  OrderIDToLocation order_id_to_location_;
  TradingSideToMapOrders order_side_to_map_orders_;
  OrderType::ID next_order_id_;
};

/*------------------------------------------DealStorage--------------------------------------------------------*/
template <typename Deal>
class DealStorage {
 public:
  using DealType = Deal;

  DealStorage() = default;

  Deal::ID Add(DealType::OrderType::MatchingType::Result matching_result);

  std::optional<DealType> Get(DealType::ID id) const;

 private:
  using DealIDToDeal = std::unordered_map<typename DealType::ID, DealType>;

  DealIDToDeal deal_id_to_deal_;
  DealType::ID next_deal_id_;
};

}  // namespace details

/*-----------------------------------------DefaultStorage------------------------------------------------------*/
template <typename Config>
class DefaultStorage : public IStorage<Config, details::CommonID> {
 public:
  using BaseType = IStorage<Config, details::CommonID>;
  using OrderType = BaseType::OrderType;
  using DealType = BaseType::DealType;

  DefaultStorage() = default;

  OrderType::ID AddOrder(OrderType::DetailsType order_details) override;

  std::optional<OrderType> GetOrder(OrderType::ID id) const override;

  DealType::ID AddDeal(typename DealType::OrderType::MatchingType::Result
                           matching_result) override;

  std::optional<DealType> GetDeal(DealType::ID id) const override;

  void FindMatchingOrders() override;

 private:
  using OrderStorageType =
      details::OrderStorage<typename Config::TradingPlatformScopeType,
                            OrderType>;
  using DealStorageType = details::DealStorage<DealType>;

  OrderStorageType order_storage_;
  DealStorageType deal_storage_;
};

}  // namespace storages

TRADING_NAMESPACE_END

#include <trading/storages/impl/default_storage.hpp>

#endif