#ifndef TRADING_PLATFORM_IMPL
#define TRADING_PLATFORM_IMPL

#include <trading/models/market_member.hpp>

TRADING_NAMESPACE_BEGIN

namespace core {

/*----------------------------------------TradingPlatformImpl--------------------------------------------------*/
namespace trading_platform_details {

template <template <typename, typename> typename Storage>
TradingPlatformImpl::TradingPlatformImpl(
    const models::SegmentConfig<Storage>& config)
    : segments_(util::custom_type_traits::construct_tuple_from_same_args<
                TupleSegmentsType>(config)) {
}

template <typename OrderType>
TradingPlatformImpl::OrderID TradingPlatformImpl::PublishOrder(
    OrderType::DetailsType order_details) {
  OrderType order(++next_order_id_, std::move(order_details));
  std::get<typename OrderType::SegmentType>(segments_).AddOrder(
      std::move(order));
  return next_order_id_;
}

template <typename Deal>
void TradingPlatformImpl::CloseDeal(
    Deal::OrderType::MatchingType::Result matching_res) {
  Deal deal(++next_deal_id_, std::move(matching_res));
  deal.BuyerPart().MarketMemberPtr()->template CloseDealPart<Deal>(
      deal.BuyerPart());
  deal.SellerPart().MarketMemberPtr()->template CloseDealPart<Deal>(
      deal.SellerPart());

  std::get<typename Deal::SegmentType>(segments_).AddDeal(std::move(deal));
}
}  // namespace trading_platform_details

/*---------------------------------------TradingPlatformScope--------------------------------------------------*/
template <template <typename, typename> typename Storage>
void TradingPlatformScope::Run(const models::SegmentConfig<Storage>& config) {
  static TradingPlatformImpl platform(config);
  trading_platform_ptr_ = &platform;
}

}  // namespace core

TRADING_NAMESPACE_END

#endif