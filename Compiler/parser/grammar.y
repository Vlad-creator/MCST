%language "c++"
%skeleton "lalr1.cc"

%defines
%define api.value.type variant
%param {yy::Driver* driver}

%code requires
{
	#include <iostream>
	#include <string>
	#include <utility>
	#include <algorithm>
	#include <vector>
	#include <memory>

	#include "../include/node.hpp"
	#include "../include/NameTab.hpp"

	namespace yy { class Driver; }
}

%code
{
#include "../include/driver.hpp"
#include "../include/node.hpp"

namespace yy 
{
	parser::token_type yylex(parser::semantic_type* yylval,                         
                         	 Driver* driver);
}
}

%token
      ASSIGN    "="
	PLUS      "+"
	MINUS     "-"
	SCOLON    ";"
	MUL       "*"
	DIV       "/"
	MOD       "%" 
	LBR       "("
      RBR       ")"
      LBC       "{"
      RBC       "}"

      LOR       "||"
      LAND      "&&"

      EQUAL     "=="
      NEQUAL    "!="
      GR        ">"
      GREQ      ">="
      LS        "<"
      LSEQ      "<="
      NOT       "!"

      KW_PRINT  "print"
      KW_IF     "if"
      KW_WHILE  "while"
      QMARK     "?"
	ERROR
;

%token <int>                                     NUMBER;
%token <std::string>                             NAME
%nterm <std::shared_ptr<ast::Node>>              expr0;
%nterm <std::shared_ptr<ast::Node>>              expr1;
%nterm <std::shared_ptr<ast::Node>>              expr2;
%nterm <std::shared_ptr<ast::Node>>              expr3;
%nterm <std::shared_ptr<ast::Node>>              line;
%nterm <std::shared_ptr<ast::Node>>              scope;
%nterm <std::shared_ptr<ast::Node>>              cl_scope;
%nterm <std::shared_ptr<ast::Node>>              op_scope;
%nterm <std::shared_ptr<ast::Node>>              lgc_or;
%nterm <std::shared_ptr<ast::Node>>              lgc_and;
%nterm <std::shared_ptr<ast::Node>>              lgc_equal;
%nterm <std::shared_ptr<ast::Node>>              pref;
%nterm <std::shared_ptr<ast::Node>>              lgc_relate;
%nterm <std::shared_ptr<ast::Node>>              assign;
%nterm <std::shared_ptr<ast::Node>>              lvalue;
%nterm <std::shared_ptr<ast::Node>>              block;
%nterm <std::shared_ptr<ast::Node>>              input;

%left '+' '-'
%left '*' '/' '%'

%start program

%%


program: instr                          { 
	                                      driver->init();
	                                  }
;

instr: line                             { 
	                                      driver->addInstr($1); 
	                                  }
| instr line                            { 
        	                                driver->addInstr($2); 
        	                            }
| instr scope                           { 
        	                                driver->addInstr($2);
        	                            }

;

scope: op_scope instr cl_scope          { 
	                                      $$ = $3; 
	                                  }
;

op_scope: LBC                           { 
	                                      driver->set_cur_scope(std::make_shared<ast::Scope_Node>(driver->get_cur_scope())); 
	                                  }
;

cl_scope: RBC                           { 
						  $$ = driver->get_cur_scope();
                                            driver->set_cur_scope(driver->get_cur_scope()->get_prev());
                                        }
;

lvalue: NAME                            {
	                                      int val;
	                                      std::string name;
	                                      std::shared_ptr<Var> tmp_var = driver->look_up($1);
	                                      if (tmp_var != nullptr)
	                                      {
                                            	tmp_var = driver->look_up($1);
	                                      }
	                                      else
	                                      {

	                                      	val = 0;
	                                      	tmp_var = std::make_shared<Var>($1, 0);
	                                      	driver->add_var(tmp_var);
	                                      }
                                            $$ = std::make_shared<ast::Var_Node>(tmp_var);
	                                  }
;

input: QMARK                             {
					          $$ = std::make_shared<ast::UnOp_Node>(ast::unop_type::INPUT, nullptr);
                                         }

assign: lvalue ASSIGN lgc_or            {
	                                      $$ = std::make_shared<ast::Op_Node>(ast::op_type::ASSIGN, $1, $3);
	                                  }
| lvalue ASSIGN input                   {
	                                      $$ = std::make_shared<ast::Op_Node>(ast::op_type::ASSIGN, $1, $3);
	                                  }
;


line: assign SCOLON                     {
	                                      $$ = $1;	                                  
	                                  }
| SCOLON                                {
    	                                      $$ = nullptr;
    	                                  }
| KW_PRINT lgc_or                       {
	                                      $$ = std::make_shared<ast::UnOp_Node>(ast::unop_type::OUTPUT, $2);
	                                  }
| KW_IF LBR lgc_or RBR block            {
                                            $$ = std::make_shared<ast::If_Node>($3, $5);
                                        }
| KW_WHILE LBR lgc_or RBR block         {
                                            $$ = std::make_shared<ast::While_Node>($3, $5);
                                        }
;

block: scope                            {
                                            $$ = $1;
                                        }
| line                                  {
                                            $$ = $1;
                                        }
;

lgc_or: lgc_or LOR lgc_and              {
	                                      $$ = std::make_shared<ast::Op_Node>(ast::op_type::LOR, $1, $3);
	                                  }
| lgc_and                               {
	                                      $$ = $1;
      	                            }
;

lgc_and: lgc_and LAND pref              {
                                            $$ = std::make_shared<ast::Op_Node>(ast::op_type::LAND, $1, $3);
                                        }
| pref                                  {
	                                      $$ = $1;
                                        }
;

pref: NOT lgc_equal                     {
	                                      $$ = std::make_shared<ast::UnOp_Node>(ast::unop_type::NOT, $2);
                                        }
| lgc_equal                             {
	                                      $$ = $1;
                                        }
;

lgc_equal: lgc_equal EQUAL lgc_relate   {
	                                      $$ = std::make_shared<ast::Op_Node>(ast::op_type::EQUAL, $1, $3);
                                        }
| lgc_equal NEQUAL lgc_relate           {
	                                      $$ = std::make_shared<ast::Op_Node>(ast::op_type::NEQUAL, $1, $3);
                                        }
| lgc_relate                            {
	                                      $$ = $1;
                                        }

lgc_relate: lgc_relate GR expr0         {
	                                      $$ = std::make_shared<ast::Op_Node>(ast::op_type::GR, $1, $3);
                                        }
| lgc_relate GREQ expr0                 {
	                                      $$ = std::make_shared<ast::Op_Node>(ast::op_type::GREQ, $1, $3);
                                        }
| lgc_relate LS expr0                   {
	                                      $$ = std::make_shared<ast::Op_Node>(ast::op_type::LS, $1, $3);
                                        }
| lgc_relate LSEQ expr0                 {
	                                      $$ = std::make_shared<ast::Op_Node>(ast::op_type::LSEQ, $1, $3);
                                        }
| expr0                                 {
	                                      $$ = $1;
                                        }
;

expr0  : expr0 PLUS  expr1              { $$ = std::make_shared<ast::Op_Node>(ast::op_type::PLUS, $1, $3); }
       | expr0 MINUS expr1              { $$ = std::make_shared<ast::Op_Node>(ast::op_type::MINUS, $1, $3); }
       | expr1                          { $$ = $1; }
;

expr1  : expr1 MUL expr2                { $$ = std::make_shared<ast::Op_Node>(ast::op_type::MUL, $1, $3); } 
       | expr1 DIV expr2                { $$ = std::make_shared<ast::Op_Node>(ast::op_type::DIV, $1, $3); }
       | expr1 MOD expr2                { $$ = std::make_shared<ast::Op_Node>(ast::op_type::MOD, $1, $3); }
       | expr2                          { $$ = $1; }
;

expr2  : PLUS  expr3                    { $$ = std::make_shared<ast::UnOp_Node>(ast::unop_type::UnPLUS, $2); std::cout << "expr2\n"; }
       | MINUS expr3                    { $$ = std::make_shared<ast::UnOp_Node>(ast::unop_type::UnMINUS, $2); }
       | expr3                          { $$ = $1; }
;

expr3  : NUMBER                         { 
	                                      $$ = std::make_shared<ast::Num_Node>($1); 
	                                  }
| NAME                                  { 
                                            $$ = std::make_shared<ast::Var_Node>(driver->look_up($1));
                                        }
| LBR lgc_or RBR                         { 
       	                               $$ = $2; 
       	                            }
;

%%

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,                         
                         Driver* driver)
{
  return driver->yylex(yylval);
}

void parser::error(const std::string&){}
}

