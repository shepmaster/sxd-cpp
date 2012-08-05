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

set(sxd_source document.cpp element.cpp exception.cpp intern.cpp nmtoken.cpp node.cpp text-node.cpp nodeset.cpp xpath.cpp xpath-functions.cpp tokenizer.cpp comment.cpp)
add_library(sxd SHARED ${sxd_source})
target_link_libraries(sxd ${GLIB2_LIBRARIES})

add_library(test-utilities SHARED test-utilities.cpp)
target_link_libraries(test-utilities ${GLIB2_LIBRARIES} sxd)

set(test_programs element intern document exception nmtoken node text-node nodeset xpath xpath-functions tokenizer comment)
foreach(program ${test_programs})
  add_executable(${program}-test ${program}-test.cpp)
  target_link_libraries(${program}-test sxd test-utilities CppUTest)
  add_test(${program} ${program}-test)
endforeach(program)
