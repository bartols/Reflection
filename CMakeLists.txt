cmake_minimum_required(VERSION 3.13)
project(Reflection)

# add_definitions(-DGTEST_HAS_PTHREAD=0)
option(CMAKE_USE_WIN32_THREADS_INIT "using WIN32 threads" ON)
option(gtest_disable_pthreads "Disable uses of pthreads in gtest." ON)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(external)
add_subdirectory(src)

enable_testing()
add_subdirectory(test)

