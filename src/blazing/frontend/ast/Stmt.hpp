#pragma once

#include <string>
#include <vector>

#include "Expr.hpp"

namespace blazing {

struct StmtVisitor {};

struct Stmt {
  virtual ~Stmt()                    = default;
  virtual void visit(StmtVisitor& v) = 0;
};

} // namespace blazing
