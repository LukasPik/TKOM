#include "node.h"
#include "../Type.h"
#include <vector>

class Variable;

class Assignment : public Node
{
	Variable *var;

public:
	void setVar(Variable *var)
	{
		this->var = var;
	}
	Variable *getVar()
	{
		return var;
	}

	void addNode(Node *node)
	{
		this->list.push_back(node);
	}

	virtual Type getType()
	{
		return Type::Assignment;
	}
	virtual void showList(int x)
	{
		getSpace(x);
		std::cout << this->getString() << " type: " << var->type ;
		//var->showList(x);
		for (auto &it : list)
		{
			it->showList(x+1);
		}
	}

	token::Type assigType;
	std::vector<Node*> list;
};