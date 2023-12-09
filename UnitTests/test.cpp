#include "pch.h"
#include "BackBeat.h"

namespace BackBeat {
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

	TEST(TestInt24, IsPositiveTrue) {
		byte t1 = 0x01;
		byte t2 = 0x02;
		byte t3 = 0x03;
		int24 test = int24(t1, t2, t3);
		EXPECT_TRUE(test.IsPositive());
	}

	TEST(TestInt24, IsPositiveFalse) {
		byte t1 = 0xFF;
		byte t2 = 0x02;
		byte t3 = 0x03;
		int24 test = int24(t1, t2, t3);
		EXPECT_FALSE(test.IsPositive());
	}

	TEST(TestInt24, ToLongPositiveTrue) {
		byte t1 = 0x00;
		byte t2 = 0x02;
		byte t3 = 0x03;
		int24 test = int24(t1, t2, t3);
		long testLong = 0x000203;
		EXPECT_EQ(testLong, test.toLong());
	}

	TEST(TestInt24, ToLongPositiveFalse) {
		byte t1 = 0x00;
		byte t2 = 0x02;
		byte t3 = 0x03;
		int24 test = int24(t1, t2, t3);
		long testLong = 100;
		EXPECT_NE(testLong, test.toLong());
	}

	TEST(TestInt24, ToLongNegativeTrue_1) {
		byte t1 = 0xFF;
		byte t2 = 0xFF;
		byte t3 = 0xFF;
		int24 test = int24(t1, t2, t3);
		long testLong = -1;
		EXPECT_EQ(testLong, test.toLong());
	}

	TEST(TestInt24, ToLongNegativeTrue_2) {
		byte t1 = 0x80;
		byte t2 = 0x00;
		byte t3 = 0x01;
		int24 test = int24(t1, t2, t3);
		long testLong = -8388607;
		EXPECT_EQ(testLong, test.toLong());
	}

	TEST(TestInt24, ToLongNegativeTrue_3) {
		byte t1 = 0xFF;
		byte t2 = 0xF0;
		byte t3 = 0x01;
		int24 test = int24(t1, t2, t3);
		long testLong = -4095;
		EXPECT_EQ(testLong, test.toLong());
	}

	TEST(TestInt24, ToLongNegativeFalse_1) {
		byte t1 = 0xFF;
		byte t2 = 0xFF;
		byte t3 = 0xFF;
		int24 test = int24(t1, t2, t3);
		long testLong = 16777215;
		EXPECT_NE(testLong, test.toLong());
	}

	TEST(TestInt24, ToLongNegativeFalse_2) {
		byte t1 = 0x80;
		byte t2 = 0x00;
		byte t3 = 0x01;
		int24 test = int24(t1, t2, t3);
		long testLong = 8388609;
		EXPECT_NE(testLong, test.toLong());
	}

	TEST(TestInt24, ToLongNegativeFalse_3) {
		byte t1 = 0xFF;
		byte t2 = 0xF0;
		byte t3 = 0x01;
		int24 test = int24(t1, t2, t3);
		long testLong = 16773121;
		EXPECT_NE(testLong, test.toLong());
	}


}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}