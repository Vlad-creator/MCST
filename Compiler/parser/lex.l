%option C++
%option yylineno
%option noyywrap nounput noinput batch debug

%{

	#include "numgrammar.tab.hh"

%}



TAB		[ \n\t\v]+
DIGIT   [0-9]
DIGIT1  [1-9]
SYMB    [a-zA-Z]
SYMBD   [a-zA-Z0-9]


%%

{TAB}              /* skip blanks and tabs */
"+"                return yy::parser::token_type::PLUS;
"-"                return yy::parser::token_type::MINUS;
"*"                return yy::parser::token_type::MUL;
"/"                return yy::parser::token_type::DIV;
"%"                return yy::parser::token_type::MOD;
"("                return yy::parser::token_type::LBR;
")"                return yy::parser::token_type::RBR;
"{"                return yy::parser::token_type::LBC;
"}"                return yy::parser::token_type::RBC;
";"                return yy::parser::token_type::SCOLON;
"print"            return yy::parser::token_type::KW_PRINT;
"if"               return yy::parser::token_type::KW_IF;
"while"            return yy::parser::token_type::KW_WHILE;
{DIGIT1}{DIGIT}*   return yy::parser::token_type::NUMBER;
{DIGIT}            return yy::parser::token_type::NUMBER;
{SYMB}{SYMBD}*     return yy::parser::token_type::NAME;
"="                return yy::parser::token_type::ASSIGN;
"||"               return yy::parser::token_type::LOR;
"&&"	           return yy::parser::token_type::LAND;	
"=="               return yy::parser::token_type::EQUAL;
"!="     		   return yy::parser::token_type::NEQUAL;
">"      		   return yy::parser::token_type::GR;
">="     		   return yy::parser::token_type::GREQ;
"<"      		   return yy::parser::token_type::LS;
"<="     		   return yy::parser::token_type::LSEQ;
"!"      		   return yy::parser::token_type::NOT;
"?"      		   return yy::parser::token_type::QMARK;
.			       return yy::parser::token_type::ERROR;

%%
