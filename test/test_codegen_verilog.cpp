#include <fstream>

#include <gtest/gtest.h>

#include "Parser.hpp"
#include "Scanner.hpp"
#include "blazing/frontend/codegen/verilog/Transpiler.hpp"
#include "gtest/gtest.h"
#include "util.hpp"

class TestCodegenVerilog : public ::testing::TestWithParam<std::filesystem::path> {};

TEST_P(TestCodegenVerilog, TestFileProcessing) {
  auto in_filepath  = GetParam();
  auto out_filepath = in_filepath;
  out_filepath.replace_extension("v");
  auto expected_filepath = in_filepath;
  expected_filepath.replace_extension("expected");

  std::ifstream in_file(in_filepath);
  std::ofstream out_file(out_filepath);

  blazing::LexContext   ctx;
  blazing::SourceModule sm;
  blazing::Scanner      scanner{in_file, out_file};
  blazing::Parser       parser{scanner, ctx, sm};

  ASSERT_EQ(parser.parse(), 0);

  blazing::codegen::verilog::Transpiler transpiler(out_file);
  sm.visit(transpiler);

  ASSERT_EQ(ReadFile(out_filepath), ReadFile(expected_filepath));
}

INSTANTIATE_TEST_SUITE_P(VerilogFiles,
                         TestCodegenVerilog,
                         ::testing::ValuesIn(GetTestFiles("testfiles/codegen/verilog", ".in")));
