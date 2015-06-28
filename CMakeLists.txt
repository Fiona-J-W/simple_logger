project(Simple-Logger)

# to shut up cmake, I have no clue what version this needs:
cmake_minimum_required(VERSION 2.8)


# create sane environment
#####################################################################

# Sadly, this doesn't seem to work:
#set_property(GLOBAL PROPERTY CXX_STANDARD 11)
#set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

# Ok, so let's just hardcode all the flags instead:
# TODO: make this portable
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11" )

# we want tests:
enable_testing()


# options
#####################################################################
Option(BUILD_TESTS "build-tests" ON)


# required packages
#####################################################################

if(BUILD_TESTS)
	find_package(Boost COMPONENTS unit_test_framework REQUIRED)
endif()


# targets
#####################################################################

include_directories("include")

file(GLOB library_headers "src/*.hpp")
file(GLOB library_code_files "src/*.cpp")
file(GLOB test_file "test/main.cpp")


add_library(simple_logger ${library_headers} ${library_code_files})

if(BUILD_TESTS)
	add_executable(simple_logger_tests ${test_file})
	target_link_libraries(simple_logger_tests
		simple_logger
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	)
	add_test(
		NAME "logger_tests"
		COMMAND simple_logger_tests
	)
endif()
