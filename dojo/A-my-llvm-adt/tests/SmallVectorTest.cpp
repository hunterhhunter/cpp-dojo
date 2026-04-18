#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include "my/SmallVector.h"

namespace my{
    TEST(SmallVectorTest, Constructed_Size) {
        SmallVector<int, 4> v;
        // 선언한 사이즈와 size() 메서드가 같은 수를 반환하는지
        EXPECT_EQ(0, v.size());
    };
    
    TEST(SmallVectorTest, Constructed_buffer) {
        SmallVector<int, 4> v;
        // 선언한 size와 buffer의 size가 같은지
        EXPECT_EQ(4, v.capacity());
    };

    TEST(SmallVectorTest, PushBack_size) {
        SmallVector<int, 4> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);

        EXPECT_EQ(3, v.size());
    };

    TEST(SmallVectorDeathTest, PushBack_oversize) {
        SmallVector<int, 4> v;
        v.push_back(4);
        v.push_back(1);
        v.push_back(1);
        v.push_back(1);
        EXPECT_DEATH(v.push_back(1), "SmallVector capacity exceeded");
    }
}
