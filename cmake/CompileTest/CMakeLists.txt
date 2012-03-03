# Copyright Troy D. Straszheim
#
# Distributed under the Boost Software License, Version 1.0.
# See http://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 2.6)

project(CompileTest)

# Set up the basic build structure for the compile test.
include_directories(${INCLUDES})
add_library(compile-test STATIC ${SOURCE})

# Set additional compiler flags
set_source_files_properties(${SOURCE}
    PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS}")
