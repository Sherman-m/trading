add_subdirectory(impl)

set(headers
  currency
  wallet
  market_member
  market_member_fwd
  order_details
  order_fwd
  orders_matching_fwd
  orders_matching
  order
  deal_details
  deal
  deal_fwd
  storage
  default_storage
  segment
  segment_analyzer
  segment_fwd
  trading_platform
  trading_side
  trading)

set(sources)

prepare_sources("${headers}" "${sources}")