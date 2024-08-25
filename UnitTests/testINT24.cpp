#include "pch.h"
#include "BackBeat.h"

namespace BackBeat {

	namespace Int24Tests
	{

		TEST(TestInt24, DefaultConstructor_1) {
			int24 test;
			EXPECT_EQ(test[0], 0);
		}

		TEST(TestInt24, DefaultConstructor_2) {
			int24 test;
			EXPECT_EQ(test[1], 0);
		}

		TEST(TestInt24, DefaultConstructor_3) {
			int24 test;
			EXPECT_EQ(test[2], 0);
		}

		TEST(TestInt24, ConstructorByte_1) {
			byte t1 = 0x01;
			byte t2 = 0x02;
			byte t3 = 0x03;
			int24 test = int24(t1, t2, t3);
			EXPECT_EQ(test[0], t1);;
		}

		TEST(TestInt24, ConstructorByte_2) {
			byte t1 = 0x01;
			byte t2 = 0x02;
			byte t3 = 0x03;
			int24 test = int24(t1, t2, t3);
			EXPECT_EQ(test[1], t2);
		}

		TEST(TestInt24, ConstructorByte_3) {
			byte t1 = 0x01;
			byte t2 = 0x02;
			byte t3 = 0x03;
			int24 test = int24(t1, t2, t3);
			EXPECT_EQ(test[2], t3);
		}

		TEST(TestInt24, ConstructorShort_1) {
			short t = 1;
			int24 test1 = int24(t);
			int24 test2 = int24(0x00, 0x00, 0x01);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorShort_2) {
			short t = 0;
			int24 test1 = int24(t);
			int24 test2 = int24();
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorShort_3) {
			short t = -1;
			int24 test1 = int24(t);
			int24 test2 = int24(0xFF, 0xFF, 0xFF);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorShort_4) {
			short t = INT16_MAX;
			int24 test1 = int24(t);
			int24 test2 = int24(0x00, 0x7F, 0xFF);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorShort_5) {
			short t = -32768;
			int24 test1 = int24(t);
			int24 test2 = int24(0xFF, 0x80, 0x00);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorLong_1) {
			long t = 1;
			int24 test1 = int24(t);
			int24 test2 = int24(0, 0, 1);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorLong_2) {
			long t = 0;
			int24 test1 = int24(t);
			int24 test2 = int24();
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorLong_3) {
			long t = -1;
			int24 test1 = int24(t);
			int24 test2 = int24(0xFF, 0xFF, 0xFF);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorLong_4) {
			long t = 7372928;
			int24 test1 = int24(t);
			int24 test2 = int24(0x70, 0x80, 0x80);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorLong_5) {
			long t = -1048576;
			int24 test1 = int24(t);
			int24 test2 = int24(0xF0, 0x00, 0x00);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorInt_1) {
			int t = 1;
			int24 test1 = int24(t);
			int24 test2 = int24(0, 0, 1);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorInt_2) {
			int t = 0;
			int24 test1 = int24(t);
			int24 test2 = int24();
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorInt_3) {
			int t = -1;
			int24 test1 = int24(t);
			int24 test2 = int24(0xFF, 0xFF, 0xFF);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorInt_4) {
			int t = 7372928;
			int24 test1 = int24(t);
			int24 test2 = int24(0x70, 0x80, 0x80);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorInt_5) {
			int t = -1048576;
			int24 test1 = int24(t);
			int24 test2 = int24(0xF0, 0x00, 0x00);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorFloat_1) {
			float t = 1;
			int24 test1 = int24(t);
			int24 test2 = int24(0, 0, 1);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorFloat_2) {
			float t = 0;
			int24 test1 = int24(t);
			int24 test2 = int24();
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorFloat_3) {
			float t = -1;
			int24 test1 = int24(t);
			int24 test2 = int24(0xFF, 0xFF, 0xFF);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorFloat_4) {
			float t = 7372928;
			int24 test1 = int24(t);
			int24 test2 = int24(0x70, 0x80, 0x80);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorFloat_5) {
			float t = -1048576;
			int24 test1 = int24(t);
			int24 test2 = int24(0xF0, 0x00, 0x00);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorDouble_1) {
			double t = 1;
			int24 test1 = int24(t);
			int24 test2 = int24(0, 0, 1);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorDouble_2) {
			double t = 0;
			int24 test1 = int24(t);
			int24 test2 = int24();
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorDouble_3) {
			double t = -1;
			int24 test1 = int24(t);
			int24 test2 = int24(0xFF, 0xFF, 0xFF);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorDouble_4) {
			double t = 7372928;
			int24 test1 = int24(t);
			int24 test2 = int24(0x70, 0x80, 0x80);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, ConstructorDouble_5) {
			double t = -1048576;
			int24 test1 = int24(t);
			int24 test2 = int24(0xF0, 0x00, 0x00);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, EqualsTrue_1) {
			byte t1 = 0x01;
			byte t2 = 0x02;
			byte t3 = 0x03;
			int24 test1 = int24(t1, t2, t3);
			int24 test2 = int24(t1, t2, t3);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, EqualsTrue_2) {
			byte t1 = 0x00;
			byte t2 = 0x00;
			byte t3 = 0x00;
			int24 test1 = int24();
			int24 test2 = int24(t1, t2, t3);
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, EqualsTrue_3) {
			int24 test1 = int24();
			int24 test2 = int24();
			EXPECT_TRUE(test1 == test2);
		}

		TEST(TestInt24, EqualsFalse_1) {
			byte t1 = 0x01;
			byte t2 = 0x02;
			byte t3 = 0x03;
			int24 test1 = int24(t1, t2, t3);
			int24 test2 = int24();
			EXPECT_FALSE(test1 == test2);
		}

		TEST(TestInt24, EqualsFalse_2) {
			byte t1 = 0x01;
			byte t2 = 0x02;
			byte t3 = 0x03;
			int24 test1 = int24(t3, t2, t1);
			int24 test2 = int24(t1, t2, t3);
			EXPECT_FALSE(test1 == test2);
		}

		TEST(TestInt24, NotEqualsTrue_1) {
			byte t1 = 0x01;
			byte t2 = 0x02;
			byte t3 = 0x03;
			int24 test1 = int24(t1, t2, t3);
			int24 test2 = int24();
			EXPECT_TRUE(test1 != test2);
		}

		TEST(TestInt24, NotEqualsTrue_2) {
			byte t1 = 0x01;
			byte t2 = 0x02;
			byte t3 = 0x03;
			int24 test1 = int24(t3, t2, t1);
			int24 test2 = int24(t1, t2, t3);
			EXPECT_TRUE(test1 != test2);
		}

		TEST(TestInt24, NotEqualsFalse_1) {
			int24 test1 = int24();
			int24 test2 = int24();
			EXPECT_FALSE(test1 != test2);
		}

		TEST(TestInt24, NotEqualsFalse_2) {
			byte t1 = 0x00;
			byte t2 = 0x00;
			byte t3 = 0x00;
			int24 test1 = int24();
			int24 test2 = int24(t1, t2, t3);
			EXPECT_FALSE(test1 != test2);
		}

		TEST(TestInt24, LessThan_1) {
			int24 test1 = int24(1);
			int24 test2 = int24(2);
			EXPECT_TRUE(test1 < test2);
		}

		TEST(TestInt24, LessThan_2) {
			int24 test1 = int24(0);
			int24 test2 = int24(1);
			EXPECT_TRUE(test1 < test2);
		}

		TEST(TestInt24, LessThan_3) {
			int24 test1 = int24(-5);
			int24 test2 = int24(5);
			EXPECT_TRUE(test1 < test2);
		}

		TEST(TestInt24, LessThan_4) {
			int24 test1 = int24(-100);
			int24 test2 = int24(-10);
			EXPECT_TRUE(test1 < test2);
		}

		TEST(TestInt24, LessThan_5) {
			int24 test1 = int24();
			int24 test2 = int24();
			EXPECT_FALSE(test1 < test2);
		}

		TEST(TestInt24, LessThan_6) {
			int24 test1 = int24(20);
			int24 test2 = int24(2);
			EXPECT_FALSE(test1 < test2);
		}

		TEST(TestInt24, LessThan_7) {
			int24 test1 = int24(-2);
			int24 test2 = int24(-20);
			EXPECT_FALSE(test1 < test2);
		}

		TEST(TestInt24, IsPositive_1) {
			byte t1 = 0x01;
			byte t2 = 0x02;
			byte t3 = 0x03;
			int24 test = int24(t1, t2, t3);
			EXPECT_TRUE(test.IsPositive());
		}

		TEST(TestInt24, IsPositive_2) {
			byte t1 = 0xFF;
			byte t2 = 0x02;
			byte t3 = 0x03;
			int24 test = int24(t1, t2, t3);
			EXPECT_FALSE(test.IsPositive());
		}

		TEST(TestInt24, IsPositive_3) {
			int x = 10;
			int24 test = int24(x);
			EXPECT_TRUE(test.IsPositive());
		}

		TEST(TestInt24, IsPositive_4) {
			int x = -10;
			int24 test = int24(x);
			EXPECT_FALSE(test.IsPositive());
		}

		TEST(TestInt24, IsPositive_5) {
			int x = 0;
			int24 test = int24(x);
			EXPECT_FALSE(test.IsPositive());
		}

		TEST(TestInt24, ToLongPositiveTrue_1) {
			byte t1 = 0x00;
			byte t2 = 0x02;
			byte t3 = 0x03;
			int24 test = int24(t1, t2, t3);
			long testLong = 0x000203;
			EXPECT_EQ(testLong, test.ToLong());
		}

		TEST(TestInt24, ToLongPositiveTrue_2) {
			long testLong = 100000;
			int24 test = int24(testLong);
			EXPECT_EQ(testLong, test.ToLong());
		}

		TEST(TestInt24, ToLongPositiveTrue_3) {
			long testLong = 0;
			int24 test = int24(testLong);
			EXPECT_EQ(testLong, test.ToLong());
		}

		TEST(TestInt24, ToLongPositiveFalse_1) {
			byte t1 = 0x00;
			byte t2 = 0x02;
			byte t3 = 0x03;
			int24 test = int24(t1, t2, t3);
			long testLong = 100;
			EXPECT_NE(testLong, test.ToLong());
		}

		TEST(TestInt24, ToLongPositiveFalse_2) {
			long testLong = 1;
			int24 test = int24(2);
			EXPECT_NE(testLong, test.ToLong());
		}

		TEST(TestInt24, ToLongNegativeTrue_1) {
			byte t1 = 0xFF;
			byte t2 = 0xFF;
			byte t3 = 0xFF;
			int24 test = int24(t1, t2, t3);
			long testLong = -1;
			EXPECT_EQ(testLong, test.ToLong());
		}

		TEST(TestInt24, ToLongNegativeTrue_2) {
			byte t1 = 0x80;
			byte t2 = 0x00;
			byte t3 = 0x01;
			int24 test = int24(t1, t2, t3);
			long testLong = -8388607;
			EXPECT_EQ(testLong, test.ToLong());
		}

		TEST(TestInt24, ToLongNegativeTrue_3) {
			byte t1 = 0xFF;
			byte t2 = 0xF0;
			byte t3 = 0x01;
			int24 test = int24(t1, t2, t3);
			long testLong = -4095;
			EXPECT_EQ(testLong, test.ToLong());
		}

		TEST(TestInt24, ToLongNegativeFalse_1) {
			byte t1 = 0xFF;
			byte t2 = 0xFF;
			byte t3 = 0xFF;
			int24 test = int24(t1, t2, t3);
			long testLong = 16777215;
			EXPECT_NE(testLong, test.ToLong());
		}

		TEST(TestInt24, ToLongNegativeFalse_2) {
			byte t1 = 0x80;
			byte t2 = 0x00;
			byte t3 = 0x01;
			int24 test = int24(t1, t2, t3);
			long testLong = 8388609;
			EXPECT_NE(testLong, test.ToLong());
		}

		TEST(TestInt24, ToLongNegativeFalse_3) {
			byte t1 = 0xFF;
			byte t2 = 0xF0;
			byte t3 = 0x01;
			int24 test = int24(t1, t2, t3);
			long testLong = 16773121;
			EXPECT_NE(testLong, test.ToLong());
		}

		TEST(TestInt24, Add_1) {
			int24 test1 = int24(1) + int24(1);
			int24 test2 = int24(2);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Add_2) {
			int24 test1 = int24(0) + int24(0);
			int24 test2 = int24(0);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Add_3) {
			int24 test1 = int24(1) + int24(0);
			int24 test2 = int24(1);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Add_4) {
			int24 test1 = int24(-1) + int24(1);
			int24 test2 = int24(0);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Add_5) {
			int24 test1 = int24(-2) + int24(-2);
			int24 test2 = int24(-4);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Add_6) {
			int24 test1 = int24(1000000) + int24(234567);
			int24 test2 = int24(1234567);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Add_7) {
			int24 test1 = int24(-123455) + int24(123455);
			int24 test2 = int24(0);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Subtraction_1) {
			int24 test1 = int24(1) - int24(1);
			int24 test2 = int24(0);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Subtraction_2) {
			int24 test1 = int24(0) - int24(6666);
			int24 test2 = int24(-6666);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Subtraction_3) {
			int24 test1 = int24(100) - int24(0);
			int24 test2 = int24(100);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Subtraction_4) {
			int24 test1 = int24(-1) - int24(7);
			int24 test2 = int24(-8);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Subtraction_5) {
			int24 test1 = int24(-2) - int24(-2);
			int24 test2 = int24(0);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Subtraction_6) {
			int24 test1 = int24(1000000) - int24(999999);
			int24 test2 = int24(1);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Subtraction_7) {
			int24 test1 = int24(-123444) - int24(123444);
			int24 test2 = int24(-246888);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Multiplication_1) {
			int24 test1 = int24(10) * int24(1);
			int24 test2 = int24(10);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Multiplication_2) {
			int24 test1 = int24(7) * int24(8);
			int24 test2 = int24(56);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Multiplication_3) {
			int24 test1 = int24(8) * int24(7);
			int24 test2 = int24(56);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Multiplication_4) {
			int24 test1 = int24(100) * int24(0);
			int24 test2 = int24(0);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Multiplication_5) {
			int24 test1 = int24(-2) * int24(-2);
			int24 test2 = int24(4);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Multiplication_6) {
			int24 test1 = int24(100) * int24(-1999);
			int24 test2 = int24(-199900);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Multiplication_7) {
			int24 test1 = int24(256) * int24(256);
			int24 test2 = int24(65536);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Division_1) {
			int24 test1 = int24(10) / int24(1);
			int24 test2 = int24(10);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Division_2) {
			int24 test1 = int24(64) / int24(8);
			int24 test2 = int24(8);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Division_3) {
			int24 test1 = int24(-49) / int24(7);
			int24 test2 = int24(-7);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Division_4) {
			int24 test1 = int24(-100) / int24(-10);
			int24 test2 = int24(10);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Division_5) {
			int24 test1 = int24(0) / int24(-2000);
			int24 test2 = int24(0);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Division_6) {
			int24 test1 = int24(-300000) / int24(300000);
			int24 test2 = int24(-1);
			EXPECT_TRUE(test2 == test1);
		}

		TEST(TestInt24, Division_7) {
			int24 test1 = int24(65536) / int24(-256);
			int24 test2 = int24(-256);
			EXPECT_TRUE(test2 == test1);
		}

	}
}