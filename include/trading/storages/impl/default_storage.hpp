#ifndef DEFAULT_STORAGE_IMPL_HPP
#define DEFAULT_STORAGE_IMPL_HPP

#include <trading/models/deal_fwd.hpp>

TRADING_NAMESPACE_BEGIN

namespace storages {

namespace details {

/*------------------------------------------OrderStorage-------------------------------------------------------*/
template <typename Order>
void OrderStorage<Order>::Add(OrderType order) {
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

template <typename Order>
std::optional<typename OrderStorage<Order>::OrderType> OrderStorage<Order>::Get(
    OrderType::ID id) const {
  if (auto it = order_id_to_location_.find(id);
      it != order_id_to_location_.end()) {
    return it->second.Value();
  }
  return std::nullopt;
}

template <typename Order>
void OrderStorage<Order>::Delete(OrderType::ID id) {
  if (auto it = order_id_to_location_.find(id);
      it != order_id_to_location_.end()) {
    order_side_to_map_orders_.erase(it->second.Key());
    order_id_to_location_.erase(id);
  }
}

template <typename Order>
void OrderStorage<Order>::FindMatches() {
  for (auto sale_order_it = MapOrders(models::TradingSide::kSale).begin();
       sale_order_it != MapOrders(models::TradingSide::kSale).end();
       MoveMapOrderIter(sale_order_it)) {
    for (auto buy_order_it = MapOrders(models::TradingSide::kBuy).rbegin();
         buy_order_it != MapOrders(models::TradingSide::kBuy).rend();
         MoveMapOrderReverseIter(buy_order_it)) {
      if (OrderType::MatchingType::DoMatch(buy_order_it->second,
                                           sale_order_it->second)) {
        auto matching_res = OrderType::MatchingType::Match(
            buy_order_it->second, sale_order_it->second);

        core::TradingPlatformScope::Get()->CloseDeal<models::Deal<OrderType>>(
            std::move(matching_res));
      }

      if (sale_order_it->second.IsCompleted()) {
        MoveMapOrderReverseIter(buy_order_it);
        break;
      }
    }
  }
}

template <typename Order>
OrderStorage<Order>::MapOrdersType& OrderStorage<Order>::MapOrders(
    models::TradingSide side) {
  return order_side_to_map_orders_[side];
}

template <typename Order>
void OrderStorage<Order>::MoveMapOrderIter(MapOrdersIterType& iter) {
  iter = (iter->second.IsCompleted()) ? Delete(iter) : std::next(iter);
}

template <typename Order>
void OrderStorage<Order>::MoveMapOrderReverseIter(
    MapOrdersReverseIterType& r_iter) {
  r_iter = (r_iter->second.IsCompleted())
               ? std::make_reverse_iterator(Delete(std::prev(r_iter.base())))
               : std::next(r_iter);
}

template <typename Order>
OrderStorage<Order>::StorageKey OrderStorage<Order>::GenerateKey(
    const OrderType& order) const {
  return StorageKey(order);
}

template <typename Order>
OrderStorage<Order>::MapOrdersIterType OrderStorage<Order>::Delete(
    MapOrdersIterType iter) {
  order_id_to_location_.erase(iter->second.Id());
  return MapOrders(iter->second.Details().Side()).erase(iter);
}

/*------------------------------------------DealStorage--------------------------------------------------------*/
template <typename Deal>
void DealStorage<Deal>::Add(DealType deal) {
  deal_id_to_deal_.emplace(deal.Id(), std::move(deal));
}

template <typename Deal>
std::optional<typename DealStorage<Deal>::DealType> DealStorage<Deal>::Get(
    DealType::ID id) const {
  if (auto it = deal_id_to_deal_.find(id); it != deal_id_to_deal_.end()) {
    return it->second;
  }
  return std::nullopt;
}

}  // namespace details

/*-----------------------------------------DefaultStorage------------------------------------------------------*/
template <typename Order, typename Deal>
void DefaultStorage<Order, Deal>::AddOrder(OrderType order) {
  order_storage_.Add(std::move(order));
}

template <typename Order, typename Deal>
std::optional<typename DefaultStorage<Order, Deal>::OrderType>
DefaultStorage<Order, Deal>::GetOrder(OrderType::ID id) const {
  return order_storage_.Get(id);
}

template <typename Order, typename Deal>
void DefaultStorage<Order, Deal>::AddDeal(DealType deal) {
  deal_storage_.Add(std::move(deal));
};

template <typename Order, typename Deal>
std::optional<typename DefaultStorage<Order, Deal>::DealType>
DefaultStorage<Order, Deal>::GetDeal(DealType::ID id) const {
  return deal_storage_.Get(id);
}

template <typename Order, typename Deal>
void DefaultStorage<Order, Deal>::FindMatchingOrders() {
  order_storage_.FindMatches();
}

}  // namespace storages

TRADING_NAMESPACE_END

#endif