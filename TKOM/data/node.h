#pragma once
#include <memory>
#include <iostream>
#include <string>





class Node 
{
protected:
	std::weak_ptr<Node> parent;

public:
	enum class Type
	{
		Assignment,
		Cond,
		Declaration,
		Expression,
		ForLoop,
		IfState,
		Program,
		SimpleCond,
		Variable,
		WhileLoop,
	};
	virtual Type getType() = 0;
	virtual void showList(int ) = 0;
	//virtual std::string getCode(std::fstream &, int) = 0;

	void getSpace(int x)
	{
		while (x--)
		{
			std::cout << "\t";
		}
	}

	std::string getSpaceFile(int x)
	{
		std::string toReturn = "\t";
		--x;
		while (x-- > 0)
		{
			toReturn.append("\t");
		}
		return toReturn;
	}

	std::string getString()
	{
		auto x = getType();
		switch (x)
		{
		case	Type::Assignment:
			return "Assignment";
		case	Type::Cond:
			return "Cond";
		case	Type::Declaration:
			return "Declaration";
		case	Type::Expression:
			return"Expression";
		case	Type::ForLoop:
			return "ForLoop";
		case	Type::IfState:
			return "IfState";
		case	Type::Program:
			return "Program";
		case	Type::SimpleCond:
			return "SimpleCond";
		case	Type::Variable:
			return "Variable";
		case	Type::WhileLoop:
			return "WhileLoop";
		default:
			return "Unknown";
		}
	}



};




#include "Variable.h"

class VarTab
{
public:
	Variable var;
	bool changing = false;
	bool isSet = false;
	bool isDecl = false;


	VarTab(Variable *var)
	{
		this->var.name = var->name;
		this->var.type = var->type;
	}
};

#include "Expression.h"
#include "Assignment.h"
#include "Cond.h"
#include "Declaration.h"

#include "ForLoop.h"
#include "IfStat.h"
#include "Program.h"
#include "SimpleCond.h"

#include "WhileLoop.h"



