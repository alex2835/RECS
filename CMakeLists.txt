cmake_minimum_required(VERSION 3.0.0)
project(RECS VERSION 0.1.0)

include(CTest)
enable_testing()

SET(SOURCE
   "src/view.cpp"
   "src/pool.cpp"
   "src/entity.cpp"
   "src/registry.cpp"
   "src/registry_meta.cpp"
   "main.cpp"
)

#add_library(RECS STATIC ${SOURCE})
add_executable(RECS ${SOURCE})


target_include_directories(
   RECS PRIVATE
   "include"
)

set_target_properties(
   RECS PROPERTIES
   CXX_STANDARD 17
   CXX_STANDARD_REQUIRED ON
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
