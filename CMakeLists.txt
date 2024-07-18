cmake_minimum_required(VERSION 3.28)
SET(PROJECT_NAME DFT})
project(DFT)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES src/main.cpp)
add_executable(${PROJECT_NAME} src/main.cpp)


find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE "include" "lib")
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-audio sfml-network)