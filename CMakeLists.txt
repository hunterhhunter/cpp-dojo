cmake_minimum_required(VERSION 3.20)
project(cpp-dojo CXX)

# FetchContent URL 추출 시 최신 타임스탬프 사용 (CMP0135 경고 제거)
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)   # std=c++17, not gnu++17
# compile_commands.json을 생성해 외부 도구가 이 파일로 소스 이해
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── 외부 의존성 먼저: 우리 경고 플래그가 third-party 코드에 새지 않도록 ──
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
)
FetchContent_MakeAvailable(googletest)
enable_testing()

# ── 우리 프로젝트 전용 경고: 이 줄 이후 생성되는 타겟에만 적용 ──
add_compile_options(
    -Wall -Wextra -Wpedantic -Werror
    -Wshadow -Wnon-virtual-dtor -Wold-style-cast
    -Wcast-align -Wunused -Woverloaded-virtual
    -Wconversion -Wsign-conversion
    -Wnull-dereference -Wdouble-promotion -Wformat=2
)

# 런타임 검사 도구 사용
# ASan(Address Sanitizer / 메모리 오류 감지) (use-after-free, buffer overflow, memory leak 등)
# UBSan (Undefined Behavior Sanitizer / 정의되지 않은 동작 감지) (정수 오버플로, 널포인터 역참조, 형변환 에러)
option(DOJO_SANITIZE "Enable ASan+UBSan in Debug" ON)
if(DOJO_SANITIZE AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address,undefined)
endif()


add_subdirectory(toolbox)
add_subdirectory(dojo/A-my-llvm-adt)
# add_subdirectory(dojo/B-mini-rewrite_engine)
# add_subdirectory(dojo/C-tiny-ir-visitor)
