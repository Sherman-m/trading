#include <gtest/gtest.h>

#include "../../src/trading/currency.hpp"

using namespace trading::currency;

TEST(CurrencyOperators, LvalueAddCurrency) {
  Usd u1(100);
  Usd u2(200);

  Usd u3 = u1 + u2;
  u1 += u2;
  ASSERT_EQ(u1, u3);
}

TEST(CurrencyOperators, RvalueAddingCurrency) {
  Usd u1(100);

  Usd u3 = u1 + Usd(100) + Usd(100);
  u1 += Usd(100) + Usd(100);
  ASSERT_EQ(u1, u3);
}

TEST(CurrencyOperators, UnaryPlusCurrency) {
  Usd u1(100);

  Usd u2 = +u1;
  ASSERT_EQ(u2, u1);
}

TEST(CurrencyOperators, LvalueSubCurrency) {
  Usd u1(100);
  Usd u2(200);

  Usd u3 = u2 - u1;
  u2 -= u1;
  ASSERT_EQ(u2, u3);
}

TEST(CurrencyOperators, RvalueSubCurrency) {
  Usd u1(100);
  Usd u2(200);

  Usd u3 = u2 - Usd(100) - Usd(100);
  u2 -= Usd(200);
  ASSERT_EQ(u2, u3);
}

TEST(CurrencyOperators, UnaryMinusCurrency) {
  Usd u1(100);

  Usd u2 = -u1;
  ASSERT_EQ(u2, -u1);
}

TEST(CurrencyValue, NegativeValue) {
  Usd u1(0);
  Usd u2(500);
  u1 -= u2;
  ASSERT_EQ(u1, -u2);
}
