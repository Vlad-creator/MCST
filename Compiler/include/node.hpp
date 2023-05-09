#pragma once

#include "types.hpp"
#include <iostream>
#include <cstdlib>
#include "NameTab.hpp"
#include "llvm/IR/Value.h"
#include "CodeGen.hpp"
#include <vector>
#include <memory>

namespace ast
{
 
class Node
{
	node_type type;
public:
    virtual int process_node() = 0;
    virtual llvm::Value *codegen() = 0;

    Node(node_type type_) : type(type_) {};
	~Node() {};
};

class Var_Node final : public Node
{
	std::shared_ptr<Var> var;
public:
	Var_Node(std::string nm, int val): Node(node_type::VAR_NODE), var(std::make_shared<Var>(nm, val)) {};
	Var_Node(std::shared_ptr<Var> v): Node(node_type::VAR_NODE), var(v) {};
	~Var_Node() {};
	llvm::Value *codegen() 
	{ 
		return GlobalGen->AddDeclRead(var->name); 
	}
    llvm::Value *assign(llvm::Value *Rhs) 
    {
    	return GlobalGen->AddDeclWrite(var->name, Rhs);
    }
	void set_val(int val)
	{
		var->set_val(val);
	}
	int process_node() override;
};

class Scope_Node final : public Node
{
	std::vector<std::shared_ptr<Node>> instrs;
    NameTab Tab;
	std::shared_ptr<Scope_Node> prev;
public:
	Scope_Node(): Node(node_type::SCOPE_NODE), prev(nullptr) {};
	Scope_Node(std::shared_ptr<Scope_Node> old): Node(node_type::SCOPE_NODE), prev(old) {};
	~Scope_Node() {};

	int process_node() override;
	llvm::Value *codegen() override;
	void addInstr(std::shared_ptr<Node> instr);
	std::shared_ptr<Var> look_up(const std::string name);
	void add_var(std::shared_ptr<Var>);
	std::shared_ptr<Scope_Node> get_prev()
	{
		return prev;
	}
};

class Op_Node final : public Node
{
	op_type op;
	std::shared_ptr<Node> left;
	std::shared_ptr<Node> right;
public:
	int process_node() override;
	llvm::Value *codegen() override 
	{
	    llvm::Value *LeftV = left->codegen();
	    llvm::Value *RightV = right->codegen();

	    if (op == op_type::ASSIGN)
	      return static_pointer_cast<Var_Node>(left)->assign(RightV);

	    return GlobalGen->AddOperation(op, LeftV, RightV);
    }
	op_type get_op_type() { return op; };

	std::shared_ptr<Node> get_left() { return left; };
	std::shared_ptr<Node> get_right() { return right; };

	Op_Node(op_type _op, std::shared_ptr<Node> left_, std::shared_ptr<Node> right_) : Node(node_type::OP_NODE), op(_op),
	                                                  left(left_), right(right_) {};
	~Op_Node() {};
};

class UnOp_Node final : public Node
{
	unop_type op;
	std::shared_ptr<Node> num;
public:
	int process_node() override;
	llvm::Value *codegen() override 
	{
		if (op == unop_type::INPUT)
		{
		    return GlobalGen->AddUnOperation(op, nullptr, nullptr);
		}
		else
		{
			llvm::Value *RightV = num->codegen();
		    return GlobalGen->AddUnOperation(op, nullptr, RightV);
		}
    }
	unop_type get_unop_type() { return op; };

	std::shared_ptr<Node> get_num() { return num; };

	UnOp_Node(unop_type op_, std::shared_ptr<Node> num_) : Node(node_type::UnOP_NODE), op(op_),
	                                       num(num_) {};
	~UnOp_Node() {};
};

class Num_Node final : public Node
{
	int val;
public:
	int process_node() override { return get_val(); };
	llvm::Value *codegen() override 
	{
	    return llvm::ConstantInt::get(GlobalGen->getInt32Ty(), val);
    }
	int get_val() { return val; };

	Num_Node(int val_) : Node(node_type::NUM_NODE), val(val_) {};
	~Num_Node() {};
};

class If_Node final : public Node
{
	std::shared_ptr<Node> cond;
	std::shared_ptr<Node> body;
public:
	If_Node(std::shared_ptr<Node> cond_, std::shared_ptr<Node> body_):
	        Node(node_type::IF_NODE), cond(cond_), body(body_) {};
	int process_node() override;
    llvm::Value* codegen() override
    {
    	auto* condV = cond->codegen();
    	auto* thenV = GlobalGen->StartIf(condV);
    	body->codegen();
    	GlobalGen->EndIf(thenV);
    	return nullptr;
    }
	~If_Node() {};
};

class While_Node final : public Node
{
	std::shared_ptr<Node> cond;
	std::shared_ptr<Node> body;
public:
	While_Node(std::shared_ptr<Node> cond_, std::shared_ptr<Node> body_):
	           Node(node_type::WHILE_NODE), cond(cond_), body(body_) {};
	int process_node() override;
	llvm::Value* codegen() override
    {
    	llvm::Value* condV = cond->codegen();
    	auto thenV = GlobalGen->StartWhile(condV);
    	body->codegen();
    	auto condVNew = cond->codegen();
    	GlobalGen->EndWhile(condVNew, thenV);
    	return nullptr;
    }
	~While_Node() {};
};

};