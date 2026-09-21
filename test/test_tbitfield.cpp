#include <gtest/gtest.h>
#include <vector>
#include <sstream>
#include "tbitfield.h"
TEST(TBitField, can_create_bitfield_with_positive_length)
{
  ASSERT_NO_THROW(TBitField bf(3));
}

TEST(TBitField, can_get_length)
{
  TBitField bf(3);

  EXPECT_EQ(3, bf.GetLength());
}

TEST(TBitField, new_bitfield_is_set_to_zero)
{
  TBitField bf(100);

  int sum = 0;
  for (int i = 0; i < bf.GetLength(); i++)
  {
    sum += bf.GetBit(i);
  }

  EXPECT_EQ(0, sum);
}

TEST(TBitField, can_set_bit)
{
  TBitField bf(10);

  EXPECT_EQ(0, bf.GetBit(3));

  bf.SetBit(3);
  EXPECT_NE(0, bf.GetBit(3));
}

TEST(TBitField, can_clear_bit)
{
  TBitField bf(10);

  int bitIdx = 3;

  bf.SetBit(bitIdx);
  EXPECT_NE(0, bf.GetBit(bitIdx));

  bf.ClrBit(bitIdx);
  EXPECT_EQ(0, bf.GetBit(bitIdx));
}

TEST(TBitField, throws_when_create_bitfield_with_negative_length)
{
  ASSERT_ANY_THROW(TBitField bf(-3));
}

TEST(TBitField, throws_when_set_bit_with_negative_index)
{
  TBitField bf(10);

  ASSERT_ANY_THROW(bf.SetBit(-3));
}

TEST(TBitField, throws_when_set_bit_with_too_large_index)
{
  TBitField bf(10);

  ASSERT_ANY_THROW(bf.SetBit(11));
}

TEST(TBitField, throws_when_get_bit_with_negative_index)
{
  TBitField bf(10);

  ASSERT_ANY_THROW(bf.GetBit(-3));
}

TEST(TBitField, throws_when_get_bit_with_too_large_index)
{
  TBitField bf(10);

  ASSERT_ANY_THROW(bf.GetBit(11));
}

TEST(TBitField, throws_when_clear_bit_with_negative_index)
{
  TBitField bf(10);

  ASSERT_ANY_THROW(bf.ClrBit(-3));
}

TEST(TBitField, throws_when_clear_bit_with_too_large_index)
{
  TBitField bf(10);

  ASSERT_ANY_THROW(bf.ClrBit(11));
}

TEST(TBitField, can_assign_bitfields_of_equal_size)
{
  const int size = 2;
  TBitField bf1(size), bf2(size);
  for (int i = 0; i < size; i++)
  {
    bf1.SetBit(i);
  }
  bf2 = bf1;

  EXPECT_NE(0, bf2.GetBit(0));
  EXPECT_NE(0, bf2.GetBit(1));
}

TEST(TBitField, assign_operator_changes_bitfield_size)
{
  const int size1 = 2, size2 = 5;
  TBitField bf1(size1), bf2(size2);
  for (int i = 0; i < size1; i++)
  {
    bf1.SetBit(i);
  }
  bf2 = bf1;

  EXPECT_EQ(2, bf2.GetLength());
}

TEST(TBitField, can_assign_bitfields_of_non_equal_size)
{
  const int size1 = 2, size2 = 5;
  TBitField bf1(size1), bf2(size2);
  for (int i = 0; i < size1; i++)
  {
    bf1.SetBit(i);
  }
  bf2 = bf1;

  EXPECT_NE(0, bf2.GetBit(0));
  EXPECT_NE(0, bf2.GetBit(1));
}

TEST(TBitField, compare_equal_bitfields_of_equal_size)
{
  const int size = 2;
  TBitField bf1(size), bf2(size);
  for (int i = 0; i < size; i++)
  {
    bf1.SetBit(i);
  }
  bf2 = bf1;

  EXPECT_EQ(bf1, bf2);
}

TEST(TBitField, or_operator_applied_to_bitfields_of_equal_size)
{
  const int size = 4;
  TBitField bf1(size), bf2(size), expBf(size);
  // bf1 = 0011
  bf1.SetBit(2);
  bf1.SetBit(3);
  // bf2 = 0101
  bf2.SetBit(1);
  bf2.SetBit(3);

  // expBf = 0111
  expBf.SetBit(1);
  expBf.SetBit(2);
  expBf.SetBit(3);

  EXPECT_EQ(expBf, bf1 | bf2);
}

TEST(TBitField, or_operator_applied_to_bitfields_of_non_equal_size)
{
  const int size1 = 4, size2 = 5;
  TBitField bf1(size1), bf2(size2), expBf(size2);
  // bf1 = 0011
  bf1.SetBit(2);
  bf1.SetBit(3);
  // bf2 = 01010
  bf2.SetBit(1);
  bf2.SetBit(3);

  // expBf = 01110
  expBf.SetBit(1);
  expBf.SetBit(2);
  expBf.SetBit(3);

  EXPECT_EQ(expBf, bf1 | bf2);
}

TEST(TBitField, and_operator_applied_to_bitfields_of_equal_size)
{
  const int size = 4;
  TBitField bf1(size), bf2(size), expBf(size);
  // bf1 = 0011
  bf1.SetBit(2);
  bf1.SetBit(3);
  // bf2 = 0101
  bf2.SetBit(1);
  bf2.SetBit(3);

  // expBf = 0001
  expBf.SetBit(3);

  EXPECT_EQ(expBf, bf1 & bf2);
}

TEST(TBitField, and_operator_applied_to_bitfields_of_non_equal_size)
{
  const int size1 = 4, size2 = 5;
  TBitField bf1(size1), bf2(size2), expBf(size2);
  // bf1 = 0011
  bf1.SetBit(2);
  bf1.SetBit(3);
  // bf2 = 01010
  bf2.SetBit(1);
  bf2.SetBit(3);

  // expBf = 00010
  expBf.SetBit(3);

  EXPECT_EQ(expBf, bf1 & bf2);
}

TEST(TBitField, can_invert_bitfield)
{
  const int size = 2;
  TBitField bf(size), negBf(size), expNegBf(size);
  // bf = 01
  bf.SetBit(1);
  negBf = ~bf;

  // expNegBf = 10
  expNegBf.SetBit(0);

  EXPECT_EQ(expNegBf, negBf);
}

TEST(TBitField, can_invert_large_bitfield)
{
  const int size = 38;
  TBitField bf(size), negBf(size), expNegBf(size);
  bf.SetBit(35);
  negBf = ~bf;

  for(int i = 0; i < size; i++)
    expNegBf.SetBit(i);
  expNegBf.ClrBit(35);

  EXPECT_EQ(expNegBf, negBf);
}

TEST(TBitField, invert_plus_and_operator_on_different_size_bitfield)
{
  const int firstSze = 4, secondSize = 8;
  TBitField firstBf(firstSze), negFirstBf(firstSze), secondBf(secondSize), testBf(secondSize);
  // firstBf = 0001
  firstBf.SetBit(0);
  negFirstBf = ~firstBf;
  // negFirstBf = 1110

  // secondBf = 00011000
  secondBf.SetBit(3);
  secondBf.SetBit(4);

  // testBf = 00001000
  testBf.SetBit(3);

  EXPECT_EQ(secondBf & negFirstBf, testBf);
}

TEST(TBitField, can_invert_many_random_bits_bitfield)
{
  const int size = 38;
  TBitField bf(size), negBf(size), expNegBf(size);

  std::vector<int> bits;
  bits.push_back(0);
  bits.push_back(1);
  bits.push_back(14);
  bits.push_back(16);
  bits.push_back(33);
  bits.push_back(37);

  for (unsigned int i = 0; i < bits.size(); i++)
    bf.SetBit(bits[i]);

  negBf = ~bf;

  for(int i = 0; i < size; i++)
    expNegBf.SetBit(i);
  for (unsigned int i = 0; i < bits.size(); i++)
    expNegBf.ClrBit(bits[i]);

  EXPECT_EQ(expNegBf, negBf);
}

TEST(TBitField, bitfields_with_different_bits_are_not_equal)
{
  const int size = 4;
  TBitField bf1(size), bf2(size);

  bf1.SetBit(1);
  bf1.SetBit(3);

  bf2.SetBit(1);
  bf2.SetBit(2);

  EXPECT_NE(bf1, bf2);
}
#include <gtest/gtest.h>
#include <vector>
#include <sstream>
#include "tbitfield.h"


// ============================================================
// НУЛЕВАЯ ДЛИНА
// ============================================================

TEST(TBitFieldExtra, ZeroLength)
{
    TBitField bf(0);

    EXPECT_EQ(bf.GetLength(), 0);
}

TEST(TBitFieldExtra, ZeroLengthDoesNotAllowAccess)
{
    TBitField bf(0);

    EXPECT_THROW(bf.GetBit(0), std::out_of_range);
    EXPECT_THROW(bf.SetBit(0), std::out_of_range);
    EXPECT_THROW(bf.ClrBit(0), std::out_of_range);
}


// ============================================================
// ГРАНИЦЫ 32 БИТ
// ============================================================

TEST(TBitFieldExtra, Exactly32Bits)
{
    TBitField bf(32);

    EXPECT_EQ(bf.GetLength(), 32);

    EXPECT_NO_THROW(bf.SetBit(0));
    EXPECT_NO_THROW(bf.SetBit(31));

    EXPECT_EQ(bf.GetBit(0), 1);
    EXPECT_EQ(bf.GetBit(31), 1);

    EXPECT_THROW(bf.GetBit(32), std::out_of_range);
}

TEST(TBitFieldExtra, ThirtyThreeBits)
{
    TBitField bf(33);

    EXPECT_EQ(bf.GetLength(), 33);

    EXPECT_NO_THROW(bf.SetBit(0));
    EXPECT_NO_THROW(bf.SetBit(31));
    EXPECT_NO_THROW(bf.SetBit(32));

    EXPECT_EQ(bf.GetBit(0), 1);
    EXPECT_EQ(bf.GetBit(31), 1);
    EXPECT_EQ(bf.GetBit(32), 1);

    EXPECT_THROW(bf.GetBit(33), std::out_of_range);
}


// ============================================================
// ГРАНИЦЫ 64 БИТ
// ============================================================

TEST(TBitFieldExtra, Exactly64Bits)
{
    TBitField bf(64);

    bf.SetBit(0);
    bf.SetBit(31);
    bf.SetBit(32);
    bf.SetBit(63);

    EXPECT_EQ(bf.GetBit(0), 1);
    EXPECT_EQ(bf.GetBit(31), 1);
    EXPECT_EQ(bf.GetBit(32), 1);
    EXPECT_EQ(bf.GetBit(63), 1);

    EXPECT_THROW(bf.GetBit(64), std::out_of_range);
}

TEST(TBitFieldExtra, SixtyFiveBits)
{
    TBitField bf(65);

    bf.SetBit(0);
    bf.SetBit(32);
    bf.SetBit(64);

    EXPECT_EQ(bf.GetBit(0), 1);
    EXPECT_EQ(bf.GetBit(32), 1);
    EXPECT_EQ(bf.GetBit(64), 1);

    EXPECT_THROW(bf.GetBit(65), std::out_of_range);
}


// ============================================================
// ПЕРВЫЙ И ПОСЛЕДНИЙ БИТ
// ============================================================

TEST(TBitFieldExtra, FirstAndLastBit)
{
    TBitField bf(10);

    bf.SetBit(0);
    bf.SetBit(9);

    EXPECT_EQ(bf.GetBit(0), 1);
    EXPECT_EQ(bf.GetBit(9), 1);

    for (int i = 1; i < 9; i++)
        EXPECT_EQ(bf.GetBit(i), 0);
}


// ============================================================
// КОНСТРУКТОР КОПИРОВАНИЯ
// ============================================================

TEST(TBitFieldExtra, CopyConstructor)
{
    TBitField original(40);

    original.SetBit(0);
    original.SetBit(10);
    original.SetBit(31);
    original.SetBit(32);
    original.SetBit(39);

    TBitField copy(original);

    EXPECT_EQ(copy.GetLength(), original.GetLength());
    EXPECT_EQ(copy, original);
}

TEST(TBitFieldExtra, CopyIsIndependent)
{
    TBitField original(40);

    original.SetBit(10);

    TBitField copy(original);

    copy.SetBit(20);

    EXPECT_EQ(original.GetBit(10), 1);
    EXPECT_EQ(original.GetBit(20), 0);

    EXPECT_EQ(copy.GetBit(10), 1);
    EXPECT_EQ(copy.GetBit(20), 1);
}


// ============================================================
// ПРИСВАИВАНИЕ
// ============================================================

TEST(TBitFieldExtra, SelfAssignment)
{
    TBitField bf(20);

    bf.SetBit(3);
    bf.SetBit(15);

    bf = bf;

    EXPECT_EQ(bf.GetLength(), 20);
    EXPECT_EQ(bf.GetBit(3), 1);
    EXPECT_EQ(bf.GetBit(15), 1);
}

TEST(TBitFieldExtra, AssignmentCreatesIndependentCopy)
{
    TBitField first(40);
    first.SetBit(5);

    TBitField second(40);
    second = first;

    second.SetBit(20);

    EXPECT_EQ(first.GetBit(5), 1);
    EXPECT_EQ(first.GetBit(20), 0);

    EXPECT_EQ(second.GetBit(5), 1);
    EXPECT_EQ(second.GetBit(20), 1);
}


// ============================================================
// ОЧИСТКА БИТОВ
// ============================================================

TEST(TBitFieldExtra, ClearAllBits)
{
    TBitField bf(40);

    for (int i = 0; i < 40; i++)
        bf.SetBit(i);

    for (int i = 0; i < 40; i++)
        bf.ClrBit(i);

    for (int i = 0; i < 40; i++)
        EXPECT_EQ(bf.GetBit(i), 0);
}


// ============================================================
// OR
// ============================================================

TEST(TBitFieldExtra, OrWithZero)
{
    TBitField first(40);
    TBitField second(40);

    first.SetBit(5);
    first.SetBit(20);

    TBitField result = first | second;

    EXPECT_EQ(result, first);
}

TEST(TBitFieldExtra, OrWithItself)
{
    TBitField first(40);

    first.SetBit(5);
    first.SetBit(20);
    first.SetBit(39);

    TBitField result = first | first;

    EXPECT_EQ(result, first);
}


// ============================================================
// AND
// ============================================================

TEST(TBitFieldExtra, AndWithZero)
{
    TBitField first(40);
    TBitField second(40);

    first.SetBit(5);
    first.SetBit(20);

    TBitField result = first & second;

    EXPECT_EQ(result.GetLength(), 40);

    for (int i = 0; i < 40; i++)
        EXPECT_EQ(result.GetBit(i), 0);
}

TEST(TBitFieldExtra, AndWithItself)
{
    TBitField first(40);

    first.SetBit(5);
    first.SetBit(20);
    first.SetBit(39);

    TBitField result = first & first;

    EXPECT_EQ(result, first);
}


// ============================================================
// NOT
// ============================================================

TEST(TBitFieldExtra, NotZero)
{
    TBitField bf(10);

    TBitField result = ~bf;

    EXPECT_EQ(result.GetLength(), 10);

    for (int i = 0; i < 10; i++)
        EXPECT_EQ(result.GetBit(i), 1);
}

TEST(TBitFieldExtra, NotFull)
{
    TBitField bf(10);

    for (int i = 0; i < 10; i++)
        bf.SetBit(i);

    TBitField result = ~bf;

    for (int i = 0; i < 10; i++)
        EXPECT_EQ(result.GetBit(i), 0);
}

TEST(TBitFieldExtra, Not33Bits)
{
    TBitField bf(33);

    bf.SetBit(0);
    bf.SetBit(32);

    TBitField result = ~bf;

    EXPECT_EQ(result.GetLength(), 33);

    EXPECT_EQ(result.GetBit(0), 0);
    EXPECT_EQ(result.GetBit(32), 0);

    for (int i = 1; i < 32; i++)
        EXPECT_EQ(result.GetBit(i), 1);
}


// ============================================================
// ВВОД
// ============================================================

TEST(TBitFieldExtra, InputOperator)
{
    TBitField bf(8);

    std::stringstream input("10110010");

    input >> bf;

    EXPECT_EQ(bf.GetBit(7), 1);
    EXPECT_EQ(bf.GetBit(6), 0);
    EXPECT_EQ(bf.GetBit(5), 1);
    EXPECT_EQ(bf.GetBit(4), 1);
    EXPECT_EQ(bf.GetBit(3), 0);
    EXPECT_EQ(bf.GetBit(2), 0);
    EXPECT_EQ(bf.GetBit(1), 1);
    EXPECT_EQ(bf.GetBit(0), 0);
}


// ============================================================
// ВЫВОД
// ============================================================

TEST(TBitFieldExtra, OutputOperator)
{
    TBitField bf(8);

    bf.SetBit(7);
    bf.SetBit(5);
    bf.SetBit(4);
    bf.SetBit(1);

    std::stringstream output;

    output << bf;

    EXPECT_EQ(output.str(), "10110010");
}


// ============================================================
// ВВОД ПОСЛЕ ПРЕДЫДУЩИХ ЗНАЧЕНИЙ
// ============================================================

TEST(TBitFieldExtra, InputReplacesOldValues)
{
    TBitField bf(8);

    for (int i = 0; i < 8; i++)
        bf.SetBit(i);

    std::stringstream input("00000000");

    input >> bf;

    for (int i = 0; i < 8; i++)
        EXPECT_EQ(bf.GetBit(i), 0);
}


// ============================================================
// КОМБИНАЦИЯ ОПЕРАЦИЙ
// ============================================================

TEST(TBitFieldExtra, ComplexBitOperations)
{
    TBitField a(8);
    TBitField b(8);

    // a = 10110000
    a.SetBit(7);
    a.SetBit(5);
    a.SetBit(4);

    // b = 11000000
    b.SetBit(7);
    b.SetBit(6);

    TBitField result = (a | b) & ~a;

    // a | b = 11110000
    // ~a    = 01001111
    // AND   = 01000000

    EXPECT_EQ(result.GetLength(), 8);
    EXPECT_EQ(result.GetBit(6), 1);

    for (int i = 0; i < 8; i++)
    {
        if (i != 6)
            EXPECT_EQ(result.GetBit(i), 0);
    }
}