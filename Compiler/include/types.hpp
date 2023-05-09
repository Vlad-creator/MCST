#pragma once

namespace ast
{

enum class node_type
{
	OP_NODE, NUM_NODE, UnOP_NODE, START_NODE, 
	SCOPE_NODE, VAR_NODE, IF_NODE, WHILE_NODE
};

enum class op_type
{
	PLUS, MINUS, MUL, DIV, MOD, LOR, LAND, ASSIGN,
	GR, GREQ, LS, LSEQ, EQUAL, NEQUAL

};

enum class unop_type
{
	UnPLUS, UnMINUS, INPUT, NOT, OUTPUT
};

};