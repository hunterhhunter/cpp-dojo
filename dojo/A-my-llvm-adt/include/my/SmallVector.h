#pragma once
#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <new>
using namespace std;

namespace my {
    template <typename T, std::size_t N>
    class SmallVector {
        static_assert(N > 0, "SmallVector size should be bigger than 0");
    public:
        SmallVector() {
        }

        ~SmallVector() {
            for (std::size_t i=0; i<size_; ++i) {
                data()[i].~T();
            }

            // 객체와 메모리 생명주기는 서로 다르기 때문에 명시적으로 해제 해주어야함.
            if (!is_small()) {
                ::operator delete(data_, std::align_val_t{alignof(T)});
            }
        }

        SmallVector(const SmallVector&) = delete;
        SmallVector& operator=(const SmallVector&) = delete;
        SmallVector(SmallVector&&) = delete;
        SmallVector& operator=(SmallVector&&) = delete;

        void push_back(const T& v) {
            // 사이즈 벗어나지 않는지 확인
            if (size_ == capacity()) grow();

            new (data_ + size_) T(v);

            ++size_;

            // size_-1 위치에 v 할당 및 size_ 1 증가
            // 1. 슬롯 주소 계산: i번째 T의 시작 위치
            // T* slot = reinterpret_cast<T*>(&buf_[size_ * sizeof(T)]);

            // new (주소) T(인자): 이 메모리 자리에 T 객체를 만들어라.
            // 메모리 할당 X 객체만 생성
            // new (slot) T(v);

            // ++size_;
        }

        std::size_t size() const { return size_; }

        std::size_t capacity() const { return capacity_;}

        T& operator[](std::size_t i) {
            assert(i < size_ && "SmallVector Out Of Index Error");
            // i 위치 계산
            // std::byte* byte_addr = &buf_[i * sizeof(T)];
            // T* slot = reinterpret_cast<T*>(byte_addr);

            return data()[i];
        }
        const T& operator[](std::size_t i) const {
            assert(i < size_ && "SmallVector Out Of Index Error");
            // i 위치 계산 
            // 아래가 맞는 원칙 const 선언
            // const std::byte* byte_addr = &buf_[i * sizeof(T)];
            // const T* slot = reinterpret_cast<const T*>(byte_addr);
            // auto* byte_addr = &buf_[i * sizeof(T)];
            // auto* slot = reinterpret_cast<const T*>(byte_addr);

            return data()[i];
        }

    private:
        // 그냥 메모리 덩어리
        // T의 메모리 할당을 N번 받지 않고 바로 연속 메모리를 할당해 T를 넣게함.
        // T의 N번 할당 < T * N 사이즈 할당 (효율 up)
        // + alignas로 T의 크기에 맞게 메모리 정렬
        alignas(T) std::byte buf_[N * sizeof(T)];
        std::size_t size_ = 0;
        std::size_t capacity_ = N;

        // byte -> const T* 헬퍼함수
        T* data() { return data_; }
        const T* data() const { return data_; }
        
        // data_ == reinterpret_cast<T*>(buf_) => inline 상태
        // data_ != reinterpret_cast<T*>(buf_) => heap 상태 
        T* data_ = reinterpret_cast<T*>(buf_);  // 현재 쓰는 저장소
        
        // buf_의 위치와 현재 data_의 위치가 다르면 heap으로 이전했다는 의미
        bool is_small() const noexcept {
            return data_ == reinterpret_cast<const T*>(buf_);
        }

        void grow() {
            // 1. 새 용량 결정 (2배씩 확장)
            std::size_t new_cap = capacity_ * 2;

            // 2. 새 heap 메모리 확보
            T* new_data = static_cast<T*>(
                ::operator new(new_cap * sizeof(T), std::align_val_t{alignof(T)})
            );

            // 3. 기존 원소들 새 자리로 이사
            for (std::size_t i=0; i < size_; ++i) {
                new (new_data + i) T(std::move(data_[i]));
                data_[i].~T();
            }

            // 4. 기존 저장소가 heap이었으면 해제 (inline이면 해제 금지)
            if (!is_small()) {
                ::operator delete(data_, std::align_val_t{alignof(T)});
            }

            // 5. 새 저장소 활성화
            data_ = new_data;
            capacity_ = new_cap;
        }
    };
}
