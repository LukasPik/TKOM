#pragma once
#include "stdafx.h"
#include "Optymalizator.h"
#include <fstream>
#include <windows.h>

Optymalizator::Optymalizator(Program *init, std::string file)
{
	this->tree = init;
	this->file = file;
}



void Optymalizator::optymalize()
{
	std::vector<Node*> toInsert = {};
	for (int i = 0; i < this->tree->list.size(); ++i)
	{
		toInsert.clear();
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
				if (assig->getVar()->type != "")
				{
					bool isNew = true;
					for (auto it2 : this->tree->varList)
					{
						if (it2.var.name == assig->getVar()->name)
							isNew = false;
					}
					if (isNew)
					{;
						assig->toAdd = true;
						this->tree->list.insert(this->tree->list.begin() + i, it);
						this->deleteFromChild(this->tree->list[i+1], assig->getVar()->name);
					}
				}
				else
				{
					assig->toAdd = true;
					this->tree->list.insert(this->tree->list.begin() + i, it);
					this->deleteFromChild(this->tree->list[i+1], assig->getVar()->name);
				}

			}
			++i;
		}
	}
	this->saveToFile();
}

std::vector<Node *> Optymalizator::searchFor(std::vector<Node*> &list, Variable *loopVar)
{
	std::vector<VarTab*> variables;
	std::vector<Node*> toReturn = {};
	VarTab *elem = new VarTab(loopVar);
	elem->changing = elem->isSet = true;
	variables.push_back(elem);

	for (int i = 0; i < list.size(); ++i)
	{
		std::vector<Node*> toInsert = {};
		if (list[i]->getType() == Node::Type::ForLoop)
		{
			auto pass = dynamic_cast<ForLoop*>(list[i]);
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
			Assignment *assig = dynamic_cast<Assignment*>(list[i]);
			VarTab *newElem = new VarTab(assig->getVar(), assig);
			variables.push_back(newElem);
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
					bool isNew = true;
					for (auto it2 : variables)
					{
						if (it2->var.name == assig->getVar()->name)
							isNew = false;
					}
					if (isNew)
					{
						assig->toAdd = true;
						VarTab *temp = new VarTab(assig->getVar(), assig);
						temp->isDecl = true;
						variables.push_back(temp);
						list.insert(list.begin() + i, it);
						this->deleteFromChild(list[i + 1], assig->getVar()->name);
					}
					else
						continue;
				}
				else
				{
					assig->toAdd = true;
					VarTab *temp = new VarTab(assig->getVar(), assig);
					variables.push_back(temp);
					list.insert(list.begin() + i, it);
					this->deleteFromChild(list[i +1], assig->getVar()->name);
				}

			}
			++i;
		}
	}

	auto temp = this->checkChangeStatus(list, variables);
	for (auto &x : temp)
	{
		toReturn.push_back(x);
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

std::vector<Node *> Optymalizator::searchWhile(std::vector<Node*> &list)
{
	size_t size = list.size();
	std::vector<VarTab*> variables;
	std::vector<Node*> toReturn = {};

	for (int i = 0; i < list.size(); ++i)
	{
		std::vector<Node*> toInsert = {};
		if (list[i]->getType() == Node::Type::ForLoop)
		{
			auto pass = dynamic_cast<ForLoop*>(list[i]);
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
			Assignment *assig = dynamic_cast<Assignment*>(list[i]);
			VarTab *newElem = new VarTab(assig->getVar(), assig);
			variables.push_back(newElem);
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
					bool isNew = true;
					for (auto it2 : variables)
					{
						if (it2->var.name == assig->getVar()->name)
							isNew = false;
					}
					if (isNew)
					{
						assig->toAdd = true;
						VarTab *temp = new VarTab(assig->getVar(), assig);
						temp->isDecl = true;
						variables.push_back(temp);
						list.insert(list.begin() + i, it);
						this->deleteFromChild(list[i + 1], assig->getVar()->name);
					}
					else
						continue;
				}
				else
				{
					assig->toAdd = true;
					VarTab *temp = new VarTab(assig->getVar(), assig);
					variables.push_back(temp);
					list.insert(list.begin() + i, it);
					this->deleteFromChild(list[i + 1], assig->getVar()->name);
				}

			}
			++i;
		}
	}

	auto temp = this->checkChangeStatus(list, variables);
	for (auto &x : temp)
	{
		toReturn.push_back(x);
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

void Optymalizator::checkVar(Node *expr, std::vector<VarTab*> &list)
{
	Variable *var = dynamic_cast<Variable*>(expr);

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
		}

	}
}


void Optymalizator::deleteFromChild(Node* elem, std::string toDelete)
{
	ForLoop *forElem = nullptr;
	WhileLoop *whileElem = nullptr;
	std::vector<Node*> *list = nullptr;

	if (elem->getType() == Node::Type::ForLoop)
	{
		forElem = dynamic_cast<ForLoop*>(elem);
		list = &forElem->list;
	}
	else if (elem->getType() == Node::Type::WhileLoop)
	{
		whileElem = dynamic_cast<WhileLoop*>(elem);
		list = &whileElem->list;
	}
	if (list == nullptr)
		return;

	for (int i = 0; i < list->size(); ++i)
	{
		if (list->at(i)->getType() == Node::Type::Assignment)
		{
			Assignment *assig = dynamic_cast<Assignment*>(list->at(i));
			if (assig->getVar()->name == toDelete)
			{
				assig->toDelete = true;
				int x = dynamic_cast<Expression*>(assig->list[0])->exprList[0]->line;
				list->erase(list->begin() + i);
			}
		}
	}
}

void Optymalizator::saveToFile()
{
	std::ifstream in(file);
	std::string outFile = "out-";
	std::ofstream out(outFile.append(file));

	this->tree->getCode(out, 0);


	in.close();
	out.close();
}

bool Optymalizator::checkSingle(VarTab *elem, std::vector<VarTab*>& varList)
{
	if (elem->assig->assigType != Type::Assigment)
	{
		elem->changing = true;
		elem->isSet = true;
	}
	else
	{
		if (elem->var.type != "")
			elem->isDecl = true;

		auto expr = dynamic_cast<Expression*>(elem->assig->list[0]);
		bool isSetted = true;
		for (auto &it : expr->exprList)
		{
			if (it->type == Type::Identifier)
			{
				isSetted = false;
				bool isInTable = false;
				for (auto &it2 : varList)
				{
					if (it2->var.name == it->value && it2->isSet)
					{
						if(!elem->changing)
							elem->changing = it2->changing;

						elem->isSet = true;
					}
					else if (it2->var.name == it->value)
					{
						isInTable = true;
						elem->isSet = false;
					}
				}
				if (!elem->isSet && !isInTable)
					elem->isSet = true;

			}
		}
		if (isSetted && !elem->isSet)
		{
			elem->isSet = true;
		}
	}

	return elem->isSet;
}

std::vector<Node *> Optymalizator::checkChangeStatus(std::vector<Node*> list, std::vector<VarTab*>& varList)
{
	bool isWork = true;
	int i = 0;
	

	while (isWork && i < 50)
	{	
		++i;
		for (int i = 0 ; i < varList.size() ; ++i)
		{
			if (varList[i]->isSet)
				continue;

			auto temp = varList[i];
			varList.erase(varList.begin() + i);
			for (auto it2 : varList)
			{
				if (temp->var.name == it2->var.name)
				{
					temp->changing = temp->isSet = it2->changing = it2->isSet = true;
					break;
				}
			}
			if (!temp->isSet)
				this->checkSingle(temp, varList);

			varList.insert(varList.begin() + i, temp);
		}
		isWork = false;
		for (auto it : varList)
		{
			if (!it->isSet)
				isWork = true;
		}
	}
	
	std::vector<Node*> toReturn = {};
	for (auto it : varList)
	{
		if (!it->changing && it->isSet)
			toReturn.push_back(it->assig);
	}


	return toReturn;
}
