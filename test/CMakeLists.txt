file(
  GLOB
  TEST_FILES
  "testfiles/ast/*.in"
  "testfiles/ast/*.expected"
  "testfiles/interpreter/*.in"
  "testfiles/interpreter/*.expected"
  "testfiles/parser/*.valid"
  "testfiles/parser/*.error")

add_custom_target(blazing_test_testfiles ALL DEPENDS ${TEST_FILES})

add_executable(blazing_test "test_parser.cpp" "test_ast.cpp")

add_dependencies(blazing_test blazing_test_testfiles)

target_link_libraries(blazing_test PUBLIC blazing_parser GTest::gtest_main)
target_include_directories(blazing_test PUBLIC ${CMAKE_SOURCE_DIR}/src)

gtest_discover_tests(blazing_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
