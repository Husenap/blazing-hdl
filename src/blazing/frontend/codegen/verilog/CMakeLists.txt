add_library(blazing_codegen_verilog STATIC "Transpiler.hpp")

target_link_libraries(blazing_codegen_verilog PUBLIC blazing_parser)
