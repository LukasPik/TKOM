#include "node.h"

class Variable;

class Declaration : public Node
{
	Variable *var;
public:
	void setVar(Variable *var)
	{
		this->var = var;
	}

	Variable *getVar()
	{
		return this->var;
	}


	virtual Type getType()
	{
		return Type::Declaration;
	}

	virtual void showList(int x)
	{
		getSpace(x);
		std::cout << this->getString() <<" ";
		//var->showList(x);

	}

	//virtual std::string getCode(std::fstream &file, int x)
	//{
	//	file << getSpaceFile(x);
	//	file << this->var->type;
	//	file << " ";
	//	file << this->var->name;
	//	file << ";\n";
	//}


};