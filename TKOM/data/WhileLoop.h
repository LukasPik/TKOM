#pragma once
#include "node.h"
#include <vector>

class Cond;

class WhileLoop : public Node
{
	

public:

	void addNode(Node *node)
	{
		this->list.push_back(node);
	}

	virtual Type getType()
	{
		return Type::WhileLoop;
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
		std::string text = "";
		text.append( getSpaceFile(x));
		text.append( "while(");
		std::cout << text;
		file << text;
		text = "";
		this->condition->getCode(file, x);
		text.append( ")\n");
		text.append( getSpaceFile(x).append("{\n"));
		std::cout << text;
		file << text;

		for (auto &it : list)
		{
			if (it->getType() == Type::Variable)
			{
				std::cout << getSpaceFile(x + 1);
				file << getSpaceFile(x + 1);
				it->getCode(file, x + 1);
				std::cout << ";\n";
				file << ";\n";
			}
			else
				it->getCode(file, x + 1);
		}
		text = getSpaceFile(x).append("}\n");
		std::cout << text;
		file << text;
	}

	std::vector<Node*> list;
	Cond *condition;

};