#pragma once

#include <memory>
#include <string>

namespace blazing {

struct AssignmentExpr;
struct BinOpExpr;
struct NumberExpr;
struct StringExpr;
struct IdUseExpr;

struct ExprVisitor {
  virtual void visit(AssignmentExpr&) = 0;
  virtual void visit(BinOpExpr&)      = 0;
  virtual void visit(NumberExpr&)     = 0;
  virtual void visit(StringExpr&)     = 0;
  virtual void visit(IdUseExpr&)      = 0;
};

struct Expr {
  virtual ~Expr()                    = default;
  virtual void visit(ExprVisitor& v) = 0;
};

struct AssignmentExpr : Expr {
  AssignmentExpr(std::string id, std::unique_ptr<Expr> value) : id(id), value(std::move(value)) {}
  std::string           id;
  std::unique_ptr<Expr> value;

  void visit(ExprVisitor& v) override {
    v.visit(*this);
  }
};

struct BinOpExpr : Expr {
  BinOpExpr(std::string op, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs)
      : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
  std::string           op;
  std::unique_ptr<Expr> lhs;
  std::unique_ptr<Expr> rhs;

  void visit(ExprVisitor& v) override {
    v.visit(*this);
  }
};

struct NumberExpr : Expr {
  NumberExpr(double value) : value(value) {}
  double value{};

  void visit(ExprVisitor& v) override {
    v.visit(*this);
  }
};

struct StringExpr : Expr {
  StringExpr(std::string value) : value(value) {}
  std::string value{};

  void visit(ExprVisitor& v) override {
    v.visit(*this);
  }
};

struct IdUseExpr : Expr {
  IdUseExpr(std::string id) : id(id) {}
  std::string id{};

  void visit(ExprVisitor& v) override {
    v.visit(*this);
  }
};

} // namespace blazing
