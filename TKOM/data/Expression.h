#include "node.h"
#include "../Token.h"
#include <vector>

class Expression :public Node
{

public:
	virtual Type getType()
	{
		return Type::Expression;
	}

	void addExpr(Token *token)
	{
		this->exprList.push_back(token);
	}
	void addNode(Node *node)
	{
		this->list.push_back(node);
	}
	virtual void showList(int x)
	{
		getSpace(x);
		std::cout << this->getString() << " ";
		for (auto &it : exprList)
		{
			std::cout << getTypeName(it->type) << " " ;
		}
		std::cout << std::endl;
	}

	bool inBracket = false;
	std::vector<Token*> exprList;
	std::vector<Node*> list;   // rather useless
};