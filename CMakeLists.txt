cmake_minimum_required(VERSION 3.0.0)

project(reflected-project)

include(scripts/utils.cmake)

add_executable(example src/main.cpp src/foo.h src/common.h)

# reflect the target
target_parse_sources(example)

# need c++17 for std::any from the <any> header
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(example PUBLIC -std=c++1z) # -stdlib=libc++
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(example PUBLIC -std=c++1z)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(example PUBLIC /std:c++latest)
endif()
