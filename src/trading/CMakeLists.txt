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
  segment_analyzer)

set(sources
  wallet
  market_member
  )

prepare_sources("${headers}" "${sources}")