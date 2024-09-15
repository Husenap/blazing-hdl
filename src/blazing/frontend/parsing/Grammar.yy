%skeleton "lalr1.cc"
%require "3.8.2"
%header
%language "C++"

%defines "Parser.hpp"
%output "Parser.cpp"

%locations
%define api.location.file "SourceLoc.hpp"
 
%define api.namespace {blazing}
%define api.parser.class {Parser}

%define api.value.type variant
%define api.token.constructor
%define api.token.raw

%define parse.trace
%define parse.assert
%define parse.error verbose
%define parse.lac full

// =================================== Require code ======================================

%code requires{
#include "LexContext.hpp"
#include "SourceModule.hpp"
#include "SyntaxError.hpp"
#include "blazing/frontend/ast/Expr.hpp"
#include "blazing/frontend/ast/Stmt.hpp"
#include "blazing/frontend/ast/Utils.hpp"

namespace blazing{
  class Scanner;
}
}
 
// This is a parameter to the parser constructor
%parse-param {Scanner &scanner}

// These are parameters to the yylex function
%param { LexContext &ctx }
%param { SourceModule &sm }


%code
{
#include "Scanner.hpp"
#define yylex(ctx, sm) scanner.lex(ctx, sm)
void blazing::Parser::error(const location &loc, const std::string &error) {
  throw blazing::SyntaxError(loc, error);
}
}


// =================================== Token Definitions =================================
 
%token END 0
%token IMPORT MODULE INTERFACE
%token LPAREN RPAREN LBRACE RBRACE COMMA SEMICOLON
%token EQUAL XOR AND
%token IN OUT COLON
%token TYPE IDENTIFIER NUMBER BOOLEAN STRING

%type <std::vector<std::string>> import_list;
%type <std::unique_ptr<StmtList>> declaration_list;
%type <std::unique_ptr<Stmt>> declaration;
%type <std::unique_ptr<Stmt>> module_declaration;
%type <std::unique_ptr<Stmt>> interface_declaration;
%type <std::vector<std::unique_ptr<Stmt>>> port_list;
%type <std::unique_ptr<Stmt>> port;
%type <PortDirection> port_direction;
%type <std::unique_ptr<StmtList>> statement_list;
%type <std::unique_ptr<Stmt>> statement;
%type <std::unique_ptr<Expr>> expression;
%type <std::unique_ptr<Expr>> assignment_expression;
%type <std::unique_ptr<Expr>> exclusive_or_expression;
%type <std::unique_ptr<Expr>> and_expression;
%type <std::unique_ptr<Expr>> primary_expression;

%type <int64_t> NUMBER;
%type <std::string> IDENTIFIER;
%type <std::string> STRING;


// =================================== Grammar Definition ================================
 
%%

%start script;

script
  : import_list declaration_list END { sm.fill(std::move($1), std::move($2)); }
  ;

import_list
  : %empty {}
  | import_list[list] IMPORT STRING[script] { $list.push_back(escapeString($script)); $$ = std::move($list); }
  ;

declaration_list
  : %empty { $$ = std::make_unique<StmtList>(); }
  | declaration_list[list] declaration[decl] { $list->push_back(std::move($decl)); $$ = std::move($list); }
  ;
declaration
  : module_declaration { $$ = std::move($1); }
  | interface_declaration { $$ = std::move($1); }
  ;

module_declaration
  : MODULE IDENTIFIER[id] LPAREN port_list[ports] RPAREN LBRACE statement_list[body] RBRACE { $$ = std::make_unique<ModStmt>($id, std::move($body), std::move($ports));}
  | MODULE IDENTIFIER[id] LPAREN RPAREN LBRACE statement_list[body] RBRACE { $$ = std::make_unique<ModStmt>($id, std::move($body));}
  ;

interface_declaration
  : INTERFACE IDENTIFIER[id] LBRACE RBRACE { $$ = std::make_unique<InterfaceStmt>($id); }
  | INTERFACE IDENTIFIER[id] LBRACE port_list[ports] RBRACE { $$ = std::make_unique<InterfaceStmt>($id, std::move($ports)); }
  | INTERFACE IDENTIFIER[id] LPAREN port_direction RPAREN LBRACE RBRACE { $$ = std::make_unique<InterfaceStmt>($id); }
  | INTERFACE IDENTIFIER[id] LPAREN port_direction RPAREN LBRACE port_list[ports] RBRACE { $$ = std::make_unique<InterfaceStmt>($id, std::move($ports)); }
  ;

port_list
  : port { $$.push_back(std::move($1)); }
  | port_list port { $1.push_back(std::move($2)); $$ = std::move($1); }
  ;
port
  : IDENTIFIER[id] port_direction[dir] IDENTIFIER[type] { $$ = std::make_unique<PortStmt>($dir, $type, $id); }
  ;
port_direction
  : COLON { $$ = PortDirection::None; }
  | IN { $$ = PortDirection::In; }
  | OUT { $$ = PortDirection::Out; }
  ;

statement_list
  : %empty { $$ = std::make_unique<StmtList>(); }
  | statement_list[list] statement[decl] { $list->push_back(std::move($decl)); $$ = std::move($list); }
  ;

statement
  : expression SEMICOLON { $$ = std::make_unique<ExprStmt>(std::move($1)); }
  ;

expression
  : assignment_expression { $$ = std::move($1); }
  ;

assignment_expression
  : IDENTIFIER[id] EQUAL assignment_expression[value] { $$ = std::make_unique<AssignmentExpr>($id, std::move($value)); }
  | exclusive_or_expression { $$ = std::move($1); }
  ;

exclusive_or_expression
  : exclusive_or_expression[lhs] XOR and_expression[rhs] { $$ = std::make_unique<BinOpExpr>("^", std::move($lhs), std::move($rhs)); }
  | and_expression { $$ = std::move($1); }
  ;

and_expression
  : and_expression[lhs] AND primary_expression[rhs] { $$ = std::make_unique<BinOpExpr>("&", std::move($lhs), std::move($rhs)); }
  | primary_expression { $$ = std::move($1); }
  ;

primary_expression
  : NUMBER { $$ = std::make_unique<NumberExpr>($1); }
  | STRING { $$ = std::make_unique<StringExpr>($1); }
  | IDENTIFIER { $$ = std::make_unique<IdUseExpr>($1); }
  ;


%%
 
