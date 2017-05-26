#ifndef TKOM_TOKEN
#define TKOM_TOKEN



#include "Type.h"


using namespace token;


		struct Token
		{
			Token(const Type& type) : type(type) {}
			Token() = default;

			Type type;

			std::string value = "";

			unsigned int line = 0;
			unsigned int pos = 0;
			std::streampos lineStart;

		};

#endif // !TKOM_TOKEN

			

		


