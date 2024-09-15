#pragma once

#include <iostream>

#include "blazing/frontend/ast/Expr.hpp"
#include "blazing/frontend/ast/Stmt.hpp"
namespace blazing::codegen::verilog {

class Transpiler : public StmtVisitor, public ExprVisitor {
  std::ostream& out;

  void print_indentation() {
    for (int i = 0; i < depth; ++i) {
      out << indentation;
    }
  }
  void indent() {
    ++depth;
  };
  void dedent() {
    --depth;
  };

  std::string_view indentation = "  ";
  int              depth       = 0;

public:
  Transpiler(std::ostream& out = std::cout) : out(out) {}

  void visit(ExprStmt& stmt) override {
    stmt.expr->visit(*this);
  }
  void visit(ModStmt& stmt) override {
    out << "module " << stmt.id << "(";
    for (int i = 0; i < stmt.ports.size(); ++i) {
      if (i > 0) out << ", ";
      stmt.ports[i]->visit(*this);
    }
    out << ");" << std::endl;

    indent();
    stmt.body->visit(*this);
    dedent();

    out << "endmodule" << std::endl << std::endl;
  }
  void visit(InterfaceStmt& stmt) override {}
  void visit(PortStmt& stmt) override {
    out << PortDirectionString[static_cast<int>(stmt.dir)] << ' ' << stmt.type << ' ' << stmt.id;
  }

  void visit(AssignmentExpr& expr) override {
    print_indentation();
    out << "assign " << expr.id << " = ";
    expr.value->visit(*this);
    out << ";" << std::endl;
  }
  void visit(BinOpExpr& expr) override {
    expr.lhs->visit(*this);
    out << ' ' << expr.op << ' ';
    expr.rhs->visit(*this);
  }
  void visit(NumberExpr&) override {}
  void visit(StringExpr&) override {}
  void visit(IdUseExpr& expr) override {
    out << expr.id;
  }
};
} // namespace blazing::codegen::verilog
