
#include "stdafx.h"
#include <cctype>
#include <algorithm>
#include <iostream>
#include "Lexer.h"
#include "Token.h"
#include "Constants.h"
#include "ErrorPrinter.h"

using namespace std;

const Token& Lexer::errorHandler(string error, const Token &token)
{
	errorPrinter("Lexer", token, error);
	return this->nextToken();
}

Lexer::Lexer(const std::string& file): reader(file)
{

}



Token Lexer::nextToken()
{
	Token token;
	char nxtChar;
	auto sign = this->reader.nextChar();

	while (isspace(sign))
	{
		sign = this->reader.nextChar();
	}

	token.line = this->reader.lineNumber;
	token.pos = this->reader.currentSignPos - 1;
	token.lineStart = this->reader.currentLinePos;


	if (this->reader.endOfFile())
	{
		cout << "koniec pliku";
		token.type = Type::EndOfFile;
		token.value = "End";
		return token;
	}

	if (isalpha(sign))			//alphabetical characters
	{
		std::string buffer;

		do
		{
			buffer.push_back(sign);
			sign = this->reader.nextChar();
		} while (isalnum(sign));

		this->reader.rewind();
		auto tempBuf = buffer;
		std::transform(tempBuf.begin(), tempBuf.end(), tempBuf.begin(), ::tolower);

		if (keyword.count(tempBuf) == 1)
		{
			token.type = keyword.at(tempBuf);
			if (token.type == Type::Type)
				token.value = buffer;
		}
		else
		{
			token.type = Type::Identifier;
			token.value = buffer;
		}
	}
	else if (isdigit(sign))		// numbers
	{
		std::string buffer;

		do
		{
			buffer.push_back(sign);
			sign = this->reader.nextChar();
		} while (isdigit(sign) || sign == '.');

		this->reader.rewind();
		token.type = Type::Number;
		token.value = buffer;
	}
	else						// special chars
	{
		switch (sign)
		{
		case '&' :
			if (this->reader.nextChar() == '&')
				token.type = Type::And;
			else
			{
				this->reader.rewind();
				token.type = Type::Invalid;
				token = errorHandler("Unexpected symbol", token);
			}
			break;
		case '|':
			if (this->reader.nextChar() == '|')
				token.type = Type::Or;
			else
			{
				this->reader.rewind();
				token.type = Type::Invalid;
				token = errorHandler("Unexpected symbol", token);
			}
			break;
		case '<':
			if (this->reader.nextChar() == '=')
				token.type = Type::LessEqual;
			else
			{
				this->reader.rewind();
				token.type = Type::Less;
			}
			break;
		case '>':
			if (this->reader.nextChar() == '=')
				token.type = Type::GreaterEqual;
			else
			{
				this->reader.rewind();
				token.type = Type::Greater;
			}
			break;

		case '=':
			if (this->reader.nextChar() == '=')
				token.type = Type::Equality;
			else
			{
				this->reader.rewind();
				token.type = Type::Assigment;
			}
			break;
		case '!':
			if (this->reader.nextChar() == '=')
				token.type = Type::Inequality;
			else
			{
				this->reader.rewind();
				token.type = Type::Invalid;
			}
			break;
		case '+':
			nxtChar = this->reader.nextChar();
			if (nxtChar == '+')
				token.type = Type::Incrementation;
			else if (nxtChar == '=')
				token.type = Type::PlusEqual;						
			else
			{
				token.type = Type::Plus;
				this->reader.rewind();
			}
			break;
		case '-':
			nxtChar = this->reader.nextChar();
			if (nxtChar == '-')
				token.type = Type::Decrementation;
			else if (nxtChar == '=')
				token.type = Type::MinusEqual;
			else
			{
				token.type = Type::Minus;
				this->reader.rewind();
			}
			break;

		default:
		{
			if (planeSigns.count(sign) == 1)
				token.type = planeSigns.at(sign);
			else
			{
				token.type = Type::Invalid;
				token = errorHandler("Unexpected symbol", token);
			}
		}
			

		}
	}
	
	cout << "Typ: " << getTypeName(token.type);
	cout.width(30);
	cout.fill('_');
	cout << "     Value ^: " << token.value;
	std::cout << " Line: "<< token.line << " Pos: " << token.pos<< endl;

	return token;
}



