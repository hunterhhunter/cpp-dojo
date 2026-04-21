#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <string>
#include "my/SmallVector.h"
#include "string.h"


namespace {
    struct Counted {
        static inline int ctor = 0;
        static inline int dtor = 0;

        int value;
        explicit Counted(int v) : value(v)         { ++ctor; } 
        Counted(const Counted& o) : value(o.value) { ++ctor; }
        ~Counted()                                 { ++dtor; }
    };
}


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

    TEST(SmallVectorTest, PushBack_oversize) {
        SmallVector<int, 4> v;
        for (int i=0; i < 10; ++i) {
            v.push_back(i);
        }

        EXPECT_EQ(v.size(), 10);
        EXPECT_GE(v.capacity(), 10);
        for (size_t i=0; i < 10; ++i) {
            EXPECT_EQ(v[i], i);
        }
    };

    TEST(SmallVectorTest, Grow_ValuePreserve) {
        SmallVector<int, 2> v;
        v.push_back(10);
        v.push_back(20);
        v.push_back(30);
        EXPECT_EQ(v[0], 10);
        EXPECT_EQ(v[1], 20);
        EXPECT_EQ(v[2], 30);
    }

    TEST(SmallVectorTest, Grow_NoLeak) {
        Counted::ctor = Counted::dtor = 0;
        {
            SmallVector<Counted, 2> v;
            for (int i=0; i<10; ++i) v.push_back(Counted{i});
        }
        EXPECT_EQ(Counted::ctor, Counted::dtor);
    }

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
        SmallVector<std::string, 4> sv;

        fv.push_back(1.1f);
        iv.push_back(1);
        bv.push_back(true);
        sv.push_back("hello World!");

        EXPECT_FLOAT_EQ(fv[0], 1.1f);
        EXPECT_EQ(iv[0], 1);
        EXPECT_TRUE(bv[0]);
        EXPECT_EQ(sv[0], "hello World!");
    };

    TEST(SmallVectorTest, Dtor_Counted) {
        Counted::ctor = Counted::dtor = 0;

        {
            SmallVector<Counted, 4> v;

            v.push_back(Counted{1});    // Counted 임시객체 생성 + 복사로 ctor + 2;
            v.push_back(Counted{2});
            EXPECT_EQ(Counted::ctor, 4);
            EXPECT_EQ(Counted::dtor, 2);
        }
        EXPECT_EQ(Counted::dtor, 4);
    };

    TEST(SmallVectorTest, Dtor_Counted2) {
        Counted::ctor = Counted::dtor = 0;

        {
            SmallVector<Counted, 4> v;

            v.push_back(Counted{1});    // Counted 임시객체 생성 / 복사로 ctor + 2;
            v.push_back(Counted{2});
            EXPECT_EQ(Counted::ctor, 4);
            EXPECT_EQ(Counted::dtor, 2);
            {
                v.push_back(Counted{3});
                EXPECT_EQ(Counted::ctor, 6);
                EXPECT_EQ(Counted::dtor, 3);
            }
            EXPECT_EQ(Counted::dtor, 3);
        }
        EXPECT_EQ(Counted::dtor, 6);
    }

}
