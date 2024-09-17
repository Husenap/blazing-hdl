#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Expr.hpp"

namespace blazing {

struct StmtList;
struct ExprStmt;
struct ModStmt;
struct InterfaceStmt;
struct PortStmt;

struct StmtVisitor {
  void         visit(StmtList& stmt);
  virtual void visit(ExprStmt& stmt) = 0;
  virtual void visit(ModStmt&)       = 0;
  virtual void visit(InterfaceStmt&) = 0;
  virtual void visit(PortStmt&)      = 0;
};

struct Stmt {
  virtual ~Stmt()                    = default;
  virtual void visit(StmtVisitor& v) = 0;
};

struct StmtList : Stmt {
  StmtList(std::vector<std::unique_ptr<Stmt>> stmts = {}) : stmts(std::move(stmts)) {}
  StmtList(std::unique_ptr<Stmt> stmt) {
    stmts.push_back(std::move(stmt));
  }
  std::vector<std::unique_ptr<Stmt>> stmts;

  void push_back(std::unique_ptr<Stmt>&& stmt) {
    stmts.push_back(std::move(stmt));
  }

  void visit(StmtVisitor& v) override {
    v.visit(*this);
  }
};
inline void StmtVisitor::visit(StmtList& stmt) {
  for (auto& param : stmt.stmts) {
    param->visit(*this);
  }
}

struct ExprStmt : Stmt {
  ExprStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}
  std::unique_ptr<Expr> expr;

  void visit(StmtVisitor& v) override {
    v.visit(*this);
  }
};

struct ModStmt : Stmt {
  ModStmt(std::string id, std::unique_ptr<Stmt> body, std::vector<std::unique_ptr<Stmt>> ports = {})
      : id(id), body(std::move(body)), ports(std::move(ports)) {}
  std::string                        id;
  std::unique_ptr<Stmt>              body;
  std::vector<std::unique_ptr<Stmt>> ports;

  void visit(StmtVisitor& v) override {
    v.visit(*this);
  }
};

struct InterfaceStmt : Stmt {
  InterfaceStmt(std::string id, std::vector<std::unique_ptr<Stmt>> ports = {}) : id(id), ports(std::move(ports)) {}
  std::string                        id;
  std::vector<std::unique_ptr<Stmt>> ports;

  void visit(StmtVisitor& v) override {
    v.visit(*this);
  }
};

enum class PortDirection { In, Out, None };
static constexpr const char* PortDirectionString[]{"input", "output", "undirected"};
struct PortStmt : Stmt {
  PortStmt(PortDirection dir, std::string type, std::string id) : dir(dir), type(type), id(id) {}
  PortDirection dir;
  std::string   type;
  std::string   id;

  void visit(StmtVisitor& v) override {
    v.visit(*this);
  }
};

} // namespace blazing
