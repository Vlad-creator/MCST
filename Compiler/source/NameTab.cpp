#include "../include/NameTab.hpp"

bool NameTab::find_var(const std::string& name) const
{
	return Tab.contains(name);
}

bool NameTab::add_var(std::shared_ptr<Var> var)
{
	if(Tab.contains(var->name))
	{
		return false;
	}
	Tab.insert({var->name, var});
	return true;
}

std::shared_ptr<Var> NameTab::get_var(const std::string& name) const
{
	if (Tab.contains(name))
	{
		return Tab.find(name)->second;
	}
	return nullptr;
}