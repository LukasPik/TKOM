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

	/*virtual std::string getCode(std::fstream &file, int x)
	{
		file << getSpaceFile(x);
		file << "while(";
		file << this->condition->getCode(file, x).append(")\n");
		file << getSpaceFile(x).append("{\n");
		for (auto &it : list)
		{
			it->getCode(file, x + 1);
		}
	}*/

	std::vector<Node*> list;
	Cond *condition;

};