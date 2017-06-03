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
			this->searchFor(dynamic_cast<ForLoop*>(this->tree->list[i])->list);
		}
		else if (this->tree->list[i]->getType() == Node::Type::WhileLoop)
		{
			this->searchFor(dynamic_cast<WhileLoop*>(this->tree->list[i])->list);
		}
		else if (this->tree->list[i]->getType() == Node::Type::IfState)
		{
			this->searchFor(dynamic_cast<IfStat*>(this->tree->list[i])->content);
		}
	}

}

void Optymalizator::searchFor(std::vector<Node*> list)
{
	size_t size = list.size();
	std::vector<varTab*> variables;
	for (int i = 0; i < size; ++i)
	{
		if (list[i]->getType() == Node::Type::ForLoop)
		{
			this->searchFor(dynamic_cast<ForLoop*>(list[i])->list);
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
			this->checkMutability(list[i], &variables);
		}
	}
}

void Optymalizator::searchWhile(std::vector<Node*> list)
{
	size_t size = list.size();
	std::vector<varTab*> variables;
	for (int i = 0; i < size; ++i)
	{
		if (list[i]->getType() == Node::Type::ForLoop)
		{
			this->searchFor(dynamic_cast<ForLoop*>(list[i])->list);
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
			this->checkMutability(list[i], &variables);
		}
	}
}

void Optymalizator::searchIf(std::vector<Node*> list)
{
	size_t size = list.size();
	for (int i = 0; i < size; ++i)
	{
		if (list[i]->getType() == Node::Type::ForLoop)
		{
			this->searchFor(dynamic_cast<ForLoop*>(list[i])->list);
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

void Optymalizator::checkMutability(Node *expr, std::vector<varTab*> *list)
{
	Assignment *assig = dynamic_cast<Assignment*>(expr);
	std::cout << "Wystapienie zmiennej: " << assig->getVar()->name << std::endl;
}

void Optymalizator::saveToFile()
{
}
