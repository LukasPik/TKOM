#pragma once

#include "node.h"
#include <iostream>
#include <vector>
class VarTab;

class Program : public Node
{

public:

	void addNode( Node *node)
	{
		this->list.push_back(node);
	}

	void addVar(VarTab &var)
	{
		varList.push_back(var);
	}

	void showVarList()
	{
		for (auto &it : varList)
		{
			std::cout << "Zmienna: " << it.var.name << " type: " << it.var.type << std::endl;
		}
	}

	virtual Type getType()
	{
		return Type::Program;
	}

	virtual void showList(int x)
	{
		getSpace(x);
		std::cout << this->getString() << std::endl;
		for (auto &it : list)
		{
			it->showList(x + 1);
		}
	}
	virtual void getCode(std::ofstream &file, int x)
	{
		std::cout << "\nint main() \n{ \n";
		file << "\nint main() \n{\n";


		for (auto &it : list)
		{
			it->getCode(file, x + 1);
		}
		std::cout << "}";
		file << "}";
	}


	std::vector<Node*> list = {};
	std::vector<VarTab> varList;
};