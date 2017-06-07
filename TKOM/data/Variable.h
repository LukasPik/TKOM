#include "node.h"
#include <iostream>

class Variable : public Node
{

public:


	virtual Type getType() 
	{
		return Type::Variable;
	}
	virtual void showList(int x)
	{
		//getSpace(x);
		if (crement)
		{
			getSpace(x);
			std::string nameType = "Incrementation";
			if (crement == 2)
				nameType = "Decrementation";
				
			std::cout << nameType << " type " << type << " name " << name << std::endl;
		}
		else
			std::cout << this->getString() <<" type " << type <<" name " << name <<std::endl;
	}

	std::string getText()
	{
		std::string text = "";
		if (type != "")
		{
			text.append(type);
			text.append(" ");
		}
		text.append(this->name);

		if (crement == 1)
		{
			text.append("++");
		}
		else if (crement == 2)
		{
			text.append("--;");
		}
		return text;
	}

	virtual void getCode(std::ofstream &file, int x)
	{
		std::string text = "";// getSpaceFile(x);
		if (type != "")
		{
			text.append(type);
			text.append(" ");
		}
		text.append(this->name);

		if (crement == 1)
		{
			text.append("++");
		}
		else if (crement == 2)
		{
			text.append("--");
		}
		std::cout << text;
		file << text;
	}

	int crement = 0;  // 0 -none, 1 - incrementation, 2 - decrementation
	std::string type = "";
	std::string name;
};