// TKOM.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "main.h"
#include "Token.h"
#include "Parser.h"
#include "Optymalizator.h"

int main()
{
	Lexer lexer("test.c");
	//Lexer lexer2("test.c");
	bool flag = true;
	Token token;

	Parser parser(lexer);
	parser.parse();
	//parser.debug();
	Optymalizator opt(parser.getTree());
	opt.optymalize();



	std::cout << "Done" << std::endl;

	return 0;

}

