#include "node.h"
#include "../Type.h"
#include "../Token.h"
#include <vector>
class SimpleCond;

class Cond : public Node
{
public:
	void addCond(Node *node)
	{
		this->conditions.push_back(node);
	}

	void addOperator(Token *node)
	{
		this->condOper.push_back(node);
	}

	virtual Type getType()
	{
		return Type::Cond;
	}
	virtual void showList(int x)
	{
		getSpace(x);
		std::cout << this->getString() << std::endl;
		for (auto &it : conditions)
		{
			it->showList(x+1);
		}
	}

	//virtual std::string getCode(std::fstream & file, int x)
	//{
	//	file << "Condition";
	//}



	bool inBrackets = false;
	token::Type condOperator = token::Type::Undefined;
	std::vector<Node*> conditions;
	std::vector<Token*> condOper;

};