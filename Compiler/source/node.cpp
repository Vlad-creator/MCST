#include "../include/node.hpp"

namespace ast
{

int Scope_Node::process_node()
{
	int result = 0;

    for (auto&& node : instrs) 
    {
        result += node->process_node();
    }
    return result;
}

llvm::Value* Scope_Node::codegen()
{
    for (auto&& node : instrs) 
    {
        node->codegen();
    }
    return nullptr;
}

void Scope_Node::addInstr(std::shared_ptr<Node> instr)
{
	if (instr != nullptr)
	{
		instrs.push_back(instr);
	}
}

int Op_Node::process_node()
{
	int result = 0;
	int value = 0;
	std::shared_ptr<Var_Node> lvalue;
	std::shared_ptr<Node> l_child = get_left();
	std::shared_ptr<Node> r_child = get_right();
	switch(op)
	{
		case op_type::PLUS:
			result = l_child->process_node() + r_child->process_node();
			return result;
		case op_type::MINUS:
			result = l_child->process_node() - r_child->process_node();
			return result;
		case op_type::MUL:
			result = l_child->process_node() * r_child->process_node();
			return result;
		case op_type::DIV:
			result = l_child->process_node() / r_child->process_node();
			return result;
		case op_type::MOD:
			result = l_child->process_node() % r_child->process_node();
			return result;
		case op_type::LOR:
			result = l_child->process_node() || r_child->process_node();
			return result;
		case op_type::LAND:
			result = l_child->process_node() && r_child->process_node();
			return result;
		case op_type::GR:
			result = l_child->process_node() > r_child->process_node();
			return result;
		case op_type::LS:
			result = l_child->process_node() < r_child->process_node();
			return result;
		case op_type::GREQ:
			result = l_child->process_node() >= r_child->process_node();
			return result;
		case op_type::LSEQ:
			result = l_child->process_node() <= r_child->process_node();
			return result;
		case op_type::EQUAL:
			result = l_child->process_node() == r_child->process_node();
			return result;
		case op_type::NEQUAL:
			result = l_child->process_node() != r_child->process_node();
			return result;
		case op_type::ASSIGN:
            value = r_child->process_node();
            lvalue = std::static_pointer_cast<Var_Node>(l_child);
			lvalue->set_val(value);
			return value;
		default:
			return result;
	}
}

int UnOp_Node::process_node()
{
	int result = 0;
	std::shared_ptr<Node> num = get_num();
	std::shared_ptr<Var_Node> lvalue;
	switch(op)
	{
		case unop_type::UnPLUS:
			result = num->process_node();
			return result;
		case unop_type::UnMINUS:
			result = (-1) * num->process_node();
			return result;
		case unop_type::NOT:
			result = !(num->process_node());
			return result;
		case unop_type::OUTPUT:
			result = num->process_node();
			std::cout << result << std::endl;
			return result;
		case unop_type::INPUT:
			std::cin >> result;
			return result;
		default:
			return result;
	}
}

int Var_Node::process_node()
{
	return var->value;
}

std::shared_ptr<Var> Scope_Node::look_up(std::string name)
{
	std::shared_ptr<Scope_Node> tmp_prev = get_prev();
	if (Tab.find_var(name))
	{
		return Tab.get_var(name);
	}
	while (tmp_prev != nullptr)
	{
		if (tmp_prev->Tab.find_var(name))
	    {
	    	return tmp_prev->Tab.get_var(name);
	    }
	    tmp_prev = tmp_prev->get_prev();
	}
	return nullptr;
}

void Scope_Node::add_var(std::shared_ptr<Var> var)
{
	GlobalGen->AddDecl(var->name);
	Tab.add_var(var);
}

int If_Node::process_node()
{
	if (cond->process_node())
	{
		body->process_node();
	}
	return 0;
}

int While_Node::process_node()
{
	while (cond->process_node())
	{
		body->process_node();
	}
	return 0;
}

}