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

	virtual void getCode(std::ofstream &file, int x)
	{
		std::string text = "";
		if (inBrackets)
			text.append( "(");

		text.append( value1->value);
		if (condOperator != nullptr)
		{
			switch (condOperator->type)
			{
			case token::Type::And:
				text.append( " && ");
				break;
			case token::Type::Or:
				text.append( " || ");
				break;
			case token::Type::Equality:
				text.append( " == ");
				break;
			case token::Type::Inequality:
				text.append( " != ");
				break;
			case token::Type::GreaterEqual:
				text.append( " >= ");
				break;
			case token::Type::LessEqual:
				text.append( " <= ");
				break;
			case token::Type::Greater:
				text.append( " > ");
				break;
			case token::Type::Less:
				text.append( " < ");
				break;
			}
			text.append( value2->value);
		}
		if (inBrackets)
			text.append( ")");

		std::cout << text;
		file << text;
	}

	bool inBrackets = false;
	Token *condOperator;
	Token *value1;
	Token *value2;
	std::vector<Expression*> list;		//prob useless
};
