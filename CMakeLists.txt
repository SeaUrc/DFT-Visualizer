cmake_minimum_required(VERSION 3.28)
SET(PROJECT_NAME DFT)
project(DFT)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1)
FetchContent_MakeAvailable(SFML)

add_subdirectory(external/nativefiledialog-extended)

set(SOURCE_FILES src/main.cpp)
set_source_files_properties(${CMAKE_SOURCE_DIR}/Info.plist PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

add_executable(${PROJECT_NAME} MACOSX_BUNDLE src/main.cpp ${CMAKE_SOURCE_DIR}/Info.plist)

#find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE "include" "lib")

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-system sfml-window sfml-network sfml-audio nfd)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set(MACOSX_BUNDLE_BUNDLE_NAME "DFT")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.test.DFT")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0")
set(MACOSX_BUNDLE_BUNDLE_VERSION "1")
set(MACOSX_BUNDLE_COPYRIGHT "DFT")

# Ensure the Info.plist is copied into the bundle
set_target_properties(DFT PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Info.plist
)