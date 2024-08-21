cmake_minimum_required(VERSION 3.16)
project(resc VERSION 0.0.0)

set(SOURCES
   "src/pool.cpp"
   "src/registry.cpp"
)

add_library(resc STATIC ${SOURCES})

target_include_directories(
   resc PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(
   resc PROPERTIES
   CXX_STANDARD 23
   CXX_STANDARD_REQUIRED ON
)


set(RECS_TESTS TRUE)
if(RECS_TESTS)
	add_subdirectory(tests)
endif()