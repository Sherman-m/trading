set(headers
  currency
  wallet
  market_member
  market_member_fwd
  order_details
  order
  order_fwd
  deal_details
  deal
  deal_fwd
  storage
  default_storage
  segment
  segment_analyzer
  segment_fwd
  trading_platform
  trading)

set(sources
  wallet
  market_member
  trading_platform
  )

prepare_sources("${headers}" "${sources}")