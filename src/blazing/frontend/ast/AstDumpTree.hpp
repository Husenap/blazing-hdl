#pragma once

#include <iostream>

#include "Expr.hpp"
#include "Stmt.hpp"

namespace blazing {

class AstDumpTree : public StmtVisitor, public ExprVisitor {
  std::ostream& out;

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

public:
  AstDumpTree(std::ostream& out = std::cout) : out(out) {}

  void visit(ExprStmt& stmt) override {
    stmt.expr->visit(*this);
  }
  void visit(ModStmt& stmt) override {
    output("ModStmt") << stmt.id << std::endl;
    if (!stmt.ports.empty()) {
      indent();

      output("Ports") << std::endl;
      indent();
      for (auto& port : stmt.ports) {
        port->visit(*this);
      }
      dedent();

      output("Body") << std::endl;
      indent();
      stmt.body->visit(*this);
      dedent();

      dedent();
    }
  }
  void visit(InterfaceStmt& stmt) override {
    output("InterfaceStmt") << stmt.id << std::endl;
    if (!stmt.ports.empty()) {
      indent();
      output("Ports") << std::endl;
      indent();
      for (auto& port : stmt.ports) {
        port->visit(*this);
      }
      dedent();
      dedent();
    }
  }
  void visit(PortStmt& stmt) override {
    output("PortStmt") << PortDirectionString[static_cast<size_t>(stmt.dir)] << ' ' << stmt.type << ' ' << stmt.id
                       << std::endl;
  }

  void visit(AssignmentExpr& expr) override {
    output("AssignmentExpr") << expr.id << std::endl;
    indent();

    expr.value->visit(*this);

    dedent();
  }
  void visit(BinOpExpr& expr) override {
    output("BinOpExpr") << expr.op << std::endl;
    indent();
    expr.lhs->visit(*this);
    expr.rhs->visit(*this);
    dedent();
  }
  void visit(NumberExpr&) override {
    output("NumberExpr") << std::endl;
  }
  void visit(StringExpr&) override {
    output("StringExpr") << std::endl;
  }
  void visit(IdUseExpr& expr) override {
    output("IdUseExpr") << expr.id << std::endl;
  }
};

} // namespace blazing
