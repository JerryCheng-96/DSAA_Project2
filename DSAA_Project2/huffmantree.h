const int n_max=256;     //���256���ַ�

class huffmantree
{
public:
	huffmantree();     //����������
	virtual ~huffmantree();     //��������
	void count(char* name);     //
	void select();     //��������ֵ��С���޸��ڵ�Ľڵ�
	void create();     //�����������
	void code();     //����
	void storetree(char* namec);     //�����������
	void tot(char* name,char* namef);     //ѹ��
	void compress(char* name,char* namef,char* namec);     //ѹ��
	void decompress(char* name,char* namef,char* namec);     //��ѹ

private:
	int n;     //Ҷ�ڵ����
	int m1,m2;     //��С������ֵ
	htnode ht[2*n_max-1];     //����ڵ�Ľṹ��
};