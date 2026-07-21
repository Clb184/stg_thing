cmake_minimum_required(VERSION 3.15)
project(shooting LANGUAGES C CXX)

add_subdirectory(deps/XPFW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_DEBUG_POSTFIX d)

file(GLOB src src/*.cpp src/XASM2/*.cpp)
add_executable(shooting ${src})
add_subdirectory(deps/cpp-httplib)
set(CURL_USE_LIBPSL OFF)
add_subdirectory(deps/curl)
target_link_libraries(shooting PUBLIC XPFW httplib libcurl)
target_include_directories(shooting PUBLIC include/)
set_target_properties(shooting PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

file(GLOB src_xasm2 xasm2/*.cpp)
add_executable(xasm2c ${src_xasm2})
target_include_directories(xasm2c PUBLIC include/XASM2/ include/)
set_target_properties(xasm2c PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
