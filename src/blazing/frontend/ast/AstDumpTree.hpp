#pragma once

#include <iostream>

#include "Expr.hpp"
#include "Stmt.hpp"

namespace blazing {

struct AstDumpTree : public ExprVisitor, public StmtVisitor {
  std::ostream& out;
  AstDumpTree(std::ostream& out = std::cout) : out(out) {}

private:
  std::ostream& output(std::string_view name) {
    for (int i = 0; i < depth; ++i) {
      out << indentation;
    }
    out << name << ": ";
    return out;
  }
  void indent() {
    ++depth;
  };
  void dedent() {
    --depth;
  };

  std::string_view indentation = "  ";
  int              depth       = 0;
};

} // namespace blazing
