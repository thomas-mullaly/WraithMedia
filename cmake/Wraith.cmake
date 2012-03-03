# Turn testing features.
# TODO: This is really annoying because the test target doesn't actually seem
# to be hookable. I either need a custom version of enable_testing and add_test
# or I need to figure out how to modify the output of the actual test
# generation... Should be possible? Yes since we can generate files for
# CTest. It should work really well.
enable_testing()

option(WRAITH_ENABLE_REGRESSION_TESTS "Enable regression testing" ON)

if(NOT WRAITH_INCLUDED)
    set(WRAITH_INCLUDED TRUE)

    # The WRAITH_CMAKE_DIR is the path to the directory containing the Wraith
    # CMake build extensions.
    get_filename_component(WRAITH_CMAKE_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
    
    include(BoostUtils)    
endif()

# Install a new target, "tests" that may or may not be hooked into the "all"
# target, depending on the status of the WRAITH_BUILD_TESTS option.
# add_custom_target(tests COMMENT "Building regression tests...")
if(WRAITH_ENABLE_REGRESSION_TESTS)
    add_custom_target(tests ALL COMMENT "Built regression tests...")
    # set_target_properties(tests PROPERTIES EXCLUDE_FROM_ALL FALSE)
else()
    add_custom_target(tests COMMENT "Done building regression tests...")
endif()

# Run a test, ensuring that it passes (returns 0).
#    wraith_test_run(name src...
#                    [ARGS args...])
#
# This is very much a watered down version of Boost's run test.
macro(wraith_test_run name)
    set(exename ${name})
    set(testname "${name}-test")
    parse_arguments(parsed "ARGS;OPTIONS" "" ${ARGN})

    # Source files from the command.
    set(src ${parsed_DEFAULT_ARGS})

    # TODO: Evaluate known parsed arguments. For now, we don't actually accept
    # any, but we could quite easily.
    if(parsed_ARGS)
        message(FATAL_ERROR "Argument passing for tests not implemented")
    endif()

    # Generate an executable program, but exclude it from the all target, but
    # make it depend on the test for the current project.
    # TODO: Append known compiler flags here...
    add_executable(${exename} ${src})
    target_link_libraries(${exename} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} wraithcore wraithgui)
    #add_dependencies(${ORIGIN_CURRENT_PROJECT}-test ${exename})

    # Generate a test for the executable.
    # TODO: Append test arguments here
    add_test(${testname} ${CMAKE_CURRENT_BINARY_DIR}/${exename})
endmacro(wraith_test_run)

# Build a test suite for a compilation test. This actually generates an
# entirely new project and runs ctest on it.
#
#    wraith_run_compile(name src...)
#
# This is taken nearly verbatim from the Boost.CMake effort.
macro(wraith_test_compile name)
    # Set up some test name information
    set(testname ${name}-test)

    # Parse the arguments and validate parameters.
    parse_arguments(parsed "" "" ${ARGN})
    set(src ${parsed_DEFAULT_ARGS})

    # Get any include directories
    get_directory_property(inc INCLUDE_DIRECTORIES)

    # Setup compilation flags... Minimally we need ORIGIN_CXX0X_FLAGS
    #set(flags "${ORIGIN_CXX0X_FLAGS}")

    # This test will basically instantiate the CompileTest template into the
    # current binary directory under the given test name.
    add_test(${testname}
      ${CMAKE_CTEST_COMMAND}
      --build-and-test
      ${WRAITH_CMAKE_DIR}/CompileTest
      ${CMAKE_CURRENT_BINARY_DIR}/${name}
      --build-generator ${CMAKE_GENERATOR}
      --build-makeprogram ${CMAKE_MAKE_PROGRAM}
      --build-project CompileTest
      --build-options
      "-DSOURCE:STRING=${CMAKE_CURRENT_SOURCE_DIR}/${src}"
      "-DINCLUDES:STRING=${inc}"
      "-DCOMPILE_FLAGS:STRING=${flags}"
      )

#     set_tests_properties(${BOOST_TEST_TESTNAME}
#       PROPERTIES
#       LABELS "${PROJECT_NAME}"
#       )
#
#     boost_test_known_failures(${testname} ${BOOST_TEST_KNOWN_FAILURES})
#
#     if (BOOST_TEST_FAIL)
#       set_tests_properties(${BOOST_TEST_TESTNAME} PROPERTIES WILL_FAIL ON)
#     endif ()
#   endif(BOOST_TEST_OKAY)
endmacro(wraith_test_compile)

