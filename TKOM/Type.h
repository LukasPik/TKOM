#ifndef TKOM_TYPE
#define TKOM_TYPE

#include <unordered_map>
namespace token {
	enum class Type : unsigned int
	{
		ParenthesisOpen = 0,
		ParenthesisClose = 1,
		BracketOpen = 2,
		BracketClose = 3,
		Comma = 4,
		Semicolon = 5,
		Apostrophe = 6,
		If = 7,
		While = 8,
		For = 9,
		Or = 10,
		And = 11,
		Equality = 12,
		Inequality = 13,
		Assigment = 14,
		Expression = 15,
		Less = 16,
		Greater = 17,
		LessEqual = 18,
		GreaterEqual = 19,
		Plus = 20,
		Minus = 21,
		Multiply = 22,
		Divide = 23,
		Incrementation = 24,
		Decrementation = 25,
		Postincrementation = 26,
		PostDecrementation = 27,
		PlusEqual = 28,
		MinusEqual = 29,
		Number = 30,
		EndOfFile = 31,
		Invalid = 32,
		Identifier = 33,
		Type = 34,
		Undefined = 35

	};

	const std::unordered_map<unsigned int, std::string> typeNames = {
		{ 0,  "ParenthesisOpen" },
		{ 1,  "ParenthesisClose" },
		{ 2,  "BracketOpen" },
		{ 3,  "BracketClose" },
		{ 4,  "Comma" },
		{ 5,  "Semicolon" },
		{ 6,  "Apostrophe" },
		{ 7,  "If" },
		{ 8, "While" },
		{ 9, "For" },
		{ 10, "Or" },
		{ 11, "And" },
		{ 12, "Equality" },
		{ 13, "Inequality" },
		{ 14,  "Assigment" },
		{ 15,  "Expression" },
		{ 16, "Less" },
		{ 17, "Greater" },
		{ 18, "LessEqual" },
		{ 19, "GreaterEqual" },
		{ 20, "Plus" },
		{ 21, "Minus" },
		{ 22, "Multiply" },
		{ 23, "Divide" },
		{ 24, "Incrementation" },
		{ 25, "Decrementation" },
		{ 26, "Postincrementation" },
		{ 27, "PostDecrementation" },
		{ 28, "PlusEqual" },
		{ 29, "MinusEqual" },
		{ 30, "Number" },
		{ 31, "EndOfFile" },
		{ 32, "Invalid" },
		{ 33, "Identifier"},
		{ 34, "Type" },
		{ 35, "Undefined" }
	};


	inline const std::string getTypeName(const Type& tokenType)
	{
		return typeNames.at(static_cast<unsigned int>(tokenType));
	};

}

#endif
