#define _CRT_SECURE_NO_WARNINGS

#include "htnode.h"
#include "huffmantree.h"

huffmantree::huffmantree(){
	n=0;
	m1=-1;
	m2=-1;
}


huffmantree::~huffmantree(){
	for(int i=0;i<n_max;i++)
	{
		if(ht[i].len!=0)
			delete [] ht[i].code;
	}
}


void huffmantree::count(char* name)//统计
{   
	FILE *fp;
	unsigned char c;
	int i=1;

	if((fp=fopen(name,"rb"))==NULL)
	{
		cout << "Can not open the file.";
		return;
	}
    fread(&c,sizeof(c),1,fp);
	if(feof(fp)) 
	{
		cout << "Empty file! No compressing!\n";
		return;
	}
	while (!feof(fp)) 
	{
		if(ht[c].weight==0)
			n++;
		
		ht[c].weight++;
		//ht[c].ch[2]='\0';
		
		ht[c].ch=c;
		//ht[c].ch[1]='\0';
		
		fread(&c,sizeof(c),1,fp);
	}
	fclose(fp);
}


void huffmantree::select()
{     
	m1=m2=-1;
	for(int i=0;i<2*n_max-1;i++)
	{
		if(ht[i].weight>0)
		{
			if(ht[i].parent==-1 && ht[i].weight>0)
			{
				if(m1==-1) m1=i;
				else if(m2==-1) m2=i;
				else if(ht[i].weight<ht[m1].weight) m1=i;
				else if(ht[i].weight<ht[m2].weight) m2=i;
			}
		}
	}

	if(ht[m1].weight>ht[m2].weight)
	{     
		int q;
		q=m1;
		m1=m2;
		m2=q;
	}
}


void huffmantree::create()
{     
	for(int i=n_max;i<n_max+n-1;i++)
	{
		select();
		ht[m1].parent=i;
		ht[m2].parent=i;
		ht[i].lchild=m1;
		ht[i].rchild=m2;
		ht[i].weight=ht[m1].weight+ht[m2].weight;
	}
}


void huffmantree::code()
{     
	int f,c,i;
	int start;
	int length=0;
	
	for(i=0;i<n_max;i++)
	{
		if(ht[i].weight>0)
		{
			c=i;
			while(ht[c].parent>=0)
			{
				ht[i].len++;
				c=ht[c].parent;
			}
			ht[i].code=new int[ht[i].len];
			start=ht[i].len-1;
			c=i;
			while((f=ht[c].parent)>=0 && start>=0)
			{
				if(ht[f].lchild==c)
					ht[i].code[start--]=0;
				else ht[i].code[start--]=1;
				c=f;
			}
		}
	}
}

void huffmantree::storetree(char* namec)
{	
	FILE *stream;
	if((stream=fopen(namec,"wb"))==NULL)
	{
		cout << "ERROR!";
		return;
	}
	for(int i=0;i<2*n_max-1;i++)
	{
		fwrite(&ht[i],sizeof(struct htnode),1,stream);
	}
	fwrite(&n,sizeof(int),1,stream);
	fclose(stream);
}


void huffmantree::tot(char* name,char* namef)
{
	
	FILE *in,*out;
	unsigned char c;
	char array[8]={'\0'};
	int h;
	if((in=fopen(name,"rb"))==NULL)
	{
		cout << "Can not open the file.\n";
		return;
	}
	if((out=fopen("tmpf.txt","w"))==NULL)
	{
		cout << "Error.\n";
		return;
	}
	fread(&c,sizeof(c),1,in);
	while(!feof(in))
	{
		for(h=0;h<ht[c].len;h++)
			fprintf(out,"%u",ht[c].code[h]);//unsigned int 0-65535
		fread(&c,sizeof(c),1,in);
	}
	fclose(in);
	fclose(out);

	//每8个编码数字放一个数组里处理
	FILE *in1,*out1;
	if((in1=fopen("tmpf.txt","r"))==NULL)
	{
		cout << "Can not open the file.\n";
		return;
	}
	if((out1=fopen(namef,"wb"))==NULL)
	{
		cout << "Error.\n";
		return;
	}
	c=fgetc(in1);
	while(!feof(in1))
	{
		array[0]=c;
		int i = 0;
		for(i=1;i<8;i++)
		{
			if((c=fgetc(in1))!=EOF)
				array[i]=c;
			else break;
		}

		if(i!=8)//如果不够8个，向低位补码0（方便解处理）
		{
			for(int d=i;d<8;d++)
				array[d]='0';
		}

		//位运算赋值
		unsigned char b = 0;
		for (int k = 0; k < 8; k++)
		{
			if (array[k] == '1')
			{
				b |= 1;
			}
			else
			{
				b &= (~1);
			}
			
			if (k == 7)
			{
				break;
			}
			else
				b = b << 1;
		}
		fwrite(&b,sizeof(b),1,out);
		c=fgetc(in1);
	}
	fclose(in1);
	fclose(out1);
	remove("tmpf.txt");
}


void huffmantree::compress(char* name,char* namef,char* namec)
{
	count(name);
	create();
	code();
	tot(name,namef);
	storetree(namec);
}

void huffmantree::decompress(char* name,char* namef,char* namec)
{
	
	int start,p,f;
	int addition,n;
	//重新读入编码树
	FILE *str;
	if((str=fopen(namec,"rb"))==NULL)
	{
		cout << "ERROR!";
		return;
	}
	if(feof(str)) 
	{
		cout << "Empty file! No decompressing!\n";
		return;
	}
	int i = 0;
	for(i=0;i<2*n_max-1;i++)
	{
		fread(&ht[i],sizeof(struct htnode),1,str);
	}
	fread(&n,sizeof(int),1,str);
	fclose(str);
	
	//重新生成编码
	for(i=0;i<2*n_max-1;i++)
	{
		if(ht[i].weight>0)
		{
			ht[i].code=new int[ht[i].len];
			start=ht[i].len-1;
			p=i;
			while((f=ht[p].parent)>=0 && start>=0)
			{
				if(ht[f].lchild==p)
					ht[i].code[start--]=0;
				else ht[i].code[start--]=1;
				
				p=f;
			}
		}
	}

	//取出编码存入临时文件
	FILE *in2,*out2;
	if((in2=fopen(name,"rb"))==NULL)
	{
		cout << "Can not open the file.\n";
		return;
	}
	if((out2=fopen("tmpf.txt","w"))==NULL)
	{
		cout << "Error.\n";
		return;
	}
	unsigned char c;
	int asc;
	fread(&c,sizeof(c),1,in2);
	while(!feof(in2))
	{
		int j=0;
		int a[8]={0};
		int s[8]={0};
		
		asc=(int)c;
		while(asc)
		{
			a[j]=asc%2;
			asc/=2;
			j++;
		}
	
		if(j!=0)
		{
			for(int i=j-1;i>=0;i--)
				s[7-i]=a[i];
			for(i=0;i<8-j;i++)
				s[i]=0;
		}
	
		for(i=0;i<8;i++)
			fprintf(out2,"%u",s[i]);

		fread(&c,sizeof(c),1,in2);	
	}
	fclose(in2);
	fclose(out2);

	//获取编码总长
	int bitsum=0;
	for(int gio=0;gio<n_max;gio++)
	{
		if(ht[gio].weight>0)
			bitsum=bitsum+ht[gio].weight*ht[gio].len;
	}
	

	FILE *in3,*out3;
	if((in3=fopen("tmpf.txt","r"))==NULL)
	{
		cout << "Can not open the file.\n";
		return;
	}
	if((out3=fopen(namef,"wb"))==NULL)
	{
		cout << "Error.\n";
		return;
	}

	int pr=n_max+n-2;
	int cu=1;
	char r;
	r=fgetc(in3);
	
	while(cu<=bitsum+1)
	{
		if(ht[pr].lchild>=0 && ht[pr].rchild>=0)
		{
			if(r=='0') pr=ht[pr].lchild;
			else if(r=='1') pr=ht[pr].rchild;
			r=fgetc(in3);
			cu++;
		}
		else
		{
			
			fwrite(&ht[pr].ch,sizeof(ht[pr].ch),1,out3);
			pr=n_max+n-2;
		}
	}
	fclose(in3);
	fclose(out3);
	remove("tmpf.txt");
}

void main(){
	int selection=0;
	char inf[100]={'\0'};
	char outf[100]={'\0'};
	char c[100]={'\0'};

	huffmantree h;

	while(selection!=3)
	{
		cout << "\n";
		cout << "1.Encode\n";
		cout << "2.Decode\n";
		cout << "3.Exit\n";
		
		cout << "Select:";
		cin >> selection;
		
		if(selection==1)
		{
			//remove("tmphuffmantree.txt");
			cout << "Source file name:";
			cin >> inf;
			ifstream fin(inf);
			if(!fin)
			{
				cout << "Can not find the source file.\n";
			}
			else
			{
				cout << "Target file name:";
				cin >> outf;
				cout << "Code file name:";
				cin >> c;
				//cout << "Processing...\n";
				h.compress(inf,outf,c);
				cout << "Done" <<endl;
				
			}
		}
		else if(selection==2)
		{
			cout << "Coded file name:";
			cin >> inf;
			cout << "Code file name:";
			cin >> c;
			ifstream fin(inf);
			if(!fin)
			{
				cout << "Can not find the source file.\n";
			}
			else
			{
				cout << "Target file name:";
				cin >> outf;
				//cout << "Processing...\n";
				h.decompress(inf,outf,c);
				cout << "Done" <<endl;
			}
		}
		else if(selection==3) exit(0);
	}
}




