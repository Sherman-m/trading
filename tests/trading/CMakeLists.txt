set(headers)

set(sources
  currency_test
  wallet_test
  market_member_test
  order_test
  deal_test
  default_storage_test
  trading_platform_test)

generate_test_targets("${headers}" "${sources}")