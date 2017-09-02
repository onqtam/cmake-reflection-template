cmake_minimum_required(VERSION 3.0.0)

project(reflected-project)

include(scripts/utils.cmake)

add_executable(example src/main.cpp src/foo.h src/common.h)

# reflect the target
target_parse_sources(example)

# need c++17 for any from the <any> header
if(MSVC)
    target_compile_options(example PUBLIC /std:c++latest)
else()
    target_compile_options(example PUBLIC -std=c++1z)
endif()
