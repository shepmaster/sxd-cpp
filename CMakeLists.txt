cmake_minimum_required (VERSION 2.6) 
project (SXD)
enable_testing()

## External Dependencies

# GLib
find_package(PkgConfig)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)

include_directories(${GLIB2_INCLUDE_DIRS})
link_directories(${GLIB2_LIBRARY_DIRS})
add_definitions(${GLIB2_CFLAGS})

## Build rules

add_definitions(-ansi -Wall -pedantic -Werror -std=c++11 -stdlib=libc++)


set(sxd_source document.cpp element.cpp exception.cpp intern.cpp nmtoken.cpp node.cpp text-node.cpp nodeset.cpp tokenizer.cpp comment.cpp xpath-factory.cpp xpath.cpp to-stream.cpp xpath-token.cpp xpath-tokenizer.cpp axis-parent.cpp axis-child.cpp axis-self.cpp xpath-parser.cpp node-test-element.cpp xpath-step.cpp)
add_library(sxd SHARED ${sxd_source})
target_link_libraries(sxd ${GLIB2_LIBRARIES})

add_definitions(-DGTEST_USE_OWN_TR1_TUPLE=1)
add_subdirectory(gmock-1.6.0)
include_directories(gmock-1.6.0/include gmock-1.6.0/gtest/include)

add_library(test-utilities SHARED test-utilities.cpp remembering-node-visitor.cpp)
target_link_libraries(test-utilities ${GLIB2_LIBRARIES} sxd)

set(test_programs element intern document exception nmtoken node text-node nodeset tokenizer comment xpath-acceptance xpath-tokenizer xpath-parser)
foreach(program ${test_programs})
  add_executable(${program}-test ${program}-test.cpp)
  target_link_libraries(${program}-test sxd test-utilities CppUTest gmock)
  add_test(${program} ${program}-test)
endforeach(program)

add_executable(util util.cpp)
target_link_libraries(util sxd)
