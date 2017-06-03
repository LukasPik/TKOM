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
	void getSpace(int x)
	{
		while (x--)
		{
			std::cout << "\t";
		}
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

class varTab
{
public:
	Variable var;
	bool changing = false;
	bool isSet = false;


	varTab(Variable *var)
	{
		this->var.name = var->name;
		this->var.type = var->type;
	}
};

#include "Assignment.h"
#include "Cond.h"
#include "Declaration.h"
#include "Expression.h"
#include "ForLoop.h"
#include "IfStat.h"
#include "Program.h"
#include "SimpleCond.h"

#include "WhileLoop.h"



