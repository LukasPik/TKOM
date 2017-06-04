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
	std::vector<Node *> searchFor(std::vector<Node*> &list, Variable *);
	std::vector<Node *> searchWhile(std::vector<Node*> &list);
	void searchIf(std::vector<Node*> &list);
	bool checkAssig(Node *, std::vector<VarTab*> &);
	void checkVar(Node * expr, std::vector<VarTab*> &list);
	void checkDecl(Node *, std::vector<VarTab*> &);
	void deleteFromChild(std::vector<Node*>, std::string );
	void saveToFile();
	void checkAgain(std::vector<Node*> , std::vector<VarTab*> &);




};