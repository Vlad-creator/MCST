#pragma once

#include "numgrammar.tab.hh"
#include "node.hpp"
#include <FlexLexer.h>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <numeric>
#include <memory>

namespace yy
{
class Driver
{
	FlexLexer*        myflex;
    const char*       program;
    std::shared_ptr<ast::Scope_Node>  global_scope;
    std::shared_ptr<ast::Scope_Node>  cur_scope;
    std::ifstream     input_file;

public:
	Driver(const char* prog_name);
	parser::token_type yylex(parser::semantic_type *yylval);
    bool parse();
    void init();
    void addInstr(std::shared_ptr<ast::Node> node);
    std::shared_ptr<ast::Scope_Node> get_cur_scope()
    {
        return cur_scope;
    }

    void set_cur_scope(std::shared_ptr<ast::Scope_Node> new_scope)
    {
        cur_scope = new_scope;
    }

    std::shared_ptr<Var> look_up(const std::string name)
    {
        return cur_scope->look_up(name);
    }
    void add_var(std::shared_ptr<Var> var)
    {
        cur_scope->add_var(var);
    }
    ~Driver();
};
}