#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include <boost/asio/io_context.hpp>
#include <chrono>
#include <memory>
#include <optional>

#include "deal_fwd.hpp"
#include "order_fwd.hpp"
#include "segment_analyzer.hpp"
#include "storage.hpp"

namespace trading {

/*--------------------------------------------SegmentConfig----------------------------------------------------*/
template <template <typename, typename> typename Storage>
struct SegmentConfig {
  using Milliseconds = std::chrono::milliseconds;

  explicit SegmentConfig(boost::asio::io_context& context,
                         Milliseconds analyze_period);

  boost::asio::io_context& context;
  Milliseconds analyze_period;
};

/*----------------------------------------------Segment--------------------------------------------------------*/
template <typename TargetCurrency, typename PaymentCurrency>
class Segment {
 public:
  using OrderType = Order<TargetCurrency, PaymentCurrency>;
  using DealType = Deal<OrderType>;

  template <template <typename, typename> typename Storage>
  Segment(const SegmentConfig<Storage>& config);

  Segment(const Segment&) = delete;
  Segment& operator=(const Segment&) = delete;

  Segment(Segment&& other);
  Segment& operator=(Segment&& other);

  ~Segment();

  void AddOrder(OrderType order);

  std::optional<OrderType> GetOrder(OrderType::ID id) const;

  void AddDeal(DealType deal);

  std::optional<DealType> GetDeal(DealType::ID id) const;

  void Analyze();

 private:
  using SegmentAnalyzerType = segment_details::SegmentAnalyzer<
      Segment<TargetCurrency, PaymentCurrency>>;
  using IStorageType = storages::IStorage<OrderType, DealType>;

  std::unique_ptr<IStorageType> storage_ptr_;
  std::shared_ptr<SegmentAnalyzerType> segment_analyzer_ptr_;
};

}  // namespace trading

#include "impl/segment_impl.hpp"

#endif