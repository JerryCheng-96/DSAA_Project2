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

		contentChr = (char*)malloc(fileSize + 1);
		fgets(contentChr, fileSize + 1, fp);
		contentChr[fileSize] = '\0';
		content = contentChr;
		free(contentChr);
		contentChr = NULL;
		fclose(fp);
	}

	void genFreqList()
	{
		//Initializing
		for (int j = 0; j < 128; j++)
		{
			freqList[j] = 0;
		}

		for (int i = 0; i < content.length(); i++)
		{
			freqList[content.c_str()[i]]++;
		}
	}

	void toUpperCase() 
	{
		char* contentChr = (char*)malloc((this->fileSize) * sizeof(char) + 1);
		for (int i = 0; i < content.length(); i++)
		{
			if (content.c_str()[i] >= 'a' && content.c_str()[i] <= 'z')
			{
				contentChr[i] = content.c_str()[i] - 32;
				continue;
			}
			contentChr[i] = content.c_str()[i];
		}
		contentChr[fileSize] = '\0';
		content.clear();
		content = contentChr;
	}

private:

};

void main()
{
	TextFile* tf = new TextFile("Salt.txt");
	tf->genFreqList();
	tf->toUpperCase();
	tf->genFreqList();
}
