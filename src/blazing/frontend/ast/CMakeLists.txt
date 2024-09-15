add_library(blazing_ast STATIC "AstDumpTree.hpp" "Expr.hpp" "Stmt.hpp")

set_target_properties(blazing_ast PROPERTIES LINKER_LANGUAGE CXX)
