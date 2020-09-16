#include <gtest.h>

TEST(TestCase, can_add_2_numbers) {
	int a = 3, b = 4;

	int c = a + b;

	ASSERT_EQ(c, 6);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
