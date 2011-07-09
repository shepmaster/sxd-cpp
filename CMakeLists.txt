cmake_minimum_required (VERSION 2.6) 
project (SXD)
enable_testing()

include_directories(/usr/local/include /usr/local/include/glib-2.0 /usr/local/lib/glib-2.0/include)

add_definitions(-ansi -Wall -pedantic -Werror)

set(sxd_source document.c element.c exception.c intern.c nmtoken.c node.c text-node.c nodeset.c xpath.c)
add_library(sxd SHARED ${sxd_source})
target_link_libraries(sxd glib-2.0 intl)

add_library(test-utilities SHARED test-utilities.c)
target_link_libraries(test-utilities glib-2.0 intl sxd)

set(test_programs element intern document exception nmtoken node text-node nodeset xpath)
foreach(program ${test_programs})
  add_executable(${program}-test ${program}-test.cpp)
  target_link_libraries(${program}-test sxd test-utilities cpputest)
  add_test(${program} ${program}-test)
endforeach(program)