#include <gtest.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(SimpleTest, can_add) {
    // Arrange
    int a = 10;
    int b = 3;

    // Act
    int c = a + b;

    // Assert
    ASSERT_EQ(13, c);
}