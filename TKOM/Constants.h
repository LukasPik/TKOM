#pragma once
#include <unordered_map>
#include "Type.h"

namespace token {
	const std::unordered_map<std::string, Type>& keyword = {
		{ "if" , Type::If },
		{ "while" , Type::While },
		{ "for" , Type::For },
		{ "or" , Type::Or },
		{ "and" , Type::And },
		{ "int", Type::Type},
		{ "double", Type::Type },
		{ "char", Type::Type }
	};


	const std::unordered_map<char, Type>& planeSigns = {
		{ '(', Type::ParenthesisOpen },
		{ ')', Type::ParenthesisClose },
		{ '{', Type::BracketOpen },
		{ '}', Type::BracketClose },
		{ ',', Type::Comma },
		{ ';', Type::Semicolon },
		{ '+', Type::Plus },
		{ '-', Type::Minus },
		{ '*', Type::Multiply },
		{ '/', Type::Divide },
		{ '\'', Type::Apostrophe}
	};

}