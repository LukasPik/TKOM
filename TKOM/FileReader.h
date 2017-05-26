#ifndef  TKOM_FILEREADER
#define TKOM_FILEREADER



#include <fstream>
#include <string>

	class FileReader
	{
	private:
		std::ifstream handler;
		char prevChar;

		



	public:
		unsigned int lineNumber = 1;
		unsigned int currentSignPos = 0;
		std::streampos currentLinePos;


		FileReader(const std::string& file);
		const char nextChar();
		const bool endOfFile();
		void rewind();
		const std::string getLine(const std::streampos & linePos);
	};
#endif // ! TKOM_FILEREADER

