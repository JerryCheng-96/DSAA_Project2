#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>

using namespace std;

class TextFile
{
public:
	string content;
	int freqList[128] = { 0 };
	long int fileSize;

	TextFile() {}

	TextFile(const char* fileName)
	{
		FILE* fp = fopen(fileName, "r");
		fileSize = 0;
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

	void genFreqList()
	{
		for (int i = 0; i < content.length(); i++)
		{
			freqList[content.c_str()[i]]++;
		}
	}

private:

};

void main()
{
	TextFile* tf = new TextFile("Salt.txt");
	tf->genFreqList();
}
