%{
#include "Parser.hpp"
#include "Scanner.hpp"

#define tk(tk)                                                            \
  ctx.loc.columns(yyleng);                                                \
  return blazing::Parser::make_##tk(ctx.loc)
#define tkv(tk, v)                                                        \
  ctx.loc.columns(yyleng);                                                \
  return blazing::Parser::make_##tk(v, ctx.loc)

#define YY_DECL                                                           \
  blazing::Parser::symbol_type blazing::Scanner::lex(blazing::LexContext &ctx,  \
                                                 blazing::SourceModule &sm)

%}
 
%option noyywrap nounput noinput batch
%option c++ interactive outfile="Lexer.cpp"

number                    (([1-9][0-9]*)|0)\.?[0-9]*
identifier                ([_a-zA-Z][_a-zA-Z0-9]*)
string                    \"(\\.|[^\\"])*\"	
 
%%
  ctx.loc.step();

"import"                            { tk(IMPORT); }

{number}                            { tkv(NUMBER, strtod(yytext, nullptr)); }
{identifier}                        { tkv(IDENTIFIER, yytext); }
{string}                            { tkv(STRING, yytext); }
 
"//".*                              { ctx.loc.columns(); }

[ \t\r\v]                           { ctx.loc.columns(); }
[\n]                                { ctx.loc.lines(); }
<<EOF>>                             { tk(END); }

.                                   { ctx.loc.columns();
                                      ctx.loc.lines(); 
                                      std::string str("Unrecognized char '");
                                      str += yytext[0];
                                      str += "'";
                                      throw blazing::SyntaxError(ctx.loc, str); }
 
%%

int yyFlexLexer::yylex() {
    throw std::runtime_error("Bad call to yyFlexLexer::yylex()");
}
