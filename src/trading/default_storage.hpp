#ifndef DEFAULT_STORAGE_HPP
#define DEFAULT_STORAGE_HPP

#include <map>
#include <optional>
#include <unordered_map>

#include "storage.hpp"
#include "trading_platform.hpp"

namespace trading {

namespace storages {

namespace details {

/*------------------------------------------OrderStorage-------------------------------------------------------*/
template <typename Order>
class OrderStorage {
 public:
  using OrderType = Order;

 public:
  OrderStorage() = default;

  void Add(OrderType order);

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

  using TradingSideToMapOrders = std::unordered_map<TradingSide, MapOrdersType>;
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
    explicit OrderLocation(TradingSide side, MapOrdersIterType it)
        : side_(side),
          it_(it) {
    }

    TradingSide Side() const noexcept {
      return side_;
    }

    StorageKey Key() const noexcept {
      return it_->first;
    }

    OrderType Value() const noexcept {
      return it_->second;
    }

   private:
    TradingSide side_;
    MapOrdersIterType it_;
  };

  MapOrdersType& MapOrders(TradingSide side);

  void MoveMapOrderIter(MapOrdersIterType& iter);

  void MoveMapOrderReverseIter(MapOrdersReverseIterType& r_iter);

  StorageKey GenerateKey(const OrderType& order) const;

  MapOrdersIterType Delete(MapOrdersIterType iter);

 private:
  OrderIDToLocation order_id_to_location_;
  TradingSideToMapOrders order_side_to_map_orders_;
};

/*------------------------------------------DealStorage--------------------------------------------------------*/
template <typename Deal>
class DealStorage {
 public:
  using DealType = Deal;

 public:
  DealStorage() = default;

  void Add(DealType deal);

  std::optional<DealType> Get(DealType::ID id) const;

 private:
  using DealIDToDeal = std::unordered_map<typename DealType::ID, DealType>;

 private:
  DealIDToDeal deal_id_to_deal_;
};

}  // namespace details

/*-----------------------------------------DefaultStorage------------------------------------------------------*/
template <typename Order, typename Deal>
class DefaultStorage : public IStorage<Order, Deal> {
 public:
  using BaseType = IStorage<Order, Deal>;
  using OrderType = BaseType::OrderType;
  using DealType = BaseType::DealType;

 public:
  DefaultStorage() = default;

  void AddOrder(OrderType order);

  std::optional<OrderType> GetOrder(OrderType::ID id) const;

  void AddDeal(DealType deal);

  std::optional<DealType> GetDeal(DealType::ID id) const;

  void FindMatchingOrders() override;

 private:
  using OrderStorageType = details::OrderStorage<OrderType>;
  using DealStorageType = details::DealStorage<DealType>;

 private:
  OrderStorageType order_storage_;
  DealStorageType deal_storage_;
};

}  // namespace storages

}  // namespace trading

#include "impl/default_storage_impl.hpp"

#endif