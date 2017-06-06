#pragma once
#include "data\node.h"
#include "Token.h"
#include <vector>;

class Optymalizator
{
private:
	std::string file;
	bool toDelete(int);
	std::vector<int> deleteTab;

public:
	Program *tree;

	Optymalizator(Program* , std::string);

	void optymalize();
	std::vector<Node *> searchFor(std::vector<Node*> &list, Variable *);
	std::vector<Node *> searchWhile(std::vector<Node*> &list);
	void searchIf(std::vector<Node*> &list);
	bool checkAssig(Node *, std::vector<VarTab*> &);
	void checkVar(Node * expr, std::vector<VarTab*> &list);
	void checkDecl(Node *, std::vector<VarTab*> &);
	void deleteFromChild(Node *, std::string );
	void saveToFile();
	std::vector<Node *> checkAgain(std::vector<Node*> , std::vector<VarTab*> &);

	void saveFor(std::ifstream &, std::ofstream &, Node *, int);
	void saveWhile(std::ifstream &, std::ofstream &, Node *, int);
	void saveIf(std::ifstream &, std::ofstream &, Node *);



};