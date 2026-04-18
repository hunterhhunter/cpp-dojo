# header only library INTERFACE target decalare
add_library(my_llvm_adt INTERFACE)
target_include_directories(my_llvm_adt INTERFACE include)


# --- test ---
# 새 자료구조 추가할 때마다 아래 2줄만 복제
add_executable(SmallVectorTest tests/SmallVectorTest.cpp)
target_link_libraries(SmallVectorTest PRIVATE my_llvm_adt GTest::gtest_main)
add_test(NAME SmallVectorTest COMMAND SmallVectortest)