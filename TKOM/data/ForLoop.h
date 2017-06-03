#include "node.h"
#include <vector>

class Variable;
class Cond;
class varTab;

class ForLoop : public Node
{
	Variable *loopVar;
	Cond *loopCond;
	Node *endExpr;
public:

	void setLoopVar(Variable *var)
	{
		this->loopVar = var;
	}

	Variable *getLoopVar()
	{
		return loopVar;
	}

	void setLoopCond(Cond *var)
	{
		this->loopCond = var;
	}

	Cond *getLoopCond()
	{
		return loopCond;
	}

	void setEndExpr(Node *var)
	{
		this->endExpr = var;
	}
	
	Node *getEndExpr()
	{
		return endExpr;
	}

	void addNode(Node *node)
	{
		this->list.push_back(node);
	}

	virtual Type getType() 
	{
		return Type::ForLoop;
	}

	void addVar(varTab &var)
	{
		varList.push_back(var);
	}

	virtual void showList(int x)
	{
		getSpace(x);
		std::cout << this->getString() << " ";
		//loopVar->showList(x);
		
		for (auto &it : list)
		{
			it->showList(x + 1);
		}
	}

	
	std::vector<Node*> list;
	std::vector<varTab> varList;
};