#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>

using namespace std;

class TextFile
{
public:
	string content;
	int freqTable[128];

	TextFile() {}

	TextFile(const char* fileName)
	{
		FILE* fp = fopen(fileName, "r");
		long int fileSize = 0;
		char* contentChr = { '\0' };
		
		//Getting length
		fseek(fp, 0, SEEK_END);
		fileSize = ftell(fp);
		rewind(fp);

		contentChr = (char*)malloc(fileSize + 10);
		fgets(contentChr, fileSize, fp);
		contentChr[fileSize] = '\0';
		content = contentChr;

		fclose(fp);
	}

private:

};

void main()
{
	TextFile* tf = new TextFile("Salt.txt");
}
