cmake_minimum_required(VERSION 3.15)
project(shooting LANGUAGES C CXX)

add_subdirectory(deps/XPFW)

file(GLOB src src/*.cpp src/XASM2/*.cpp)
add_executable(shooting ${src})
target_link_libraries(shooting XPFW)
target_include_directories(shooting PUBLIC include/)
set_target_properties(shooting PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

file(GLOB src_xasm2 xasm2/*.cpp)
add_executable(xasm2c ${src_xasm2})
target_include_directories(xasm2c PUBLIC include/XASM2/)
set_target_properties(xasm2c PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
