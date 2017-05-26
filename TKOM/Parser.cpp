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

	this->getMain(syntaxTree);  // chyba do usuniecia prametr wywolania

	Node * lastNode;

	while (lastNode = this->parseLine())
	{
		syntaxTree->addNode(lastNode);
		if (lastNode->getType() == Node::Type::Declaration )
		{
			varTab var(dynamic_cast<Declaration*>(lastNode)->getVar());
			syntaxTree->addVar(var);
		}
		else if (lastNode->getType() == Node::Type::Assignment)
		{
			Assignment *assigNode = dynamic_cast<Assignment*>(lastNode);
			if (assigNode->getVar()->type != "")
			{
				varTab var(assigNode->getVar());
				syntaxTree->addVar(var);
			}
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
	{
		//// error
	}
	token = lexer.nextToken();

	if (token.type != Type::Identifier)
	{
		//error
	}
	token = lexer.nextToken();
	token2 = lexer.nextToken();
	if (token.type != Type::ParenthesisOpen && token2.type != Type::ParenthesisClose)
	{
		//error
	}

	token = lexer.nextToken();
	if (token.type != Type::BracketOpen)
	{
		//error
	}



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
		node = parseIdentifier();		//dziala
		break;
	case Type::If:						//dziala
		node = parseIfStatement();
		break;

	case Type::Type :					// dziala
		node = parseType();
		break;

	case Type::Decrementation :
	case Type::Incrementation :
		node = parseCrementation();		//dziala
		break;

	case Type::For :					// dziala
		node = parseFor();
		break;

	case Type::While :					// dziala
		node = parseWhile();
		break;

	case Type::BracketClose :			// dziala
		//nextToken();
		return nullptr;
		break;
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
	{
		//error
	}

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
		assig->addNode(this->parseExpression());

		return assig;
	}
	else
	{
		//error unexpected token
	}

}

Node * Parser::parseIdentifier()
{
	Variable *node = new Variable();

	node->name = act.value;
	nextToken();
	if (isAcceptable(act, { Type::Assigment, Type::PlusEqual, Type::MinusEqual }))
	{
		Assignment *assig = new Assignment();
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
			//blad
		}


		return node;
	}
	else
	{
		//error unexpected token
	}
}

Node * Parser::parseExpression()
{
	Expression *expr = new Expression();
	nextToken();
	if (act.type == Type::Semicolon)
	{
		// error unexpected token
	}

	while (act.type != Type::Semicolon)
	{
		Token *token = new Token(act);
		// need validity check
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
		//expected Parenthesis Open
	}

	Cond *cond;
	cond = dynamic_cast<Cond*>(this->parseCond());
	std::cout << cond->getString() << "          ";
	node->condition = cond;

	//nextToken();
	if (act.type != Type::ParenthesisClose)
	{
		//expected Parenthesis Open
	}


	nextToken();
	if (act.type != Type::BracketOpen)
	{
		// error expected {
	}

	Node * lastNode;
	while (lastNode = this->parseLine())
	{
		node->addContent(lastNode);
	}

	return node;
}

/*Node * Parser::parseCond()
{
	Cond *cond = new Cond();
	nextToken();
	if (act.type == Type::ParenthesisClose)
	{
		//error no condition
	}
	SimpleCond *last;

	if (act.type == Type::ParenthesisOpen)
	{
		Node *node = parseCond();
		nextToken();
		if (act.type == Type::ParenthesisClose)
		{
			dynamic_cast<Cond*>(node)->inBrackets = true;

		}
		else
		{
			// no parent close
		}
		//cond->inBrackets = true;
	}
	

	while (last = dynamic_cast<SimpleCond*>(this->parseSimpleCond()))
	{
		cond->addCond(last);
		if (cond->inBrackets && !last->inBrackets)
		{

		}
		//nextToken();
		if (act.type == Type::And || act.type == Type::Or)
		{
			Token *token = new Token(act);
			cond->addOperator(token);
			nextToken();
			if (act.type == Type::ParenthesisOpen)
			{
				Node *node = parseCond();
				if (act.type == Type::ParenthesisClose)
				{
					dynamic_cast<Cond*>(node)->inBrackets = true;
					cond->addCond(node);
				}
				else
				{
					// no parent close
				}
				//cond->inBrackets = true;
			}

		}
		else
			break;
	}
	
	return cond;
}
*/


Node* Parser::parseCond()
{
	Cond* cond = new Cond();
	nextToken();

	if (act.type == Type::ParenthesisOpen)
	{
		Node *node = parseCond();
		cond->addCond(node);

		//nextToken();
		if (act.type == Type::ParenthesisClose)
		{
			dynamic_cast<Cond*>(node)->inBrackets = true;
			nextToken();
			Node * last;
			if (act.type == Type::And || act.type == Type::Or)
			{
				Token *token = new Token(act);
				cond->addOperator(token);
				//nextToken();
				if (last = parseCond())
				{
					cond->addCond(last);
				}
				else
				{
					//missing condition after and, or
				}
			}
		}
		else
		{
			//missing parent close
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
			//nextToken();
			if (last = parseCond())
			{
				cond->addCond(last);
			}
			else
			{
				//missing condition after and, or
			}
		}
		
	}

	return cond;

}

Node * Parser::parseSimpleCond()
{
	SimpleCond *cond = new SimpleCond();
	std::cout << "Started simple" << std::endl;
	//nextToken();
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
		cond->value1 = &act;//new Token(act);
		nextToken();
		if (isAcceptable(act, { Type::Greater, Type::Equality, Type::GreaterEqual, Type::Less, Type::LessEqual, Type::Inequality }))
		{
			cond->condOperator = &act;//new Token(act);
			nextToken();
			if (isAcceptable(act, { Type::Number, Type::Identifier }))
			{
				cond->value2 = &act;//new Token>(act);
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
		else if (cond->inBrackets && act.type == Type::ParenthesisClose)
		{
			//nextToken();
		}
		else
		{
			//unexpected token
		}		
	}
	std::cout << "Created simple" << std::endl;

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
		if (act.type != Type::Semicolon)
		{
			// blad
		}
	}
	else
	{
		//expected Identifier;
	}


	return node;
}

Node * Parser::parseFor()
{
	ForLoop *node = new ForLoop();

	nextToken();
	if (act.type != Type::ParenthesisOpen)
	{
		//expected parent open
	}
	nextToken();
	if (act.type != Type::Identifier)
	{
		//expected parent open
	}
	Variable *var = new Variable();
	var->name = act.value;
	node->setLoopVar(var);


	nextToken();
	if (act.type != Type::Semicolon)
	{
		//expected parent open
	}

	Cond *cond = dynamic_cast<Cond*>(parseCond());
	node->setLoopCond(cond);

	//nextToken();
	if (act.type != Type::Semicolon)
	{
		//expected parent open
	}
	nextToken();

//	Expression *expr = dynamic_cast<Expression*>(parseExpression()); //? chyba co innego trzeba. lub to zmienic
//	node->setEndExpr(expr);
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
		//error
	}

	//nextToken();
	if (act.type != Type::ParenthesisClose)
	{
		//expected parent open
	}
	nextToken();
	if (act.type != Type::BracketOpen)
	{
		//expected parent open
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
		//expected parent open
	}
	//nextToken();

	node->condition = dynamic_cast<Cond*>(parseCond());

	//nextToken();
	if (act.type != Type::ParenthesisClose)
	{
		//expected parent open
	}
	nextToken();
	if (act.type != Type::BracketOpen)
	{
		//expected parent open
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
	//errorPrinter("Parser", token, error);


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