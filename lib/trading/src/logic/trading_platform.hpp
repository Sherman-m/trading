#ifndef TRADING_PLATFORM_HPP
#define TRADING_PLATFORM_HPP

#include <utility>

#include "currency.hpp"
#include "deal.hpp"
#include "market_member_fwd.hpp"
#include "order.hpp"
#include "segment.hpp"
#include "trading_side.hpp"

namespace trading {

/*-------------------------------------TradingPlatformScopeFWD-------------------------------------------------*/
class TradingPlatformScope;

/*---------------------------------------TradingPlatformImpl---------------------------------------------------*/
namespace trading_platform_details {

class TradingPlatformImpl {
  friend class trading::TradingPlatformScope;

 public:
  using OrderID = order_details::OrderBase::ID;
  using DealID = deal_details::DealBase::ID;

  template <typename Order>
  OrderID PublishOrder(Order::DetailsType order_details);

  template <typename Deal>
  void CloseDeal(Deal::OrderType::MatchingType::Result matching_res);

 private:
  template <typename TargetCurrency, typename PaymentCurrency>
  using SegmentTemplate = Segment<TargetCurrency, PaymentCurrency>;
  using TupleSegmentsType =
      currencies::TupleObj2ArgsFromCurrencyTypesT<SegmentTemplate>;

  template <template <typename, typename> typename Storage>
  explicit TradingPlatformImpl(const SegmentConfig<Storage>& config);

  ~TradingPlatformImpl() = default;

  TupleSegmentsType segments_;
  OrderID next_order_id_{0};
  DealID next_deal_id_{0};
};

}  // namespace trading_platform_details

/*---------------------------------------TradingPlatformScope--------------------------------------------------*/
class TradingPlatformScope {
 public:
  using TradingPlatformImpl = trading_platform_details::TradingPlatformImpl;

  TradingPlatformScope() = delete;

  TradingPlatformScope(const TradingPlatformScope&) = delete;
  TradingPlatformScope& operator=(const TradingPlatformScope&) = delete;

  TradingPlatformScope(TradingPlatformScope&&) = delete;
  TradingPlatformScope& operator=(TradingPlatformScope&&) = delete;

  ~TradingPlatformScope() = default;

  template <template <typename, typename> typename Storage>
  static void Run(const SegmentConfig<Storage>& config);

  static TradingPlatformImpl* Get();

  static void Stop();

 private:
  static TradingPlatformImpl* trading_platform_ptr_;
};

}  // namespace trading

#include "impl/trading_platform_impl.hpp"

#endif