cmake_minimum_required(VERSION 3.3)

# disable googlemock
option(BUILD_GMOCK "Builds the googlemock subproject" OFF)

# create string variable ${PROJECT_NAME}='set'
set(PROJECT_NAME set)
# the solution with the 'set' name
project(${PROJECT_NAME})

# set c++ language standard
set(CMAKE_CXX_STANDARD 17)

# create string variables for subproject names
set(MP2_LIBRARY "${PROJECT_NAME}")
set(MP2_TESTS   "test_${PROJECT_NAME}")
# create string variable for include path
set(MP2_INCLUDES "${PROJECT_SOURCE_DIR}/include")

# add googletest, version 1.18.0
add_subdirectory("${PROJECT_SOURCE_DIR}/gtest/googletest-1.18.0")
set(MP2_GTEST_INCLUDES "${gtest_SOURCE_DIR}/include/gtest")

# build subprojects
add_subdirectory("${PROJECT_SOURCE_DIR}/src")
add_subdirectory("${PROJECT_SOURCE_DIR}/samples")
add_subdirectory("${PROJECT_SOURCE_DIR}/test")

# report
message( STATUS "")
message( STATUS "General configuration for ${PROJECT_NAME}")
message( STATUS "======================================")
message( STATUS "")
message( STATUS "   Configuration: ${CMAKE_BUILD_TYPE}")
message( STATUS "")
