#define _CRT_SECURE_NO_WANINGS

#include <iostream>
#include <fstream>
#include <queue>
using namespace std;

typedef struct htnode{
	unsigned char ch;     //�����ַ�
	int weight;     //Ȩֵ
	int parent,lchild,rchild;
	int* code;     //��������
	int len;     //���볤��

	htnode()
	{
		weight=0;
		parent=-1;
		lchild=-1;
		rchild=-1;
		len=0;
	}
}htnode;