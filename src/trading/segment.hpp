#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include <optional>
#include "deal_fwd.hpp"
#include "order_fwd.hpp"
#include "segment_analyzer.hpp"
#include "storage.hpp"

#include <memory>

namespace trading {

template <template <typename, typename> typename Storage>
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

template <typename TargetCurrency, typename PaymentCurrency>
class Segment {
 public:
  using SegmentType = Segment<TargetCurrency, PaymentCurrency>;
  using OrderType = Order<TargetCurrency, PaymentCurrency>;
  using DealType = Deal<TargetCurrency, PaymentCurrency>;

 public:
  template <template <typename, typename> typename Storage>
  Segment(const SegmentConfig<Storage>& config)
      : storage_(std::make_unique<Storage<TargetCurrency, PaymentCurrency>>()),
        segment_analyzer_ptr_(std::make_shared<SegmentAnalyzerType>(
            config.context, config.analyze_period)) {
    segment_analyzer_ptr_->Start(std::bind(&Segment::Analyze, this));
  }

  Segment(const Segment&) = delete;
  Segment& operator=(const Segment&) = delete;

  Segment(Segment&& other) {
    other.segment_analyzer_ptr_->Stop();

    storage_ = std::move(other.storage_);
    segment_analyzer_ptr_ = std::move(other.segment_analyzer_ptr_);

    segment_analyzer_ptr_->Start(std::bind(&Segment::Analyze, this));
  }

  Segment& operator=(Segment&& other) {
    if (this != &other) {
      other.segment_analyzer_ptr_->Stop();

      storage_ = std::move(other.storage_);
      segment_analyzer_ptr_ = std::move(other.segment_analyzer_ptr_);

      segment_analyzer_ptr_->Start(std::bind(&Segment::Analyze, this));
    }
    return *this;
  }

  ~Segment() {
    segment_analyzer_ptr_.reset();
  }

  void Start() {
    segment_analyzer_ptr_->Start(std::bind(&Segment::Analyze, this));
  }

  void AddOrder(OrderType order) {
    storage_->AddOrder(std::move(order));
  }

  std::optional<OrderType> GetOrder(OrderType::ID id) {
    return storage_->GetOrder(std::move(id));
  }

  void AddDeal(DealType deal) {
    storage_->AddDeal(deal);
  }

  std::optional<DealType> GetDeal(DealType::ID id) {
    return storage_->GetDeal(std::move(id));
  }

  void Analyze() {
    storage_->FindMatchingOrders();
  }

 private:
  using SegmentAnalyzerType = segment_details::SegmentAnalyzer<SegmentType>;
  using IStorageType = storages::IStorage<TargetCurrency, PaymentCurrency>;

 private:
  std::unique_ptr<IStorageType> storage_;
  std::shared_ptr<SegmentAnalyzerType> segment_analyzer_ptr_;
};
}  // namespace trading

#endif