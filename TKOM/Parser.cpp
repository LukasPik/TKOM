#pragma once
#include "stdafx.h"
#include "Parser.h"
#include "Lexer.h"
#include <iostream>




Parser::Parser(Lexer& lexer) : lexer(lexer)
{

}



Program* Parser::parse()
{
	Program* syntaxTree = new Program();

	this->getMain(syntaxTree); 

	Node * lastNode;

	while (lastNode = this->parseLine())
	{
		syntaxTree->addNode(lastNode);
		if (lastNode->getType() == Node::Type::Declaration )
		{
			VarTab var(dynamic_cast<Declaration*>(lastNode)->getVar());
			syntaxTree->addVar(var);
		}
		else if (lastNode->getType() == Node::Type::Assignment)
		{
			Assignment *assigNode = dynamic_cast<Assignment*>(lastNode);
			VarTab var(assigNode->getVar());
			syntaxTree->addVar(var);
		}
	}
	this->tree = syntaxTree;

	std::cout << "========== Tree ================" << std::endl;
	syntaxTree->showList(0);

	syntaxTree->showVarList();
	return syntaxTree;
}

void Parser::getMain(Node * tree)
{
	Token token = lexer.nextToken();
	Token token2;

	if (token.type != Type::Type)
		this->errorHandler("Type", token);

	token = lexer.nextToken();

	if (token.type != Type::Identifier)
		this->errorHandler("Identifier", token);

	token = lexer.nextToken();
	token2 = lexer.nextToken();
	if (token.type != Type::ParenthesisOpen)
		this->errorHandler("Parenthesis Open", token);
	if(token2.type != Type::ParenthesisClose)
		this->errorHandler("Parenthesis Close", token2);

	token = lexer.nextToken();
	if (token.type != Type::BracketOpen)
		this->errorHandler("Bracket Open", token);

}



Node * Parser::parseLine()
{
	Node * node = nullptr;

	nextToken();

	if (act.type == Type::EndOfFile)
	{
		return nullptr;
	}

	switch (act.type)
	{
	case Type::Identifier :
		node = parseIdentifier();		
		break;
	case Type::If:						
		node = parseIfStatement();
		break;

	case Type::Type :					
		node = parseType();
		break;

	case Type::Decrementation :
	case Type::Incrementation :
		node = parseCrementation();		
		break;

	case Type::For :				
		node = parseFor();
		break;

	case Type::While :					
		node = parseWhile();
		break;

	case Type::BracketClose :			
		return nullptr;
		break;
	default:
		this->errorHandler("One of instructions", act);
	}
	std::cout << node->getString() << std::endl;
	return node;
}


Node * Parser::parseType()
{
	Variable* node = new Variable();
	node->type = act.value;

	nextToken();
	if (act.type != Type::Identifier)
		this->errorHandler("Identifier", act);

	node->name = act.value;
	nextToken();
	if (act.type == Type::Semicolon)
	{
		Declaration* decl = new Declaration();
		decl->setVar(node);
		return decl;
	}
	else if (act.type == Type::Assigment)
	{
		Assignment * assig = new Assignment();
		assig->setVar(node);
		assig->assigType = act.type;
		assig->addNode(this->parseExpression());

		return assig;
	}
	else
		this->errorHandler("", act);

}

Node * Parser::parseIdentifier()
{
	Variable *node = new Variable();

	node->name = act.value;
	nextToken();
	if (isAcceptable(act, { Type::Assigment, Type::PlusEqual, Type::MinusEqual }))
	{
		Assignment *assig = new Assignment();
		assig->assigType = act.type;
		assig->setVar(node);
		assig->addNode(this->parseExpression());

		return assig;
	}
	else if (isAcceptable(act, { Type::Decrementation, Type::Incrementation }))
	{
		if (act.type == Type::Incrementation)
			node->crement = 1;
		else
			node->crement = 2;

		nextToken();
		if (act.type != Type::Semicolon)
		{
			this->errorHandler("Semicolon", act);
		}


		return node;
	}
	else
	{
		this->errorHandler("", act);
	}
}

Node * Parser::parseExpression()
{
	Expression *expr = new Expression();
	nextToken();
	if (act.type == Type::Semicolon)
	{
		this->errorHandler("Semicolon", act);
	}

	while (act.type != Type::Semicolon)
	{
		Token *token = new Token(act);
		expr->addExpr(token);
		nextToken();
	}

	return expr;
}

Node * Parser::parseIfStatement()
{
	IfStat *node = new IfStat();
	nextToken();
	if (act.type != Type::ParenthesisOpen)
	{
		this->errorHandler("Parenthesis Open", act);
	}

	Cond *cond;
	cond = dynamic_cast<Cond*>(this->parseCond());
	std::cout << cond->getString() << "          ";
	node->condition = cond;

	if (act.type != Type::ParenthesisClose)
	{
		this->errorHandler("Parenthesis Close", act);
	}


	nextToken();
	if (act.type != Type::BracketOpen)
	{
		this->errorHandler("Bracket open", act);
	}

	Node * lastNode;
	while (lastNode = this->parseLine())
	{
		node->addContent(lastNode);
	}

	return node;
}


Node* Parser::parseCond()
{
	Cond* cond = new Cond();
	nextToken();

	if (act.type == Type::ParenthesisOpen)
	{
		Node *node = parseCond();
		cond->addCond(node);

		if (act.type == Type::ParenthesisClose)
		{
			dynamic_cast<Cond*>(node)->inBrackets = true;
			nextToken();
			Node * last;
			if (act.type == Type::And || act.type == Type::Or)
			{
				Token *token = new Token(act);
				cond->addOperator(token);
				if (last = parseCond())
				{
					cond->addCond(last);
				}
				else
				{
					this->errorHandler("Condition after || or &&", act);
				}
			}
		}
		else
		{
			this->errorHandler("Parenthesis close", act);
		}

	}
	else if (isAcceptable(act, {Type::Identifier, Type::Number}))
	{
		Node *last = parseSimpleCond();
		cond->addCond(last);
		if(dynamic_cast<SimpleCond*>(last)->condOperator)
			nextToken();
		if (act.type == Type::And || act.type == Type::Or)
		{
			Token *token = new Token(act);
			cond->addOperator(token);
			if (last = parseCond())
			{
				cond->addCond(last);
			}
			else
			{
				this->errorHandler("Condition after || or &&", act);
			}
		}
		
	}

	return cond;

}

Node * Parser::parseSimpleCond()
{
	SimpleCond *cond = new SimpleCond();
	std::cout << "Started simple" << std::endl;
	if (act.type == Type::ParenthesisClose || act.type == Type::Semicolon)
	{
		return nullptr;
	}

	if (act.type == Type::ParenthesisOpen)
	{
		cond->inBrackets = true;
		nextToken();
	}

	if (isAcceptable(act, {Type::Number, Type::Identifier}))
	{
		cond->value1 = new Token(act);
		nextToken();
		if (isAcceptable(act, { Type::Greater, Type::Equality, Type::GreaterEqual, Type::Less, Type::LessEqual, Type::Inequality }))
		{
			cond->condOperator = new Token(act);
			nextToken();
			if (isAcceptable(act, { Type::Number, Type::Identifier }))
			{
				cond->value2 = new Token(act);
				if(cond->inBrackets)
					nextToken();

				if (cond->inBrackets && act.type == Type::ParenthesisClose)
				{
					nextToken();
				}
				else
					cond->inBrackets = false;
			}
		}
		else if (act.type != Type::ParenthesisClose)
		{
			this->errorHandler("condition statement", act);
		}		
	}

	return cond;
}

Node * Parser::parseCrementation()
{
	Variable *node = new Variable();
	nextToken();
	if (act.type == Type::Identifier)
	{
		node->name = act.value;
		if (prevToken.type == Type::Incrementation)
			node->crement = 1;
		else
			node->crement = 2;
		nextToken();
	
	}
	else
	{
		this->errorHandler("Identifier", act);
	}


	return node;
}

Node * Parser::parseFor()
{
	ForLoop *node = new ForLoop();

	nextToken();
	if (act.type != Type::ParenthesisOpen)
	{
		this->errorHandler("Parenthesis open", act);
	}
	nextToken();
	if (act.type != Type::Identifier)
	{
		this->errorHandler("Identifier", act);
	}
	Variable *var = new Variable();
	var->name = act.value;
	node->setLoopVar(var);


	nextToken();
	if (act.type != Type::Semicolon)
	{
		this->errorHandler("Semicolon", act);
	}

	Cond *cond = dynamic_cast<Cond*>(parseCond());
	node->setLoopCond(cond);

	if (act.type != Type::Semicolon)
	{
		this->errorHandler("Semicolon", act);
	}
	nextToken();

	if (isAcceptable(act, { Type::Incrementation, Type::Decrementation }))
	{
		Node *expr = parseCrementation();
		node->setEndExpr(expr);
	}
	else if (act.type == Type::Identifier)
	{
		node->setEndExpr(parseIdentifier());
	}
	else
	{
		this->errorHandler("End expression of loop", act);
	}

	if (act.type != Type::ParenthesisClose)
	{
		this->errorHandler("Parenthesis open", act);
	}
	nextToken();
	if (act.type != Type::BracketOpen)
	{
		this->errorHandler("Bracket open", act);
	}

	Node *last;
	while (last = parseLine())
	{
		node->addNode(last);

	}

	return node;
}

Node * Parser::parseWhile()
{
	WhileLoop *node = new WhileLoop();
	nextToken();
	if (act.type != Type::ParenthesisOpen)
	{
		this->errorHandler("Parenthesis open", act);
	}

	node->condition = dynamic_cast<Cond*>(parseCond());

	if (act.type != Type::ParenthesisClose)
	{
		this->errorHandler("Parenthesis close", act);
	}
	nextToken();
	if (act.type != Type::BracketOpen)
	{
		this->errorHandler("Bracket open", act);
	}

	Node *last;
	while (last = parseLine())
	{
		node->addNode(last);
	}

	return node;
}


void Parser::nextToken()
{
	prevToken = act;
	act = lexer.nextToken();
}

void Parser::errorHandler(std::string error, const Token & token)
{
	std::cout << "In line: " << token.line << " position: " << token.pos << std::endl;
	std::cout << "Unexpected token: " << getTypeName(token.type) << ". Expected: " << error << std::endl;
	exit(0);
}


bool Parser::isAcceptable(const Token& token, const std::initializer_list<Type>& acceptable) const
{
	for (auto& it : acceptable)
	{
		if (token.type == it)
		{
			return true;
		}
	}

	return false;
}


void Parser::debug()
{
	std::cout << std::endl;
	for (auto &it : tree->list)
	{
		std::cout << "[ " << it->getString() << " ]" << std::endl;
	}
}

Program * Parser::getTree()
{
	return this->tree;
}
