cmake_minimum_required(VERSION 3.0.0)
project(recs_tests VERSION 0.1.0)

SET(SOURCE
   "../src/pool.cpp"
   "../src/registry.cpp"
)

add_executable(recs_tests ${SOURCE} "test.cpp")

target_include_directories(
   recs_tests PUBLIC
   "../include"
)

set_target_properties(
   recs_tests PROPERTIES
   CXX_STANDARD 23
   CXX_STANDARD_REQUIRED ON
)