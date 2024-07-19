#ifndef DEFAULT_STORAGE_IMPL_HPP
#define DEFAULT_STORAGE_IMPL_HPP

#include <trading/models/deal_fwd.hpp>

TRADING_NAMESPACE_BEGIN

namespace storages {

namespace details {

/*------------------------------------------OrderStorage-------------------------------------------------------*/
template <typename TradingPlatformScope, typename Order>
OrderStorage<TradingPlatformScope, Order>::OrderType::ID
OrderStorage<TradingPlatformScope, Order>::Add(
    typename OrderType::DetailsType order_details) {
  auto order_id = next_order_id_++;

  Order order(order_id, std::move(order_details));

  auto key = GenerateKey(order);
  auto order_side = order.Details().Side();

  auto [it, is_inserted] =
      MapOrders(order_side).emplace(std::move(key), std::move(order));
  if (is_inserted) {
    order_id_to_location_.emplace(std::move(order_id),
                                  OrderLocation(order_side, it));
  }
  return order_id;
}

template <typename TradingPlatformScope, typename Order>
std::optional<typename OrderStorage<TradingPlatformScope, Order>::OrderType>
OrderStorage<TradingPlatformScope, Order>::Get(OrderType::ID id) const {
  if (auto it = order_id_to_location_.find(id);
      it != order_id_to_location_.end()) {
    return it->second.Value();
  }
  return std::nullopt;
}

template <typename TradingPlatformScope, typename Order>
void OrderStorage<TradingPlatformScope, Order>::Delete(OrderType::ID id) {
  if (auto it = order_id_to_location_.find(id);
      it != order_id_to_location_.end()) {
    order_side_to_map_orders_.erase(it->second.Key());
    order_id_to_location_.erase(id);
  }
}

template <typename TradingPlatformScope, typename Order>
void OrderStorage<TradingPlatformScope, Order>::FindMatches() {
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
        TradingPlatformScope::Get()
            ->template CloseDeal<models::Deal<OrderType>>(
                std::move(matching_res));
      }

      if (sale_order_it->second.IsCompleted()) {
        MoveMapOrderReverseIter(buy_order_it);
        break;
      }
    }
  }
}

template <typename TradingPlatformScope, typename Order>
OrderStorage<TradingPlatformScope, Order>::MapOrdersType&
OrderStorage<TradingPlatformScope, Order>::MapOrders(models::TradingSide side) {
  return order_side_to_map_orders_[side];
}

template <typename TradingPlatformScope, typename Order>
void OrderStorage<TradingPlatformScope, Order>::MoveMapOrderIter(
    MapOrdersIterType& iter) {
  iter = (iter->second.IsCompleted()) ? Delete(iter) : std::next(iter);
}

template <typename TradingPlatformScope, typename Order>
void OrderStorage<TradingPlatformScope, Order>::MoveMapOrderReverseIter(
    MapOrdersReverseIterType& r_iter) {
  r_iter = (r_iter->second.IsCompleted())
               ? std::make_reverse_iterator(Delete(std::prev(r_iter.base())))
               : std::next(r_iter);
}

template <typename TradingPlatformScope, typename Order>
OrderStorage<TradingPlatformScope, Order>::StorageKey
OrderStorage<TradingPlatformScope, Order>::GenerateKey(
    const OrderType& order) const {
  return StorageKey(order);
}

template <typename TradingPlatformScope, typename Order>
OrderStorage<TradingPlatformScope, Order>::MapOrdersIterType
OrderStorage<TradingPlatformScope, Order>::Delete(MapOrdersIterType iter) {
  order_id_to_location_.erase(iter->second.Id());
  return MapOrders(iter->second.Details().Side()).erase(iter);
}

/*------------------------------------------DealStorage--------------------------------------------------------*/
template <typename Deal>
DealStorage<Deal>::DealType::ID DealStorage<Deal>::Add(
    DealType::OrderType::MatchingType::Result matching_result) {
  auto deal = DealType(next_deal_id_++, std::move(matching_result));
  auto deal_id = deal.Id();
  deal_id_to_deal_.emplace(deal.Id(), std::move(deal));
  return deal_id;
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
template <typename Config>
typename DefaultStorage<Config>::OrderType::ID DefaultStorage<Config>::AddOrder(
    OrderType::DetailsType order_details) {
  return order_storage_.Add(std::move(order_details));
}

template <typename Config>
std::optional<typename DefaultStorage<Config>::OrderType>
DefaultStorage<Config>::GetOrder(OrderType::ID id) const {
  return order_storage_.Get(id);
}

template <typename Config>
DefaultStorage<Config>::DealType::ID DefaultStorage<Config>::AddDeal(
    typename DealType::OrderType::MatchingType::Result matching_result) {
  return deal_storage_.Add(std::move(matching_result));
};

template <typename Config>
std::optional<typename DefaultStorage<Config>::DealType>
DefaultStorage<Config>::GetDeal(DealType::ID id) const {
  return deal_storage_.Get(id);
}

template <typename Config>
void DefaultStorage<Config>::FindMatchingOrders() {
  order_storage_.FindMatches();
}

}  // namespace storages

TRADING_NAMESPACE_END

#endif