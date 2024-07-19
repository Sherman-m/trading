#include <chrono>
#include <iostream>
#include <trading/core/config_base.hpp>
#include <trading/core/trading_platform.hpp>
#include <trading/models/market_member_fwd.hpp>
#include <trading/storages/default_storage.hpp>
#include <vector>

template <typename Con>
struct MmStorage {
  struct MConfig {
    using Id = int;
    using TradingPlatformScopeType = Con::TradingPlatformScopeType;

    template <typename TargetCurrency, typename PaymentCurrency>
    using OrderType = typename Con::OrderType<TargetCurrency, PaymentCurrency>;
  };
  using MarketMemberType = trading::models::MarketMember<MConfig>;

  typename MarketMemberType::ID AddMarketMember() {
    storage_.emplace_back(next_id++);
    return storage_.back().Id();
  }

  MarketMemberType* GetMarketMember(MarketMemberType::ID id) {
    return &storage_[id];
  }
  template <typename Deal>
  void CompleteDeal(Deal& deal) {
    storage_[deal.BuyerPart().MarketMemberId()].template WithdrawWallet(
        deal.BuyerPart().Paid());
    storage_[deal.SellerPart().MarketMemberId()].template TopUpWallet(
        deal.SellerPart().Paid());

    storage_[deal.BuyerPart().MarketMemberId()]
        .template TopUpWallet<typename Deal::TargetCurrencyType>(
            deal.BuyerPart().NumUnits());
    storage_[deal.SellerPart().MarketMemberId()]
        .template TopUpWallet<typename Deal::TargetCurrencyType>(
            deal.SellerPart().NumUnits());
  }

  int next_id = 0;
  std::vector<MarketMemberType> storage_;
};

using Config =
    trading::configs::ConfigBase<trading::storages::DefaultStorage, MmStorage>;

using namespace trading;

int main() {
  boost::asio::io_context ioc;
  Config config(ioc, std::chrono::milliseconds(30));

  core::TradingPlatformScope<Config>::Run(config);

  auto user1_id = core::TradingPlatformScope<Config>::Get()->AddMarketMember();
  {
    auto user1 =
        core::TradingPlatformScope<Config>::Get()->GetMarketMember(user1_id);
    user1->BuyCurrency<models::currencies::Usd>(30,
                                                models::currencies::Rubles(30));
  }

  auto user2_id = core::TradingPlatformScope<Config>::Get()->AddMarketMember();
  {
    auto user2 =
        core::TradingPlatformScope<Config>::Get()->GetMarketMember(user2_id);
    user2->SaleCurrency<models::currencies::Usd>(
        30, models::currencies::Rubles(30));
  }

  boost::asio::signal_set signals(ioc, SIGINT, SIGTERM, SIGHUP);
  signals.async_wait([&ioc](const boost::system::error_code& ec,
                            [[maybe_unused]] int32_t signal_number) {
    if (ec) {
      throw std::runtime_error("Error signal receiving");
    };
    ioc.stop();
  });
  ioc.run();

  {
    auto user1 =
        core::TradingPlatformScope<Config>::Get()->GetMarketMember(user1_id);
    std::cout << "User1 bought Usd:"
              << *user1->CurrencyBalance<models::currencies::Usd>()
              << std::endl;
    std::cout << "User1 sold out Rubles:"
              << *user1->CurrencyBalance<models::currencies::Rubles>()
              << std::endl;
  }
  {
    auto user2 =
        core::TradingPlatformScope<Config>::Get()->GetMarketMember(user2_id);
    std::cout << "User2 bought Rubles:"
              << *user2->CurrencyBalance<models::currencies::Rubles>()
              << std::endl;
    std::cout << "User2 sold out Usd:"
              << *user2->CurrencyBalance<models::currencies::Usd>()
              << std::endl;
  }

  core::TradingPlatformScope<Config>::Stop();
}