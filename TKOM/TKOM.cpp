// TKOM.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "main.h"
#include "Token.h"
#include "Parser.h"
#include "Optymalizator.h"

int main()
{
	std::string file;
	std::cout << "Podaj nazwe pliku: " << std::endl;
	std::cin >> file;

	Lexer lexer(file);
	//Lexer lexer2("test.c");
	bool flag = true;
	Token token;

	Parser parser(lexer);
	parser.parse();
	//parser.debug();
	Optymalizator opt(parser.getTree(), file);
	opt.optymalize();
	std::cout << "=============Wyjsciowe drzewo==================" << std::endl;
	opt.tree->showList(0);



	std::cout << "Done" << std::endl;

	return 0;

}

