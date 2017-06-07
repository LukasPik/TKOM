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

	virtual void getCode(std::ofstream &file, int x)
	{
		std::string text = "";
		if (inBrackets)
		{
			std::cout << "(";
			file << "(";
		}
		for (int i = 0; i < conditions.size() ; ++i)
		{
			conditions[i]->getCode(file, x);
			if (i < condOper.size())
			{
				switch (condOper[i]->type)
				{
				case token::Type::And :
					text.append( " && ");
					break;
				case token::Type::Or :
					text.append( " || ");
					break;
				}
				std::cout << text;
				file << text;
				text = "";
			}
		}
		text = "";
		if (inBrackets)
			text.append( ")");

		std::cout << text;
		file << text;
	}



	bool inBrackets = false;
	token::Type condOperator = token::Type::Undefined;
	std::vector<Node*> conditions;
	std::vector<Token*> condOper;

};