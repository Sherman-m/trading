set(headers)

set(sources
  currency_test
  wallet_test)

generate_test_targets("${headers}" "${sources}")