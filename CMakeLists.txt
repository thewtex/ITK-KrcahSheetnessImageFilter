cmake_minimum_required(VERSION 2.8)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0053 NEW)
endif(COMMAND cmake_policy)

set (CMAKE_CXX_STANDARD 14)

# Turn on with 'cmake -Dmyvarname=ON'.
option(test "Build all tests." OFF) # Makes boolean 'test' available.

# ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

# Include this project
include_directories(include)

option(BUILD_EXAMPLES "Build examples." ON)
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

################################
# gtest
################################
if (test)
  find_package(GTest REQUIRED)
  enable_testing()
  include_directories(${GTEST_INCLUDE_DIRS})

  add_subdirectory(test)
endif()
