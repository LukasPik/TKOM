
#include "stdafx.h"
#include "FileReader.h"
#include <iostream>



FileReader::FileReader(const std::string& file): handler(file)
{

	if (this->handler.fail())
	{
		std::cout << "Failed to open file" << std::endl;
	}

}

const char FileReader::nextChar()
{
	char sign;

	while (true)
	{
		sign = this->handler.get();
		if (sign == '\n' || sign == '\r')
		{
			if (this->prevChar != sign &&
				(this->prevChar == '\n' || this->prevChar == '\r')
				)
			{
				// CRLF or LFCR, just skip it
			}
			else
			{
				this->lineNumber++;
				this->currentSignPos = 0;
				this->currentLinePos = this->handler.tellg();
			}

			this->prevChar = sign;
			continue;
		}

		this->prevChar = sign;

		this->currentSignPos++;
		return sign;
	}
}


const bool FileReader::endOfFile()
{
	return this->handler.eof();
}


void FileReader::rewind()
{
	this->handler.unget().unget();

	this->prevChar = this->handler.get();
	auto peek = this->handler.peek();

	if (peek == '\n' || peek == '\r')
	{
		this->prevChar = this->handler.get();
		return;
	}
	this->currentSignPos--;
}


const std::string FileReader::getLine(const std::streampos& linePos)
{
	const std::streampos currentPos = this->handler.tellg();
	std::string line;

	this->handler.seekg(linePos);

	std::getline(this->handler, line);

	this->handler.seekg(currentPos);

	return line;
}