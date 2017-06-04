#pragma once
#include "data\node.h"
#include "Token.h"
#include <vector>;

class Optymalizator
{
public:
	Program *tree;

	Optymalizator(Program* );

	void optymalize();
	void searchFor(std::vector<Node*> list, Variable *);
	void searchWhile(std::vector<Node*> list);
	void searchIf(std::vector<Node*> list);
	void checkAssig(Node *, std::vector<VarTab*> &);
	void checkVar(Node * expr, std::vector<VarTab*> &list);
	void checkDecl(Node *, std::vector<VarTab*> &);
	void saveToFile();




};