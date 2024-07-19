#ifndef TRADING_PLATFORM_IMPL
#define TRADING_PLATFORM_IMPL

#include <trading/models/market_member.hpp>

TRADING_NAMESPACE_BEGIN

namespace core {

/*----------------------------------------TradingPlatformImpl--------------------------------------------------*/
namespace trading_platform_details {

template <typename TradingPlatformConfig>
TradingPlatformImpl<TradingPlatformConfig>::TradingPlatformImpl(
    TradingPlatformConfig config)
    : config_(std::move(config)),
      segments_(util::custom_type_traits::construct_tuple_from_same_args<
                TupleSegmentsType>(SegmentConfig(config_))) {
  // config_.Setup();
}

template <typename TradingPlatformConfig>
template <typename Order>
Order::ID TradingPlatformImpl<TradingPlatformConfig>::PublishOrder(
    Order::DetailsType order_details) {
  return std::get<typename Order::SegmentType>(segments_).AddOrder(
      std::move(order_details));
}

template <typename TradingPlatformConfig>
template <typename Deal>
void TradingPlatformImpl<TradingPlatformConfig>::CloseDeal(
    Deal::OrderType::MatchingType::Result matching_result) {
  Deal deal(0, matching_result);
  mm_storage_.CompleteDeal(deal);
}

}  // namespace trading_platform_details

/*---------------------------------------TradingPlatformScope--------------------------------------------------*/
template <typename TradingPlatformConfig>
void TradingPlatformScope<TradingPlatformConfig>::Run(
    TradingPlatformConfig config) {
  static TradingPlatformImpl platform(std::move(config));
  trading_platform_ = &platform;
}

template <typename TradingPlatformConfig>
TradingPlatformScope<TradingPlatformConfig>::TradingPlatformImpl*
    TradingPlatformScope<TradingPlatformConfig>::trading_platform_ = nullptr;

template <typename TradingPlatformConfig>
TradingPlatformScope<TradingPlatformConfig>::TradingPlatformImpl*
TradingPlatformScope<TradingPlatformConfig>::Get() {
  return trading_platform_;
}

template <typename TradingPlatformConfig>
void TradingPlatformScope<TradingPlatformConfig>::Stop() {
  trading_platform_->~TradingPlatformImpl();
}

}  // namespace core

TRADING_NAMESPACE_END

#endif