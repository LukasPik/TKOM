#pragma once
#include <iostream>

#include "stdafx.h"
#include "ErrorPrinter.h"
#include "Token.h"

#include <string>


void errorPrinter(std::string module, const Token &token, std::string info)
{
	std::string message = std::string("Error ")
		.append(module)
		.append(" : ")
		.append(info)
		.append(" line: ")
		.append(std::to_string(token.line))
		.append(" Pos: ")
		.append(std::to_string(token.pos));


	std::cout << "****************" << std::endl;
	std::cout << message << std::endl << "***************** " << std::endl;

}