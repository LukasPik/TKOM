#pragma once
#include "stdafx.h"
#include "Optymalizator.h"

Optymalizator::Optymalizator(Program *init)
{
	this->tree = init;
}



void Optymalizator::optymalize()
{
	size_t size = this->tree->list.size();
	for (int i = 0; i < size; ++i) 
	{
		if (this->tree->list[i]->getType() == Node::Type::ForLoop)
		{
			auto pass = dynamic_cast<ForLoop*>(this->tree->list[i]);
			this->searchFor(pass->list, pass->getLoopVar());
		}
		else if (this->tree->list[i]->getType() == Node::Type::WhileLoop)
		{
			this->searchWhile(dynamic_cast<WhileLoop*>(this->tree->list[i])->list);
		}
		else if (this->tree->list[i]->getType() == Node::Type::IfState)
		{
			this->searchIf(dynamic_cast<IfStat*>(this->tree->list[i])->content);
		}
	}

}

void Optymalizator::searchFor(std::vector<Node*> list, Variable *loopVar)
{
	size_t size = list.size();
	std::vector<VarTab*> variables;
	VarTab *elem = new VarTab(loopVar);
	elem->changing = elem->isSet = true;
	variables.push_back(elem);

	for (int i = 0; i < size; ++i)
	{
		if (list[i]->getType() == Node::Type::ForLoop)
		{
			auto pass = dynamic_cast<ForLoop*>(this->tree->list[i]);
			this->searchFor(pass->list, pass->getLoopVar());
		}
		else if (list[i]->getType() == Node::Type::WhileLoop)
		{
			this->searchWhile(dynamic_cast<WhileLoop*>(list[i])->list);
		}
		else if (list[i]->getType() == Node::Type::IfState)
		{
			this->searchIf(dynamic_cast<IfStat*>(list[i])->content);
		}
		else if (list[i]->getType() == Node::Type::Assignment)
		{
			this->checkAssig(list[i], variables);
		}
		else if (list[i]->getType() == Node::Type::Variable)
		{
			this->checkVar(list[i], variables);
		}
		else if (list[i]->getType() == Node::Type::Declaration)
		{
			this->checkDecl(list[i], variables);
		}
	}
	std::cout << "===============================" << std::endl;
	for (auto &it : variables)
	{
		if (it->changing)
			std::cout << "!";

		std::cout << "Zawartosc listy: " << it->var.name << std::endl;
	}
}

void Optymalizator::searchWhile(std::vector<Node*> list)
{
	size_t size = list.size();
	std::vector<VarTab*> variables;
	for (int i = 0; i < size; ++i)
	{
		if (list[i]->getType() == Node::Type::ForLoop)
		{
			auto pass = dynamic_cast<ForLoop*>(this->tree->list[i]);
			this->searchFor(pass->list, pass->getLoopVar());
		}
		else if (list[i]->getType() == Node::Type::WhileLoop)
		{
			this->searchWhile(dynamic_cast<WhileLoop*>(list[i])->list);
		}
		else if (list[i]->getType() == Node::Type::IfState)
		{
			this->searchIf(dynamic_cast<IfStat*>(list[i])->content);
		}
		else if (list[i]->getType() == Node::Type::Assignment)
		{
			this->checkAssig(list[i], variables);
		}
		else if (list[i]->getType() == Node::Type::Variable)
		{
			this->checkVar(list[i], variables);
		}
		else if (list[i]->getType() == Node::Type::Declaration)
		{
			this->checkDecl(list[i], variables);
		}
	}
	std::cout << "===============================" << std::endl;
	for (auto &it : variables)
	{
		if (it->changing)
			std::cout << "!";

		std::cout << "Zawartosc listy: " << it->var.name << std::endl;
	}
}

void Optymalizator::searchIf(std::vector<Node*> list)
{
	size_t size = list.size();
	for (int i = 0; i < size; ++i)
	{
		if (list[i]->getType() == Node::Type::ForLoop)
		{
			auto pass = dynamic_cast<ForLoop*>(this->tree->list[i]);
			this->searchFor(pass->list, pass->getLoopVar());
		}
		else if (list[i]->getType() == Node::Type::WhileLoop)
		{
			this->searchWhile(dynamic_cast<WhileLoop*>(list[i])->list);
		}
		else if (list[i]->getType() == Node::Type::IfState)
		{
			this->searchIf(dynamic_cast<IfStat*>(list[i])->content);
		}
	}
}

void Optymalizator::checkAssig(Node *expr, std::vector<VarTab*> &list)
{
	Assignment *assig = dynamic_cast<Assignment*>(expr);
	std::cout << "Wystapienie przypisania: " << assig->getVar()->name << std::endl;
	VarTab *elem = new VarTab(assig->getVar());
	bool isChanging = false;

	if (assig->getVar()->type != "")
	{
		//typ (deklaracja + przypisanie)
		elem->isDecl = true;
	}
	else
	{
		for (auto &it : list)
		{
			if (it->var.name == elem->var.name)
			{
				if (it->changing)
				{
					isChanging = true;
					break;
				}
			}
		}
	}
	if (!isChanging)
	{
		if (assig->assigType != Type::Assigment)
		{
			elem->changing = true;
			elem->isSet = true;
		}
		else
		{
			auto expr = dynamic_cast<Expression*>(assig->list[0]);
			for (auto &it : expr->exprList)
			{
				if(it->type == Type::Identifier)
				{
					for (auto &it2 : list)
					{
						if (it2->var.name == it->value && it2->isSet && it2->changing)
						{
							elem->changing = elem->isSet = true;
							break;
						}
					}
				}
			}
		}

	}

	list.push_back(elem);
}

void Optymalizator::checkVar(Node *expr, std::vector<VarTab*> &list)
{
	Variable *var = dynamic_cast<Variable*>(expr);
	std::cout << "Wystapienie zmiennej: " << var->name << std::endl;

	bool isFound = false;
	if (var->crement != 0)
	{
		for (int i = 0; i < list.size(); ++i)
		{
			auto x = list[i]->var.name;
			if (list[i]->var.name == var->name)
			{
				list[i]->changing = list[i]->isSet = true;
				isFound = true;
				break;
			}
		}
		if (!isFound)
		{
			VarTab *elem = new VarTab(var);
			elem->changing = elem->isSet = true;
			list.push_back(elem);
			std::cout << "Dodano do listy" << std::endl;
		}

	}
}

void Optymalizator::checkDecl(Node *expr, std::vector<VarTab*> &list)
{
	Declaration *decl = dynamic_cast<Declaration*>(expr);
	std::cout << "Wystapienie zmiennej: " << decl->getVar()->name << std::endl;



}

void Optymalizator::saveToFile()
{
}


