#ifndef SEGMENT_FWD_HPP
#define SEGMENT_FWD_HPP

TRADING_NAMESPACE_BEGIN

namespace models {

/*--------------------------------------------SegmentConfig----------------------------------------------------*/
template <template <typename, typename> typename Storage>
struct SegmentConfig;

/*----------------------------------------------Segment--------------------------------------------------------*/
template <typename TargetCurrency, typename PaymentCurrency>
class Segment;

}  // namespace models

TRADING_NAMESPACE_END

#endif