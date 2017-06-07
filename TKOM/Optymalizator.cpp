#pragma once
#include "stdafx.h"
#include "Optymalizator.h"
#include <fstream>
#include <windows.h>

bool Optymalizator::toDelete(int x)
{
	for (auto it : deleteTab)
	{
		if (it == x)
			return true;
	}
	return false;
}

Optymalizator::Optymalizator(Program *init, std::string file)
{
	this->tree = init;
	this->file = file;
}



void Optymalizator::optymalize()
{
	size_t size = this->tree->list.size();
	std::vector<Node*> toInsert = {};
	for (int i = 0; i < size; ++i) 
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
				std::cout << "Nazwa assig " << assig->getVar()->name << std::endl;
				if (assig->getVar()->type != "")
				{
					std::cout << "Test";
					bool isNew = true;
					for (auto it2 : this->tree->varList)
					{
						std::cout << "Sprawdzamy z: "<< it2.var.name << std::endl;
						if (it2.var.name == assig->getVar()->name)
							isNew = false;
					}
					if (isNew)
					{
						std::cout << "##### insert decl to tree ##### ";
						assig->toAdd = true;
						this->tree->list.insert(this->tree->list.begin() + i, it);
						this->deleteFromChild(this->tree->list[i+1], assig->getVar()->name);
					}
				}
				else
				{
					std::cout << "##### insert assig to tree ##### ";
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
			//if (this->checkAssig(list[i], variables))
			//{
			//	std::cout << "Test push_back ";
			//	toReturn.push_back(list[i]);
		//	}
			Assignment *assig = dynamic_cast<Assignment*>(list[i]);
			VarTab *newElem = new VarTab(assig->getVar(), assig);
			variables.push_back(newElem);
			std::cout << "Dodajemy: " << newElem->assig->getAssig(0) << std::endl;

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
				std::cout << "Nazwa assig " << assig->getVar()->name << std::endl;
				if (assig->getVar()->type != "")
				{
					std::cout << "Test";
					bool isNew = true;
					for (auto it2 : variables)
					{
						if (it2->var.name == assig->getVar()->name)
							isNew = false;
					}
					if (isNew)
					{
						std::cout << "##### insert decl to tree ##### ";
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
					std::cout << "##### insert assig to tree ##### ";
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

	auto temp = this->checkAgain(list, variables);
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
			//if (this->checkAssig(list[i], variables))
			//{
			//	std::cout << "Test push_back ";
			//	toReturn.push_back(list[i]);
			//	}
			Assignment *assig = dynamic_cast<Assignment*>(list[i]);
			VarTab *newElem = new VarTab(assig->getVar(), assig);
			variables.push_back(newElem);
			std::cout << "Dodajemy: " << newElem->assig->getAssig(0) << std::endl;

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
				std::cout << "Nazwa assig " << assig->getVar()->name << std::endl;
				if (assig->getVar()->type != "")
				{
					std::cout << "Test";
					bool isNew = true;
					for (auto it2 : variables)
					{
						if (it2->var.name == assig->getVar()->name)
							isNew = false;
					}
					if (isNew)
					{
						std::cout << "##### insert decl to tree ##### ";
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
					std::cout << "##### insert assig to tree ##### ";
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

	auto temp = this->checkAgain(list, variables);
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
							elem->changing = it2->changing = true;
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


void Optymalizator::deleteFromChild(Node* elem, std::string toDelete)
{
	ForLoop *forElem = nullptr;
	WhileLoop *whileElem = nullptr;
	std::vector<Node*> *list = nullptr;

	std::cout << "Usuwawnie " << elem->getString();

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
				std::cout << "Dodajemy do tablicy: " << x << std::endl;
				//deleteTab.push_back(dynamic_cast<Expression*>(assig->list[0])->exprList[0]->line);
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
	std::string text;
	std::ios::streampos prevPos;
	int i = 0;
	bool wasAdded = false;
	getline(in, text);
	std::cout <<std::endl << text << std::endl;
	out << text << std::endl;
	getline(in, text);
	std::cout << text << std::endl;
	out << text << std::endl;
	getline(in, text);

	for (auto &it : tree->list)
	{
		it->getCode(out, 0);
	}
	std::cout << "\n}\n";
	out << "\n}\n";

/*	//while(getline(in, text))
	//{
	//	if ( text != "\n" && text != "" && i < tree->list.size())
	//	{

	//		if (this->tree->list[i]->getType() == Node::Type::ForLoop)
	//		{
	//			//std::cout << tekst << std::endl;
	//			///out << tekst << std::endl;
	//		//	getline(in, tekst);
	//			if (wasAdded)
	//			{
	//				in.seekg(prevPos);
	//				wasAdded = false;
	//			}
	//			else 
	//			{
	//				std::cout << text << std::endl;
	//				out << text << std::endl;
	//			}
	//			this->saveFor(in, out, this->tree->list[i], 1);
	//			getline(in, text);
	//		}
	//		else if (this->tree->list[i]->getType() == Node::Type::WhileLoop)
	//		{
	//			if (wasAdded)
	//			{
	//				in.seekg(prevPos);
	//				wasAdded = false;
	//			}
	//			else
	//			{
	//				std::cout << text << std::endl;
	//				out << text << std::endl;
	//			}
	//			this->saveWhile(in, out, this->tree->list[i], 1);
	//			getline(in, text);
	//		}
	//		else if (this->tree->list[i]->getType() == Node::Type::IfState)
	//		{
	//			std::cout << text << std::endl;
	//			out << text << std::endl;
	//			this->saveIf(in, out, this->tree->list[i]);
	//			getline(in, text);
	//		}
	//		else if (this->tree->list[i]->getType() == Node::Type::Assignment)
	//		{
	//			
	//			Assignment *assig = dynamic_cast<Assignment*>(tree->list[i]);
	//			if (assig->toAdd)
	//			{
	//				std::string temp = assig->getAssig(0);
	//				std::cout << temp << std::endl;
	//				out << temp << std::endl;
	//				if (this->tree->list[i + 1]->getType() != Node::Type::Assignment)
	//				{
	//					std::cout << text << std::endl;
	//					out << text << std::endl;	
	//					prevPos = in.tellg();
	//				}
	//				assig->toDelete = true;
	//				assig->toAdd = false;
	//				++i;
	//				wasAdded = true;
	//				continue;
	//			}
	//		}
	//		++i;
	//	}
	//	std::cout << text << std::endl;
	//	out << text << std::endl;
	//}*/
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
		{
			//typ (deklaracja + przypisanie)
			elem->isDecl = true;
		}
		auto expr = dynamic_cast<Expression*>(elem->assig->list[0]);
		bool isSetted = true;
		for (auto &it : expr->exprList)
		{
			if (it->type == Type::Identifier)
			{
				isSetted = false;
				for (auto &it2 : varList)
				{
					if (it2->var.name == it->value && it2->isSet)
					{
						elem->changing = it2->changing;
						elem->isSet = true;
						break;
					}
				}
			}
		}
		if (isSetted && !elem->isSet)
		{
			elem->isSet = true;
		}
	}

	return elem->isSet;
}

std::vector<Node *> Optymalizator::checkAgain(std::vector<Node*> list, std::vector<VarTab*>& varList)
{
/*	std::vector<Node *> toReturn = {};
	for (auto &it : list)
	{
		if (it->getType() == Node::Type::Assignment)
		{
			Assignment *assig = dynamic_cast<Assignment*>(it);
			for (auto &it2 : varList)
			{
				if (!it2->isSet)
				{
					std::cout << "repeat+++++++++++++++++++++" << std::endl;
					auto expr = dynamic_cast<Expression*>(assig->list[0]);
					for (auto &itExpr : expr->exprList)
					{
						if (itExpr->type == Type::Identifier)
						{
							for (auto &itVar : varList)
							{
								if (itVar->var.name == itExpr->value && it2->isSet) //&& it2->changing)
								{
									it2->changing = it2->isSet = true;
									break;
								}
							}
							
							break;
						}
					}
					it2->isSet = true;
					toReturn.push_back(it);
				}				
			}
		}
	}


	return toReturn; */

	bool isWork = true;
	while (isWork)
	{
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
		if (!it->changing)
			toReturn.push_back(it->assig);
	}


	return toReturn;
}

void Optymalizator::saveFor(std::ifstream &in, std::ofstream &out, Node *elem, int x)
{
	ForLoop *loop = dynamic_cast<ForLoop*>(elem);
	std::vector<Node*> list = loop->list;
	std::string text, temp;
	std::ios::streampos prevPos;
	//int  i = 0;

	getline(in, text);
	std::cout << text << std::endl;
	out << text << std::endl;

	bool wasDeleted = false;
	
	//for (auto &it : loop->list)
	for(int i = 0 ; i < loop->list.size(); )
	{
		getline(in, text);

		if (text != "\n" && text != "" && i < loop->list.size())
		{
			if (list[i]->getType() == Node::Type::ForLoop)
			{
				if (wasDeleted)
				{
					in.seekg(prevPos);
					wasDeleted = false;
					std::cout << temp << std::endl;
					out << temp << std::endl;
				}
				else
				{
					std::cout << text << std::endl;
					out << text << std::endl;
				}
				this->saveFor(in, out, list[i], x+1);
				getline(in, text);
			}
			else if(list[i]->getType() == Node::Type::WhileLoop)
			{
				std::cout << text << std::endl;
				out << text << std::endl;
				this->saveWhile(in, out, list[i], x+1);
				getline(in, text);
			}
			else if(list[i]->getType() == Node::Type::IfState)
			{
				std::cout << text << std::endl;
				out << text << std::endl;
				this->saveIf(in, out, list[i]);
				getline(in, text);
			}
			else if (list[i]->getType() == Node::Type::Assignment)
			{
				
				Assignment *assig = dynamic_cast<Assignment*>(list[i]);
				if (assig->toAdd) //&& !assig->toDelete)
				{
					std::string temp = assig->getAssig(x);
					std::cout << temp << std::endl;
					out << temp << std::endl;
					std::cout << text << std::endl;
					out << text << std::endl;
					assig->toDelete = true;
					assig->toAdd = false;
					++i;
					continue;
				}
				else if (assig->toDelete)
				{
					++i;
					wasDeleted = true;
					prevPos = in.tellg();
					//getline(in, text);
					temp = text;
					//std::cout << text << std::endl;
					//out << text << std::endl;
					continue;
				}
			}
			++i;
		}
		std::cout << text << std::endl;
		out << text << std::endl;


	}


}

void Optymalizator::saveWhile(std::ifstream &in, std::ofstream &out, Node *elem, int x)
{
	WhileLoop *loop = dynamic_cast<WhileLoop*>(elem);
	std::vector<Node*> list = loop->list;
	std::string text, temp;
	std::ios::streampos prevPos;
	int  i = 0;

	getline(in, text);
	std::cout << text << std::endl;
	out << text << std::endl;

	bool wasDeleted = false;

	for (auto &it : loop->list)
	{
		getline(in, text);

		if (text != "\n" && text != "" && i < loop->list.size())
		{
			if (list[i]->getType() == Node::Type::ForLoop)
			{
				if (wasDeleted)
				{
					in.seekg(prevPos);
					wasDeleted = false;
					std::cout << temp << std::endl;
					out << temp << std::endl;
				}
				else
				{
					std::cout << text << std::endl;
					out << text << std::endl;
				}
				this->saveFor(in, out, list[i], x + 1);
				getline(in, text);
			}
			else if (list[i]->getType() == Node::Type::WhileLoop)
			{
				std::cout << text << std::endl;
				out << text << std::endl;
				this->saveWhile(in, out, list[i], x + 1);
				getline(in, text);
			}
			else if (list[i]->getType() == Node::Type::IfState)
			{
				std::cout << text << std::endl;
				out << text << std::endl;
				this->saveIf(in, out, list[i]);
				getline(in, text);
			}
			else if (list[i]->getType() == Node::Type::Assignment)
			{

				Assignment *assig = dynamic_cast<Assignment*>(list[i]);
				if (assig->toAdd) //&& !assig->toDelete)
				{
					std::string temp = assig->getAssig(x);
					std::cout << temp << std::endl;
					out << temp << std::endl;
					std::cout << text << std::endl;
					out << text << std::endl;
					assig->toDelete = true;
					assig->toAdd = false;
					++i;
					continue;
				}
				else if (assig->toDelete)
				{
					++i;
					wasDeleted = true;
					prevPos = in.tellg();
					//getline(in, text);
					temp = text;
					//std::cout << text << std::endl;
					//out << text << std::endl;
					continue;
				}
			}
			++i;
		}
		std::cout << text << std::endl;
		out << text << std::endl;


	}

}

void Optymalizator::saveIf(std::ifstream &in, std::ofstream &out, Node *elem)
{
	IfStat *expr = dynamic_cast<IfStat*>(elem);
	std::string text;

	getline(in, text);
	std::cout << text << std::endl;
	out << text << std::endl;

	for (auto &it : expr->content)
	{
		getline(in, text);
		std::cout << text << std::endl;
		out << text << std::endl;
	}
	
}



