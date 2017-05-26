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


	int crement = 0;  // 0 -none, 1 - incrementation, 2 - decrementation
	std::string type = "";
	std::string name;
};