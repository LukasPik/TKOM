// TKOM.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "main.h"
#include "Token.h"
#include "Parser.h"

int main()
{
	Lexer lexer("test.c");
	//Lexer lexer2("test.c");
	bool flag = true;
	Token token;
	int val = 0;
/*	while (flag)
	{
		//++val;
		token = lexer.nextToken();

		//std::cout <<"Main: "<< token.value << std::endl;
		if (token.value == "End")
			flag = false;

		if (val > 60)
			break;
	} */
	Parser parser(lexer);
	parser.parse();
	//parser.debug();
	std::cout << "Done" << std::endl;

	//while (true) {}
	int a;
	//std::cin >> a;
	return 0;

}

