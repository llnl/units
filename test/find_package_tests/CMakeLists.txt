# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Copyright (c) 2017-2025, Battelle Memorial Institute; Lawrence Livermore
# National Security, LLC; Alliance for Sustainable Energy, LLC.
# See the top-level NOTICE for additional details.
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

cmake_minimum_required(VERSION 3.10...3.31)

project(units-find-package-test)

include(CTest)

if(NOT UNITS_TARGET)
    set(UNITS_TARGET units)
endif()

if(NOT DEFINED UNITS_BUILD_CXX_MODULE)
    set(UNITS_BUILD_CXX_MODULE OFF)
endif()

# Test the HELICS CMake package config
find_package(${UNITS_TARGET} 0.9 REQUIRED)

# add some tests inside of the CMake

message(STATUS "Binary location is ${PROJECT_BINARY_DIR}")
# Test the CXX shared library target
if(UNITS_BUILD_SHARED_LIBRARY)
    add_executable(shared-library-test-exe ../pkg_test_code/main.cpp)
    target_link_libraries(shared-library-test-exe ${UNITS_TARGET}::units)
    target_compile_definitions(
        shared-library-test-exe PUBLIC UNITS_LIB_HEADER="${UNITS_TARGET}/units.hpp"
    )

    add_test(NAME shared-library-test COMMAND shared-library-test-exe)
    set_property(TEST shared-library-test PROPERTY PASS_REGULAR_EXPRESSION "10.7")

endif()
# Test the header only targets
add_executable(header_only-exe ../pkg_test_code/header_only.cpp)
target_link_libraries(header_only-exe ${UNITS_TARGET}::header_only)
target_compile_definitions(
    header_only-exe PUBLIC UNITS_LIB_HEADER="${UNITS_TARGET}/units.hpp"
)
add_test(NAME header_only-exe COMMAND header_only-exe)
set_property(TEST header_only-exe PROPERTY PASS_REGULAR_EXPRESSION "PASS")

if(UNITS_BUILD_CXX_MODULE AND TARGET ${UNITS_TARGET}::module)
    add_executable(module-test-exe ../pkg_test_code/module.cpp)
    target_link_libraries(module-test-exe ${UNITS_TARGET}::module)
    target_compile_features(module-test-exe PRIVATE cxx_std_20)

    add_test(NAME module-test-exe COMMAND module-test-exe)
    set_property(TEST module-test-exe PROPERTY PASS_REGULAR_EXPRESSION "PASS")
endif()
