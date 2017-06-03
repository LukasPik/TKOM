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
	void searchFor(std::vector<Node*> list);
	void searchWhile(std::vector<Node*> list);
	void searchIf(std::vector<Node*> list);
	void checkMutability(Node *, std::vector<varTab*> *);
	void saveToFile();



};