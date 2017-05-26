
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

	std::vector<Node*> content;
};