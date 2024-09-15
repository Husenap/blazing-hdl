#include <fstream>

#include <gtest/gtest.h>

#include "Parser.hpp"
#include "Scanner.hpp"
#include "gtest/gtest.h"
#include "util.hpp"

template <bool should_fail>
void test_result(std::filesystem::path filepath) {
  std::ifstream f(filepath);

  blazing::LexContext   ctx;
  blazing::SourceModule sm;
  blazing::Scanner      scanner{f, std::cout};
  blazing::Parser       parser{scanner, ctx, sm};

  if constexpr (should_fail) {
    EXPECT_THROW(parser.parse(), blazing::SyntaxError);
  } else {
    EXPECT_EQ(parser.parse(), 0);
  }
}

class TestValidSyntax : public ::testing::TestWithParam<std::filesystem::path> {};

TEST_P(TestValidSyntax, TestFileProcessing) {
  test_result<false>(GetParam());
}

INSTANTIATE_TEST_SUITE_P(ValidFiles, TestValidSyntax, ::testing::ValuesIn(GetTestFiles("testfiles/parser", ".valid")));

class TestSyntaxError : public ::testing::TestWithParam<std::filesystem::path> {};

TEST_P(TestSyntaxError, TestFileProcessing) {
  test_result<true>(GetParam());
}

INSTANTIATE_TEST_SUITE_P(ErrorFiles, TestSyntaxError, ::testing::ValuesIn(GetTestFiles("testfiles/parser", ".error")));
