#include "node.h"
#include "../Token.h"

class Expression;


class SimpleCond : public Node
{

public:
	void addNode(Expression *node)
	{
		this->list.push_back(node);
	}

	virtual Type getType()
	{
		return Type::SimpleCond;
	}
	virtual void showList(int x)
	{
		getSpace(x);
		std::cout << this->getString() << " val1 " << getTypeName(value1->type) << " val2 " << getTypeName(value2->type) <<  std::endl;

	}

	//virtual std::string getCode(std::fstream &file, int x)
	//{
	//	file << getSpaceFile(x);
	//	file << "Simple cond";
	//}

	bool inBrackets = false;
	Token *condOperator;
	Token *value1;
	Token *value2;
	std::vector<Expression*> list;		//prob useless
};
