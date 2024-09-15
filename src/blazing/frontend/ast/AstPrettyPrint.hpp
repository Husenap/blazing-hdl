#pragma once

#include <iostream>

#include "Expr.hpp"
#include "Stmt.hpp"

namespace blazing {

struct AstPrettyPrint {
  std::ostream& out;
  AstPrettyPrint(std::ostream& out = std::cout) : out(out) {}
};

} // namespace blazing
