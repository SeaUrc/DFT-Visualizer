cmake_minimum_required(VERSION 3.28)
SET(PROJECT_NAME DFT)
project(DFT)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 404b3e71eab1a99910d10546d5384042177269ab)
FetchContent_MakeAvailable(SFML)

add_subdirectory(external/tinyxml2)

set(SOURCE_FILES src/main.cpp)
add_executable(${PROJECT_NAME} src/main.cpp)

#find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE "include" "lib")

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-system sfml-window sfml-network sfml-audio)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)