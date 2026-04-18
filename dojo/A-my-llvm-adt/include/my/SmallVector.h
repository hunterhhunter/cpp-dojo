#pragma once
#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>
using namespace std;

namespace my {
    template <typename T, std::size_t N>
    class SmallVector {
    public:
        SmallVector() {
            this->size_ = 0;
        }

        ~SmallVector() {

        }

        void push_back(const T& v) {
            // 사이즈 벗어나지 않는지 확인
            assert(size_ < N && "SmallVector capacity exceeded");
            // size_-1 위치에 v 할당 및 size_ 1 증가
            // 1. 슬롯 주소 계산: i번째 T의 시작 위치
            T* slot = reinterpret_cast<T*>(&buf_[size_ * sizeof(T)]);

            // new (주소) T(인자): 이 메모리 자리에 T 객체를 만들어라.
            // 메모리 할당 X 객체만 생성
            new (slot) T(v);

            ++size_;
        }
        std::size_t size() const { return size_; }
        std::size_t capacity() const { return N;}             // == N
        // T& operator[](std::size_t i);
        // const T& operator[](std::size_t i) const;

    private:
        // 그냥 메모리 덩어리
        // T의 메모리 할당을 N번 받지 않고 바로 연속 메모리를 할당해 T를 넣게함.
        // T의 N번 할당 < T * N 사이즈 할당 (효율 up)
        // + alignas로 T의 크기에 맞게 메모리 정렬
        alignas(T) std::byte buf_[N * sizeof(T)];
        std::size_t size_;
    };
}