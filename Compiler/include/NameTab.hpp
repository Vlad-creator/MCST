#pragma once

#include <unordered_map>
#include <memory>
#include <iostream>

struct Var
{
	Var(std::string name_, int val) : name(name_), value(val) {}; 
	const std::string name;
	int value;
	void set_val(int val)
	{
		value = val;
	}
};

class NameTab final
{
private:
	std::unordered_map<std::string, std::shared_ptr<Var>> Tab;
public:
	bool add_var(std::shared_ptr<Var>);
	std::shared_ptr<Var>  get_var(const std::string& name) const;
	bool find_var(const std::string& name) const;
};