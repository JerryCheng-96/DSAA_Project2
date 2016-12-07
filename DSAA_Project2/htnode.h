#define _CRT_SECURE_NO_WANINGS

#include <iostream>
#include <fstream>
#include <queue>
using namespace std;

typedef struct htnode{
	unsigned char ch;     //储存字符
	int weight;     //权值
	int parent,lchild,rchild;
	int* code;     //编码数组
	int len;     //编码长度

	htnode()
	{
		weight=0;
		parent=-1;
		lchild=-1;
		rchild=-1;
		len=0;
	}
}htnode;