find_package(BISON 3.8.2 REQUIRED)
find_package(FLEX 2.6.4 REQUIRED)

bison_target(blazing_grammar "Grammar.yy"
             "${CMAKE_CURRENT_BINARY_DIR}/Parser.cpp")
flex_target(blazing_lexer "Lex.l" "${CMAKE_CURRENT_BINARY_DIR}/Lexer.cpp")
add_flex_bison_dependency(blazing_lexer blazing_grammar)

add_library(blazing_parser ${BISON_blazing_grammar_OUTPUTS}
                           ${FLEX_blazing_lexer_OUTPUTS})

target_link_libraries(blazing_parser PUBLIC blazing_ast)

target_include_directories(blazing_parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(blazing_parser PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
