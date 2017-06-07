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
		std::cout << std::endl;
		
		for (auto &it : list)
		{
			it->showList(x + 1);
		}
		std::cout << std::endl;
	}

	virtual void getCode(std::ofstream &file, int x)
	{
		std::string text = "";
		text.append( getSpaceFile(x));
		text.append( "for( ");
		text.append( this->loopVar->name.append(" ; "));
		std::cout << text;
		file << text;
		text = "";
		this->loopCond->getCode(file, x);
		text.append( " ;");
		std::cout << text;
		file << text;
		text = "";
		this->endExpr->getCode(file, x);
		text.append( ")\n");
		text.append( getSpaceFile(x).append("{\n"));
		std::cout << text;
		file << text;
		for (auto &it : list)
		{
			it->getCode(file, x + 1);
		}
		text = getSpaceFile(x).append("}\n");
		std::cout << text;
		file << text;
	}

	
	std::vector<Node*> list;
	std::vector<VarTab> varList;
};