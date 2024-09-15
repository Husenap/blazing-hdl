#pragma once

#include <memory>

#include "blazing/frontend/ast/Stmt.hpp"

namespace blazing {
class SourceModule {
public:
  void visit(StmtVisitor& visitor) {
    statement->visit(visitor);
  }
  void fill(std::vector<std::string> imports, std::unique_ptr<Stmt> stmt) {
    importPaths = std::move(imports);
    statement   = std::move(stmt);
  }

  const std::vector<std::string>& getImportPaths() const {
    return importPaths;
  }

  std::unique_ptr<Stmt> getStatement() {
    return std::move(statement);
  }

private:
  std::vector<std::string> importPaths;
  std::unique_ptr<Stmt>    statement;
};
} // namespace blazing
