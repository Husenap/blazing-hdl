#pragma once

#include <memory>
#include <string>
#include <vector>

namespace blazing {

struct ExprVisitor {};

struct Expr {
  virtual ~Expr()                    = default;
  virtual void visit(ExprVisitor& v) = 0;
};

} // namespace blazing
