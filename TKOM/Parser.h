#pragma once
#include <memory>
#include "data\node.h"
#include "Token.h"



class Lexer;

class Parser
{

public:

	Parser(Lexer &lexer);
	Program *parse();
	void debug();
private:
	Program *tree;
	Lexer &lexer;
	Token prevToken;
	Token act;

	void getMain(Node * );
	Node * parseLine();

	Node * parseType();
	Node * parseIdentifier();
	Node * parseExpression();
	Node * parseIfStatement();
	Node * parseCond();
	Node * parseSimpleCond();
	Node * parseCrementation();
	Node * parseFor();
	Node * parseWhile();



	bool Parser::isAcceptable(const Token& token, const std::initializer_list<Type>& acceptable) const;
	void nextToken();
	void errorHandler(std::string error, const Token &token);

	

};
