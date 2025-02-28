set(CMAKE_CXX_STANDARD 20)
cmake_minimum_required(VERSION 3.14)
project(BigDecimal)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.8.0 # Замени на актуальную версию, если нужно
)
FetchContent_MakeAvailable(Catch2)

# Основной исполняемый файл
add_executable(BigDecimal main.cpp src/BigDecimal.cpp src/operations.cpp)


# Тесты
add_executable(BigDecimalTests tests/tests_BigDecimal.cpp
        tests/tests_BigDecimal.cpp)
target_include_directories(BigDecimalTests PRIVATE src)
target_link_libraries(BigDecimalTests PRIVATE Catch2::Catch2WithMain)

include(CTest)
enable_testing()
add_test(NAME BigDecimalTests COMMAND tests)