#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <string>
#include "my/SmallVector.h"
#include "string.h"

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
    };

    TEST(SmallVectorTest, Index_InsertionOrder) {
        SmallVector<int, 4> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        v.push_back(4);
        
        EXPECT_EQ(v[0], 1);
        EXPECT_EQ(v[1], 2);
        EXPECT_EQ(v[2], 3);
        EXPECT_EQ(v[3], 4);
    };

    TEST(SmallVectorTest, Index_MutableReference) {
        SmallVector<int, 4>v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        v.push_back(4);
        
        EXPECT_EQ(v[0], 1);
        v[0] = 5;
        EXPECT_EQ(v[0], 5);
        EXPECT_EQ(v[1], 2);
    };

    TEST(SmallVectorTest, Index_ConstRead) {
        SmallVector<int, 4> v;
        v.push_back(4);
        const auto& cv = v;
        EXPECT_EQ(cv[0], 4);
    };

    TEST(SmallVectorDeathTest, Index_OORAssert){
        SmallVector<int, 4> v;
        EXPECT_DEATH((void)v[0], "SmallVector Out Of Index Error");

        v.push_back(1);

        EXPECT_DEATH((void)v[1], "SmallVector Out Of Index Error");
    };

    TEST(SmallVectorTest, Ctor_CreateEmpty) {
        SmallVector<int, 4> v;
        EXPECT_EQ(v.size(), 0);
        EXPECT_EQ(v.capacity(), 4);
    };

    TEST(SmallVectorTest, Ctor_Usable) {
        SmallVector<int, 4> v1;

        v1.push_back(10);
        EXPECT_EQ(v1[0], 10);
        EXPECT_EQ(v1.size(), 1);
    };

    TEST(SmallVectorTest, Ctor_VariousType) {
        SmallVector<float, 4> fv;
        SmallVector<int, 4> iv;
        SmallVector<bool, 4> bv;
        SmallVector<string, 4> sv;

        fv.push_back(1.1f);
        iv.push_back(1);
        bv.push_back(true);
        sv.push_back("hello World!");

        EXPECT_FLOAT_EQ(fv[0], 1.1f);
        EXPECT_EQ(iv[0], 1);
        EXPECT_TRUE(bv[0]);
        EXPECT_EQ(sv[0], "hello World!");
    };

    TEST(SmallVectorDeathTest, Ctor_Under0) {
        // EXPECT_EXIT(SmallVector<int, 0>, predicate, matcher)
    }


}
