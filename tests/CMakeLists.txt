cmake_minimum_required(VERSION 3.0.0)
project(RECS VERSION 0.1.0)

SET(SOURCE
   "../src/pool.cpp"
   "../src/registry.cpp"
   "../src/registry_meta.cpp"
)

add_executable(RECS ${SOURCE} "test.cpp")

target_include_directories(
   RECS PUBLIC
   "../include"
)

set_target_properties(
   RECS PROPERTIES
   CXX_STANDARD 17
   CXX_STANDARD_REQUIRED ON
)