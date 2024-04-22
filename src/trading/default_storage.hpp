#ifndef DEFAULT_STORAGE_HPP
#define DEFAULT_STORAGE_HPP

#include "storage.hpp"
#include "deal.hpp"
#include "order.hpp"
#include <map>
#include <unordered_map>

namespace trading {

namespace storages {

namespace details {

/*-------------------------------------------------------OrderStorage-------------------------------------------------------------*/
template <typename TargetCurrency, typename PaymentCurrency>
class OrderStorage {
 public:
  using OrderType = Order<TargetCurrency, PaymentCurrency>;
  using OrderMatchingType = OrderType::OrdersMatchingType;

 public:
  OrderStorage() = default;

  void Add(OrderType order) {
    auto key = GenerateKey(order);
    auto order_id = order.GetId();
    auto order_side = order.GetDetails().side;

    auto [it, is_inserted] =
        GetMapOrders(order_side).emplace(std::move(key), std::move(order));
    if (is_inserted) {
      order_id_to_location_.emplace(std::move(order_id),
                                    OrderLocation(order_side, it));
    }
  }

  std::optional<OrderType> Get(OrderType::Id id) const {
    if (auto it = order_id_to_location_.find(id);
        it != order_id_to_location_.end()) {
      return it->second.GetOrder();
    }
    return std::nullopt;
  }

  void Delete(OrderType::Id id) {
    if (auto it = order_id_to_location_.find(id);
        it != order_id_to_location_.end()) {
      order_side_to_map_orders_.erase(it->second.GetKey());
      order_id_to_location_.erase(id);
    }
  }

  void FindMatches() {
    for (auto buy_order_it = GetMapOrders(OrderType::Side::kBuy).begin();
         buy_order_it != GetMapOrders(OrderType::Side::kBuy).end();) {
      for (auto sale_order_it = GetMapOrders(OrderType::Side::kSale).begin();
           sale_order_it != GetMapOrders(OrderType::Side::kSale).end();) {
        if (OrderMatchingType::DoOrdersMatch(buy_order_it->second,
                                             sale_order_it->second)) {
          auto matching_res = OrderMatchingType::MatchOrders(
              buy_order_it->second, sale_order_it->second);

          // TODO: after implementation TradingPlatform
          // TradingPlatform::Get()->CloseDeal(std::move(matching_res));
        }
        buy_order_it = (buy_order_it->second.IsCompleted())
                           ? Delete(buy_order_it)
                           : std::next(buy_order_it);
        sale_order_it = (sale_order_it->second.IsCompleted())
                            ? Delete(sale_order_it)
                            : std::next(sale_order_it);
      }
    }
  }

 private:
  // TODO: StorageKey maybe better
  class OrderStorageKey;
  class OrderLocation;
  struct OrderStorageKeyComp;

  using MapOrders = std::map<OrderStorageKey, OrderType, OrderStorageKeyComp>;
  using MapOrdersIter = typename MapOrders::iterator;

  using OrderSideToMapOrders =
      std::unordered_map<typename OrderType::Side, MapOrders>;
  using OrderIdToLocation =
      std::unordered_map<typename OrderType::Id, OrderLocation>;

  class OrderStorageKey {
    friend struct OrderStorageKeyComp;

   public:
    explicit OrderStorageKey(const OrderType& order)
        : order_id_(order.GetId()),
          unit_price_(order.GetDetails().unit_price) {
    }

   private:
    OrderType::Id order_id_;
    PaymentCurrency unit_price_;
  };

  struct OrderStorageKeyComp {
    bool operator()(const OrderStorageKey& first,
                    const OrderStorageKey& second) const noexcept {
      if (first.unit_price_ == second.unit_price_) {
        return first.order_id_ < second.order_id_;
      }
      return first.unit_price_ < second.unit_price_;
    }
  };

  class OrderLocation {
   public:
    explicit OrderLocation(OrderType::Side side, MapOrdersIter it)
        : side_(side),
          it_(it) {
    }
    // TODO: remove prefix Get from names of methods
    OrderType::Side GetSide() const noexcept {
      return side_;
    }
    // TODO: remove prfix Get from names of methods
    OrderStorageKey GetKey() const noexcept {
      return it_->first;
    }
    // TODO: remove prfix Get from names of methods
    OrderType GetOrder() const noexcept {
      return it_->second;
    }

   private:
    OrderType::Side side_;
    MapOrdersIter it_;
  };
  // TODO: remove prefix Get from names of methods
  MapOrders& GetMapOrders(OrderType::Side side) {
    return order_side_to_map_orders_[side];
  }

  OrderStorageKey GenerateKey(const OrderType& order) const {
    return OrderStorageKey(order);
  }

  MapOrdersIter Delete(MapOrdersIter iter) {
    order_id_to_location_.erase(iter->second.GetId());
    return GetMapOrders(iter->second.GetDetails().side).erase(iter);
  }

 private:
  OrderIdToLocation order_id_to_location_;
  OrderSideToMapOrders order_side_to_map_orders_;
};

/*-------------------------------------------------------DealStorage--------------------------------------------------------------*/
template <typename TargetCurrency, typename PaymentCurrency>
class DealStorage {
 public:
  using DealType = Deal<TargetCurrency, PaymentCurrency>;

 public:
  DealStorage() = default;

  void Add(DealType deal) {
    deal_id_to_deal_.emplace(deal.GetId(), std::move(deal));
  }

  std::optional<DealType> Get(DealType::Id id) const {
    if (auto it = deal_id_to_deal_.find(id); it != deal_id_to_deal_.end()) {
      return it->second;
    }
    return std::nullopt;
  }

 private:
  using DealIdToDeal = std::unordered_map<typename DealType::Id, DealType>;

 private:
  DealIdToDeal deal_id_to_deal_;
};

}  // namespace details

/*-------------------------------------------------------DefaultStorage--------------------------------------------------------------*/
template <typename TargetCurrency, typename PaymentCurrency>
class DefaultStorage : public IStorage<TargetCurrency, PaymentCurrency> {
 public:
  using BaseType = IStorage<TargetCurrency, PaymentCurrency>;
  using OrderType = BaseType::OrderType;
  using DealType = BaseType::DealType;

 public:
  DefaultStorage() = default;

  void AddOrder(OrderType order) override {
    order_storage_.Add(std::move(order));
  }

  std::optional<OrderType> GetOrder(OrderType::Id id) const override {
    return order_storage_.Get(id);
  }

  void AddDeal(DealType deal) override {
    deal_storage_.Add(std::move(deal));
  };

  std::optional<DealType> GetDeal(DealType::Id id) const override {
    return deal_storage_.Get(id);
  }

  void FindOrderMatches() override {
    order_storage_.FindMatches();
  }

 private:
  using OrderStorageType =
      details::OrderStorage<TargetCurrency, PaymentCurrency>;
  using DealStorageType = details::DealStorage<TargetCurrency, PaymentCurrency>;

 private:
  OrderStorageType order_storage_;
  DealStorageType deal_storage_;
};

}  // namespace storages

}  // namespace trading

#endif