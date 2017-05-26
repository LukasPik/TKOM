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

	std::vector<Node*> list;
	Cond *condition;

};