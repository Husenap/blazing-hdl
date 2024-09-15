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
%token IMPORT
%token TYPE IDENTIFIER NUMBER BOOLEAN STRING
%token IS EQ NEQ LT LEQ GT GEQ PLUS MINUS MUL DIV MOD

%type <double> NUMBER;
%type <std::string> IDENTIFIER;
%type <std::string> STRING;


// =================================== Grammar Definition ================================
 
%%

%start module;

module
  :  END {  }
  ;


%%
 
