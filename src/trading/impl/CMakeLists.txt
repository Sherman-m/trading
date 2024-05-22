set(headers 
  deal_details_impl
  deal_impl
  default_storage_impl
  market_member_impl
  order_details_impl
  order_impl
  orders_matching_impl
  segment_analyzer_impl
  segment_impl
  trading_platform_impl
  wallet_impl)

set(sources
  market_member_impl
  trading_platform_impl
  wallet_impl)

prepare_sources("${headers}" "${sources}")