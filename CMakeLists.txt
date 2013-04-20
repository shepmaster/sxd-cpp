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

set(
  sxd_source
  qname.cpp
  document.cpp
  element.cpp
  attribute.cpp
  nmtoken.cpp
  node.cpp
  text-node.cpp
  root-node.cpp
  nodeset.cpp
  tokenizer.cpp
  comment.cpp
  xpath-factory.cpp
  xpath.cpp
  to-stream.cpp
  prefixed-name.cpp
  xpath-token.cpp
  xpath-tokenizer.cpp
  xpath-parser-token-source.cpp
  xpath-token-deabbreviator.cpp
  xpath-token-disambiguator.cpp
  axis-parent.cpp
  axis-child.cpp
  axis-self.cpp
  axis-descendant.cpp
  axis-descendant-or-self.cpp
  axis-attribute.cpp
  xpath-parser.cpp
  node-test-node.cpp
  node-test-element.cpp
  node-test-attribute.cpp
  node-test-text.cpp
  expression-literal.cpp
  expression-variable.cpp
  expression-function.cpp
  expression-step.cpp
  expression-predicate.cpp
  expression-path.cpp
  expression-context-node.cpp
  expression-root-node.cpp
  expression-math.cpp
  expression-negation.cpp
  expression-union.cpp
  expression-or.cpp
  expression-and.cpp
  expression-equal.cpp
  expression-relational.cpp
  xpath-evaluation-context-impl.cpp
  xpath-value.cpp
  xpath-value-boolean.cpp
  xpath-value-nodeset.cpp
  xpath-value-number.cpp
  xpath-value-string.cpp
  xpath-function-library.cpp
  xpath-variable-bindings-hash.cpp
  function-last.cpp
  function-position.cpp
  function-not.cpp
  function-true.cpp
  function-false.cpp
  xpath-core-function-library.cpp
)
add_library(sxd SHARED ${sxd_source})
target_link_libraries(sxd ${GLIB2_LIBRARIES})

add_definitions(-DGTEST_USE_OWN_TR1_TUPLE=1)
add_subdirectory(gmock-1.6.0)
include_directories(gmock-1.6.0/include gmock-1.6.0/gtest/include)

add_library(test-utilities SHARED test-utilities.cpp remembering-node-visitor.cpp)
target_link_libraries(test-utilities ${GLIB2_LIBRARIES} sxd)

set(test_programs
  qname
  element
  document
  nmtoken
  node
  text-node
  nodeset
  tokenizer
  comment
  xpath-acceptance
  xpath-tokenizer
  xpath-token-deabbreviator
  xpath-token-disambiguator
  xpath-parser
  node-test-attribute
  xpath-value
  xpath-value-boolean
  xpath-value-nodeset
  xpath-value-number
  xpath-value-string
  expression-variable
  expression-function
  expression-step
  expression-predicate
  expression-math
  expression-negation
  expression-union
  expression-or
  expression-and
  expression-equal
  expression-relational
  xpath-evaluation-context-impl
  xpath-function-library
  function-last
  function-position
  function-not
  xpath-core-function-library
)
foreach(program ${test_programs})
  add_executable(${program}-test ${program}-test.cpp)
  target_link_libraries(${program}-test sxd test-utilities gmock)
  add_test(${program} ${program}-test)
endforeach(program)

add_executable(util util.cpp)
target_link_libraries(util sxd)

find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    @ONLY)

  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)
endif(DOXYGEN_FOUND)
