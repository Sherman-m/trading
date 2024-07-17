#ifndef SEGMENT_FWD_HPP
#define SEGMENT_FWD_HPP

namespace trading {

/*--------------------------------------------SegmentConfig----------------------------------------------------*/
template <template <typename, typename> typename Storage>
struct SegmentConfig;

/*----------------------------------------------Segment--------------------------------------------------------*/
template <typename TargetCurrency, typename PaymentCurrency>
class Segment;

}  // namespace trading

#endif