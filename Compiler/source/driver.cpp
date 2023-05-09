#include <iostream>

#include "../include/driver.hpp"

namespace yy
{
Driver::Driver(const char* prog_name): program(prog_name), myflex(new yyFlexLexer), 
							     global_scope(new ast::Scope_Node), cur_scope(global_scope)
{
	input_file.open(program);

    // Set new input stream
    myflex->switch_streams(input_file, std::cout);

    input_file.clear();
    input_file.seekg(0);
}


parser::token_type Driver::yylex(parser::semantic_type *yylval) 
{
    parser::token_type tt = static_cast<parser::token_type>(myflex->yylex());
    
	if (tt == yy::parser::token_type::NUMBER)
    {
        yylval->as<int>() = std::stoi(myflex->YYText());
    }
    if (tt == yy::parser::token_type::NAME) 
    {
        std::string name(myflex->YYText());
        parser::semantic_type tmp;
        tmp.as<std::string>() = name;
        yylval->swap<std::string>(tmp);
    }

    return tt;
}

bool Driver::parse() 
{
	parser parser(this);
    GlobalGen.reset(createCodeGen("pcl.module"));
    GlobalGen->StartFunction("__pcl_start");

	int res = parser.parse();

    GlobalGen->EndCurrentFunction();
    auto ModuleName = std::filesystem::path(program).filename().string() + ".ll";
    std::cout << "Saving module to: " << ModuleName << "\n";
    GlobalGen->SaveModule(ModuleName);
 	return (!res); 
}

void Driver::addInstr(std::shared_ptr<ast::Node> node)
{
	if (node != nullptr)
	{
		cur_scope->addInstr(node);
	}
}

void Driver::init()
{
	//cur_scope->process_node();
    cur_scope->codegen();
}

Driver::~Driver()
{
	input_file.close();
}

}
