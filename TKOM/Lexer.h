#ifndef TKOM_LEXER
#define TKOM_LEXER
#include "FileReader.h"

struct Token;

class Lexer
{
		private:
			FileReader reader;
			const Token& errorHandler(std::string error, const Token &token);

		public:
			Lexer(const std::string& file);
			

			Token nextToken();




};

#endif
