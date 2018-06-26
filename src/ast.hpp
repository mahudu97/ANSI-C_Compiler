#ifndef ast_hpp
#define ast_hpp

#include "ast/ast_node.hpp"
#include "ast/ast_program.hpp"
#include "ast/ast_declaration.hpp"
#include "ast/ast_specifier_qualifier.hpp"
#include "ast/ast_statement.hpp"
#include "ast/ast_expression.hpp"

#include "ast/compile_context.hpp"

extern const Node *parseAST(char* in);

#endif
