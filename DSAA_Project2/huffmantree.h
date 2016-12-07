const int n_max=256;     //最多256个字符

class huffmantree
{
public:
	huffmantree();     //哈夫曼树类
	virtual ~huffmantree();     //析构函数
	void count(char* name);     //
	void select();     //找两个求值最小且无父节点的节点
	void create();     //构造哈夫曼树
	void code();     //编码
	void storetree(char* namec);     //储存哈夫曼树
	void tot(char* name,char* namef);     //压码
	void compress(char* name,char* namef,char* namec);     //压缩
	void decompress(char* name,char* namef,char* namec);     //解压

private:
	int n;     //叶节点个数
	int m1,m2;     //最小的两个值
	htnode ht[2*n_max-1];     //储存节点的结构体
};