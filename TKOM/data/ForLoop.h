#include "node.h"
#include <vector>

class Variable;
class Cond;
class VarTab;

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

	void addVar(VarTab &var)
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

	/*virtual std::string getCode(std::fstream &file, int x)
	{
		file << getSpaceFile(x);
		file << "for(";
		file << this->loopVar->name.append(" ; ");
		file << this->loopCond->getCode(file, x).append(" ; ");
		file << this->endExpr->getCode(file, x).append(")\n");
		file << getSpaceFile(x).append("{\n");
		for (auto &it : list)
		{
			it->getCode(file, x + 1);
		}
		file << getSpaceFile(x);
		file << "}";
	}*/

	
	std::vector<Node*> list;
	std::vector<VarTab> varList;
};