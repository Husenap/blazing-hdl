#ifndef FLEX_SCANNER
#include "FlexLexer.h"
#endif

namespace blazing {

class Scanner : public yyFlexLexer {
public:
  Scanner(std::istream& arg_yyin, std::ostream& arg_yyout) : yyFlexLexer(arg_yyin, arg_yyout) {}
  Scanner(std::istream* arg_yyin = nullptr, std::ostream* arg_yyout = nullptr) : yyFlexLexer(arg_yyin, arg_yyout) {}
  blazing::Parser::symbol_type lex(blazing::LexContext& ctx, blazing::SourceModule& sm);
};

} // namespace blazing
