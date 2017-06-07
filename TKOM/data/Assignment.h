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
		if (toDelete)
			std::cout << "- ";
		std::cout << this->getString() << " type: " << var->type << " name " << var->name ;
		//var->showList(x);
		for (auto &it : list)
		{
			it->showList(x+1);
		}
	}

	std::string getAssig(int x)
	{
		std::string text = "";
		text.append(getSpaceFile(x));
		if (toDelete)
			text.append("- ");

		text.append(this->var->getText());
		switch (assigType)
		{
		case token::Type::MinusEqual :
			text.append(" -= ");
			break;
		case token::Type::PlusEqual:
			text.append(" += ");
			break;
		default:
			text.append( " = ");
			break;
		}
		text.append( dynamic_cast<Expression*>(list[0])->getText());

		text.append(";");
		return text;
	} 

	bool toAdd = false, toDelete = false;

	token::Type assigType;
	std::vector<Node*> list;
};