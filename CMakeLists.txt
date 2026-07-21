cmake_minimum_required(VERSION 3.15)
project(shooting LANGUAGES C CXX)

add_subdirectory(deps/XPFW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_DEBUG_POSTFIX d)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
			GIT_TAG f091b2c061b307ee89b164c39976fc9202a1c79d
)
FetchContent_MakeAvailable(cpr)

file(GLOB src src/*.cpp src/XASM2/*.cpp)
add_executable(shooting ${src})
target_link_libraries(shooting PUBLIC XPFW cpr)
target_include_directories(shooting PUBLIC include/)
set_target_properties(shooting PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

file(GLOB src_xasm2 xasm2/*.cpp)
add_executable(xasm2c ${src_xasm2})
target_include_directories(xasm2c PUBLIC include/XASM2/ include/)
set_target_properties(xasm2c PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
