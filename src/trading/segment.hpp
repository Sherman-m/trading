#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include <optional>
#include "deal_fwd.hpp"
#include "order_fwd.hpp"
#include "segment_analyzer.hpp"

namespace trading {

struct SegmentConfig {
  using Milliseconds = std::chrono::milliseconds;

  explicit SegmentConfig(boost::asio::io_context& context,
                         Milliseconds analyze_period)
      : context(context),
        analyze_period(analyze_period) {
  }

  boost::asio::io_context& context;
  Milliseconds analyze_period;
};

template <typename TargetCurrency, typename PaymentCurrency,
          template <typename, typename> typename Storage>
class Segment {
 public:
  using SegmentType = Segment<TargetCurrency, PaymentCurrency, Storage>;
  using OrderType = Order<TargetCurrency, PaymentCurrency>;
  using DealType = Deal<TargetCurrency, PaymentCurrency>;

 public:
  Segment(const SegmentConfig& config)
      : segment_analyzer_ptr_(std::make_shared<SegmentAnalyzerType>(
            config.context, config.analyze_period, [this]() {
              this->Analyze();
            })) {
    segment_analyzer_ptr_->Start();
  }

  void AddOrder(OrderType order) {
    storage_.AddOrder(std::move(order));
  }

  std::optional<OrderType> GetOrder(OrderType::ID id) {
    return storage_.GetOrder(std::move(id));
  }

  void AddDeal(DealType deal) {
    storage_.AddDeal(deal);
  }

  std::optional<DealType> GetDeal(DealType::ID id) {
    return storage_.GetDeal(std::move(id));
  }

  void Analyze() {
    storage_.FindMatchingOrders();
  }

 private:
  using StorageType = Storage<TargetCurrency, PaymentCurrency>;
  using SegmentAnalyzerType = segment_details::SegmentAnalyzer<SegmentType>;

 private:
  StorageType storage_;
  std::shared_ptr<SegmentAnalyzerType> segment_analyzer_ptr_;
};
}  // namespace trading

#endif