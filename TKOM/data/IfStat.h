
#include "node.h"
#include <vector>

class Cond;

class IfStat : public Node
{
public:
	Cond *condition;

	void addContent(Node *node)
	{
		this->content.push_back(node);
	}

	virtual Type getType()
	{
		return Type::IfState;
	}
	virtual void showList(int x)
	{
		getSpace(x);
		std::cout << this->getString() << " "<<condition->getString() <<std::endl;
		for (auto &it : content)
		{
			it->showList(x + 1);
		}
	}

	virtual void getCode(std::ofstream &file, int x)
	{
		std::string text = "";
		text.append( getSpaceFile(x));
		text.append( "if(");
		std::cout << text;
		file << text;
		text = " ";
		this->condition->getCode(file, x);
		text.append( ")\n");
		text.append( getSpaceFile(x).append("{\n"));
		std::cout << text;
		file << text;
		for (auto &it : content)
		{
			it->getCode(file, x + 1);
		}
		text = getSpaceFile(x).append("}\n");
		std::cout << text;
		file << text;

	}

	std::vector<Node*> content;
};