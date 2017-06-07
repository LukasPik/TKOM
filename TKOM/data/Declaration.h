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

	virtual void getCode(std::ofstream &file, int x)
	{
		std::string text = "";
		text.append( getSpaceFile(x));
		text.append( this->var->type);
		text.append( " ");
		text.append( this->var->name);
		text.append( ";\n");

		std::cout << text;
		file << text;
	}


};