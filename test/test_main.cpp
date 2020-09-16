#include <gtest.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(TestCase, can_add_2_numbers)
{
	int a = 3, b = 4;
	int c = a + b;
		ASSERT_EQ(7, c);
}