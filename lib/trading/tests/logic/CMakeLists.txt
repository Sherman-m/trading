set(subdirs)

set(hpp)

set(cpp
  currency_test
  wallet_test
  market_member_test
  order_test
  deal_test
  default_storage_test
  trading_platform_test)

prepare_hpp_cpp("${hpp}" "${cpp}" "${subdirs}")
