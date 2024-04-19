set(headers
  currency
  wallet
  market_member
  market_member_fwd
  order_details
  order
  deal_details
  deal)

set(sources
  wallet
  market_member
  )

prepare_sources("${headers}" "${sources}")