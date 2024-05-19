#ifndef TRADING_PLATFORM_HPP
#define TRADING_PLATFORM_HPP

#include "currency.hpp"
#include "segment.hpp"
#include "order.hpp"
#include "deal.hpp"
#include <utility>
#include "storage.hpp"

namespace trading {

/*------------------------------------------------TradingPlatformScopeFWD--------------------------------------------------------------*/
class TradingPlatformScope;

/*--------------------------------------------------TradingPlatformImpl----------------------------------------------------------------*/
namespace trading_platform_details {

class TradingPlatformImpl {
  friend class trading::TradingPlatformScope;

 public:
  using OrderID = order_details::OrderBase::ID;
  using DealID = deal_details::DealBase::ID;

 public:
  template <typename TargetCurrency, typename PaymentCurrency>
  OrderID PublishOrder(
      Order<TargetCurrency, PaymentCurrency>::DetailsType order_details) {
    Order<TargetCurrency, PaymentCurrency> order(++next_order_id_,
                                                 std::move(order_details));
    std::get<typename Order<TargetCurrency, PaymentCurrency>::SegmentType>(
        segments_)
        .AddOrder(std::move(order));
    return next_order_id_;
  }

  template <typename TargetCurrency, typename PaymentCurrency>
  void CloseDeal(Order<TargetCurrency, PaymentCurrency>::MatchingType::Result
                     matching_res) {
    Deal<TargetCurrency, PaymentCurrency> deal(++next_deal_id_,
                                               std::move(matching_res));
    deal.BuyerPart().MarketMemberPtr()->CloseDealPart(deal.BuyerPart());
    deal.SellerPart().MarketMemberPtr()->CloseDealPart(deal.SellerPart());

    std::get<typename Deal<TargetCurrency, PaymentCurrency>::SegmentType>(
        segments_)
        .AddDeal(std::move(deal));
  }

 private:
  template <typename TargetCurrency, typename PaymentCurrency>
  using SegmentTemplate = Segment<TargetCurrency, PaymentCurrency>;
  using TupleSegmentsType =
      currencies::TupleObj2ArgsFromCurrencyTypesT<SegmentTemplate>;

  template <template <typename, typename> typename Storage>
  explicit TradingPlatformImpl(const SegmentConfig<Storage>& config)
      : segments_(util::custom_type_traits::construct_tuple_from_same_args<
                  TupleSegmentsType>(config)) {
  }

  ~TradingPlatformImpl() = default;

 private:
  TupleSegmentsType segments_;
  OrderID next_order_id_{0};
  DealID next_deal_id_{0};
};

}  // namespace trading_platform_details

/*-------------------------------------------------TradingPlatformScope----------------------------------------------------------------*/
class TradingPlatformScope {
 public:
  template <template <typename, typename> typename Storage>
  static void Run(const SegmentConfig<Storage>& config) {
    static trading_platform_details::TradingPlatformImpl platform(config);
    trading_platform_ptr_ = &platform;
  }

  static trading_platform_details::TradingPlatformImpl* Get();

  static void Stop();

 private:
  static trading_platform_details::TradingPlatformImpl* trading_platform_ptr_;
};

}  // namespace trading

#endif