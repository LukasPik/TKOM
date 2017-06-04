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
		std::vector<Node*> toInsert = {};
		if (this->tree->list[i]->getType() == Node::Type::ForLoop)
		{
			auto pass = dynamic_cast<ForLoop*>(this->tree->list[i]);
			toInsert = this->searchFor(pass->list, pass->getLoopVar());
		}
		else if (this->tree->list[i]->getType() == Node::Type::WhileLoop)
		{
			toInsert = this->searchWhile(dynamic_cast<WhileLoop*>(this->tree->list[i])->list);
		}
		else if (this->tree->list[i]->getType() == Node::Type::IfState)
		{
			this->searchIf(dynamic_cast<IfStat*>(this->tree->list[i])->content);
		}
		for (auto &it : toInsert)
		{
			if (it->getType() == Node::Type::Assignment)
			{
				Assignment *assig = dynamic_cast<Assignment*>(it);
				std::cout << "Nazwa assig " << assig->getVar()->name << std::endl;
				if (assig->getVar()->type != "")
				{
					std::cout << "Test";
					bool isNew = false;
					for (auto it2 : this->tree->varList)
					{
						if (it2.var.name == assig->getVar()->name)
						{
							isNew = true;
						}
					}
					if (isNew)
					{
						std::cout << "##### insert to tree ##### ";
						this->tree->list.insert(this->tree->list.begin() + i, it);
						//this->deleteFromChild();
					}
				}
				else
				{
					this->tree->list.insert(this->tree->list.begin() + i, it);
				}

			}
		}
	}

}

std::vector<Node *> Optymalizator::searchFor(std::vector<Node*> &list, Variable *loopVar)
{
	size_t size = list.size();
	std::vector<VarTab*> variables;
	std::vector<Node*> toReturn = {};
	VarTab *elem = new VarTab(loopVar);
	elem->changing = elem->isSet = true;
	variables.push_back(elem);

	for (int i = 0; i < size; ++i)
	{
		std::vector<Node*> toInsert = {};
		if (list[i]->getType() == Node::Type::ForLoop)
		{
			auto pass = dynamic_cast<ForLoop*>(this->tree->list[i]);
			toInsert = this->searchFor(pass->list, pass->getLoopVar());
		}
		else if (list[i]->getType() == Node::Type::WhileLoop)
		{
			toInsert = this->searchWhile(dynamic_cast<WhileLoop*>(list[i])->list);
		}
		else if (list[i]->getType() == Node::Type::IfState)
		{
			this->searchIf(dynamic_cast<IfStat*>(list[i])->content);
		}
		else if (list[i]->getType() == Node::Type::Assignment)
		{
			if (this->checkAssig(list[i], variables))
			{
				std::cout << "Test push_back ";
				toReturn.push_back(list[i]);
			}
		}
		else if (list[i]->getType() == Node::Type::Variable)
		{
			this->checkVar(list[i], variables);
		}

		for (auto &it : toInsert)
		{
			if (it->getType() == Node::Type::Assignment)
			{
				Assignment *assig = dynamic_cast<Assignment*>(it);
				if (assig->getVar()->type != "")
				{
					bool isNew = false;
					for (auto &it2 : variables)
					{
						if (it2->var.name == assig->getVar()->name)
						{
							isNew = true;
						}
					}
					if (isNew)
					{
						list.insert(list.begin() + i , it);
						//this->deleteFromChild();
					}
				}

			}
		}
	}

	this->checkAgain(list, variables);

	std::cout << "===============================" << std::endl;
	for (auto &it : variables)
	{
		if (it->changing)
			std::cout << "!";

		std::cout << "Zawartosc listy: " << it->var.name << std::endl;
	}
	
	return toReturn;
}

std::vector<Node *> Optymalizator::searchWhile(std::vector<Node*> &list)
{
	size_t size = list.size();
	std::vector<VarTab*> variables;
	std::vector<Node*> toReturn = {};

	for (int i = 0; i < size; ++i)
	{
		std::vector<Node*> toInsert = {};
		if (list[i]->getType() == Node::Type::ForLoop)
		{
			auto pass = dynamic_cast<ForLoop*>(this->tree->list[i]);
			toInsert = this->searchFor(pass->list, pass->getLoopVar());
		}
		else if (list[i]->getType() == Node::Type::WhileLoop)
		{
			toInsert = this->searchWhile(dynamic_cast<WhileLoop*>(list[i])->list);
		}
		else if (list[i]->getType() == Node::Type::IfState)
		{
			this->searchIf(dynamic_cast<IfStat*>(list[i])->content);
		}
		else if (list[i]->getType() == Node::Type::Assignment)
		{
			if (this->checkAssig(list[i], variables))
				toReturn.push_back(list[i]);
		}
		else if (list[i]->getType() == Node::Type::Variable)
		{
			this->checkVar(list[i], variables);
		}


		for (auto &it : toInsert)
		{
			if (it->getType() == Node::Type::Assignment)
			{
				Assignment *assig = dynamic_cast<Assignment*>(it);
				if (assig->getVar()->type != "")
				{
					bool isNew = false;
					for (auto &it2 : variables)
					{
						if (it2->var.name == assig->getVar()->name)
						{
							isNew = true;
						}
					}
					if (isNew)
					{
						list.insert(list.begin() + i, it);
						//this->deleteFromChild();
					}
				}

			}
		}
	}

	



	std::cout << "===============================" << std::endl;
	for (auto &it : variables)
	{
		if (it->changing)
			std::cout << "!";

		std::cout << "Zawartosc listy: " << it->var.name << std::endl;
	}






	return toReturn;
}

void Optymalizator::searchIf(std::vector<Node*> &list)
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

bool Optymalizator::checkAssig(Node *expr, std::vector<VarTab*> &list)
{
	Assignment *assig = dynamic_cast<Assignment*>(expr);
	//std::cout << "Wystapienie przypisania: " << assig->getVar()->name << std::endl;
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
				if (!it->changing)
				{
					it->isSet = it->changing = true;
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
				bool isSetted = true;
				if(it->type == Type::Identifier)
				{
					isSetted = false;
					for (auto &it2 : list)
					{
						if (it2->var.name == it->value && it2->isSet )
						{
							elem->changing = it2->changing;
							elem->isSet = true;
							break;
						}
					}
				}
				if (isSetted && !elem->isSet)
				{
					elem->isSet = true;
				}
			}
		}
		list.push_back(elem);
	}
	bool dec = false;
	if (!elem->changing && elem->isSet)
		dec = true;

	return dec;
}

void Optymalizator::checkVar(Node *expr, std::vector<VarTab*> &list)
{
	Variable *var = dynamic_cast<Variable*>(expr);
	//std::cout << "Wystapienie zmiennej: " << var->name << std::endl;

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
			//std::cout << "Dodano do listy" << std::endl;
		}

	}
}


void Optymalizator::deleteFromChild(std::vector<Node*>, std::string)
{
}

void Optymalizator::saveToFile()
{
}

void Optymalizator::checkAgain(std::vector<Node*> list, std::vector<VarTab*>& varList)
{
	for (auto &it : list)
	{
		if (it->getType() == Node::Type::Assignment)
		{
			Assignment *assig = dynamic_cast<Assignment*>(it);
			for (auto &it2 : varList)
			{
				if (!it2->isSet)
				{
					auto expr = dynamic_cast<Expression*>(assig->list[0]);
					for (auto &itExpr : expr->exprList)
					{
						if (itExpr->type == Type::Identifier)
						{
							for (auto &itVar : varList)
							{
								if (itVar->var.name == itExpr->value && it2->isSet && it2->changing)
								{
									it2->changing = it2->isSet = true;
									break;
								}
							}
							break;
						}
					}
					it2->isSet = true;
				}
			}
		}
	}



}


