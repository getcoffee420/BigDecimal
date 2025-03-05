cmake_minimum_required(VERSION 3.10)

project(BigDecimalTest)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(googletest)

include_directories(src)
enable_testing()

set(SOURCES
        src/BigDecimal.cpp
        src/operations.cpp
        tests/tests_BigDecimal.cpp
)

add_library(BigDecimal STATIC src/BigDecimal.cpp src/operations.cpp)

add_executable(BigDecimalTests ${SOURCES})

target_link_libraries(BigDecimalTests gtest gtest_main pthread)

add_test(NAME BigDecimalTests COMMAND BigDecimalTests)