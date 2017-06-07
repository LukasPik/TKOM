#pragma once
#include "data\node.h"
#include "Token.h"
#include <vector>;

class Optymalizator
{
private:
	std::string file;

public:
	Program *tree;

	Optymalizator(Program* , std::string);

	void optymalize();
	std::vector<Node *> searchFor(std::vector<Node*> &list, Variable *);
	std::vector<Node *> searchWhile(std::vector<Node*> &list);
	void searchIf(std::vector<Node*> &list);
	void checkVar(Node * expr, std::vector<VarTab*> &list);
	void deleteFromChild(Node *, std::string );
	void saveToFile();
	bool checkSingle(VarTab *, std::vector<VarTab*> &);
	std::vector<Node *> checkChangeStatus(std::vector<Node*> , std::vector<VarTab*> &);

};