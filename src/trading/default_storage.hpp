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
  using OrderMatchingType = OrderType::MatchingType;

 public:
  OrderStorage() = default;

  void Add(OrderType order) {
    auto key = GenerateKey(order);
    auto order_id = order.Id();
    auto order_side = order.Details().Side();

    auto [it, is_inserted] =
        MapOrders(order_side).emplace(std::move(key), std::move(order));
    if (is_inserted) {
      order_id_to_location_.emplace(std::move(order_id),
                                    OrderLocation(order_side, it));
    }
  }

  std::optional<OrderType> Get(OrderType::ID id) const {
    if (auto it = order_id_to_location_.find(id);
        it != order_id_to_location_.end()) {
      return it->second.Order();
    }
    return std::nullopt;
  }

  void Delete(OrderType::ID id) {
    if (auto it = order_id_to_location_.find(id);
        it != order_id_to_location_.end()) {
      order_side_to_map_orders_.erase(it->second.Key());
      order_id_to_location_.erase(id);
    }
  }

  void FindMatches() {
    for (auto sale_order_it = MapOrders(OrderType::Side::kSale).begin();
         sale_order_it != MapOrders(OrderType::Side::kSale).end();
         MoveMapOrderIter(sale_order_it)) {
      for (auto buy_order_it = MapOrders(OrderType::Side::kBuy).rbegin();
           buy_order_it != MapOrders(OrderType::Side::kBuy).rend();
           MoveMapOrderReverseIter(buy_order_it)) {
        if (OrderMatchingType::DoMatch(buy_order_it->second,
                                       sale_order_it->second)) {
          auto matching_res = OrderMatchingType::Match(buy_order_it->second,
                                                       sale_order_it->second);

          // TODO: after implementation TradingPlatform
          // TradingPlatform::Get()->CloseDeal(std::move(matching_res));
        }

        if (sale_order_it->second.IsCompleted()) {
          MoveMapOrderReverseIter(buy_order_it);
          break;
        }
      }
    }
  }

 private:
  class StorageKey;
  class OrderLocation;
  struct StorageKeyComp;

  using MapOrdersType = std::map<StorageKey, OrderType, StorageKeyComp>;
  using MapOrdersIterType = typename MapOrdersType::iterator;
  using MapOrdersReverseIterType = typename MapOrdersType::reverse_iterator;

  using OrderSideToMapOrders =
      std::unordered_map<typename OrderType::Side, MapOrdersType>;
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
    PaymentCurrency unit_price_;
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
    explicit OrderLocation(OrderType::Side side, MapOrdersIterType it)
        : side_(side),
          it_(it) {
    }

    OrderType::Side Side() const noexcept {
      return side_;
    }

    StorageKey Key() const noexcept {
      return it_->first;
    }

    OrderType Order() const noexcept {
      return it_->second;
    }

   private:
    OrderType::Side side_;
    MapOrdersIterType it_;
  };

  MapOrdersType& MapOrders(OrderType::Side side) {
    return order_side_to_map_orders_[side];
  }

  void MoveMapOrderIter(MapOrdersIterType& iter) {
    iter = (iter->second.IsCompleted()) ? Delete(iter) : std::next(iter);
  }

  void MoveMapOrderReverseIter(MapOrdersReverseIterType& r_iter) {
    r_iter = (r_iter->second.IsCompleted())
                 ? std::make_reverse_iterator(Delete(std::prev(r_iter.base())))
                 : std::next(r_iter);
  }

  StorageKey GenerateKey(const OrderType& order) const {
    return StorageKey(order);
  }

  MapOrdersIterType Delete(MapOrdersIterType iter) {
    order_id_to_location_.erase(iter->second.Id());
    return MapOrders(iter->second.Details().Side()).erase(iter);
  }

 private:
  OrderIDToLocation order_id_to_location_;
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
    deal_id_to_deal_.emplace(deal.Id(), std::move(deal));
  }

  std::optional<DealType> Get(DealType::ID id) const {
    if (auto it = deal_id_to_deal_.find(id); it != deal_id_to_deal_.end()) {
      return it->second;
    }
    return std::nullopt;
  }

 private:
  using DealIDToDeal = std::unordered_map<typename DealType::ID, DealType>;

 private:
  DealIDToDeal deal_id_to_deal_;
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

  std::optional<OrderType> GetOrder(OrderType::ID id) const override {
    return order_storage_.Get(id);
  }

  void AddDeal(DealType deal) override {
    deal_storage_.Add(std::move(deal));
  };

  std::optional<DealType> GetDeal(DealType::ID id) const override {
    return deal_storage_.Get(id);
  }

  void FindMatchingOrders() override {
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