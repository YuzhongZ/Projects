#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define leafNumber 27
#define totalNumber 53
#define Maxvalue 3000

#include"Huffman.h"

void createHFTree(HFTree& HT,char value[],int ft[],int n)      //构造哈夫曼树
{
	int i,k,s1,s2;
	int min1,min2;
	for(i=0;i<n;i++)
	{
		HT.elem[i].data=value[i];
		HT.elem[i].weight=ft[i];
	}
	for(i=0;i<totalNumber;i++)
	{ 
		HT.elem[i].parent=HT.elem[i].lchild=HT.elem[i].rchild=-1;        //所有指针置为-1
	}
	for(i=n;i<2*n-1;i++)
	{
		min1=min2=Maxvalue;
		s1=s2=0;
		for(k=0;k<i;k++)                                    //开始循环构造
			if(HT.elem[k].parent==-1)
				if(HT.elem[k].weight<min1)
				{
					min2=min1;
					s2=s1;
					min1=HT.elem[k].weight;
					s1=k;
				}
				else
					if(HT.elem[k].weight<min2)
					{
						min2=HT.elem[k].weight;
				    	s2=k;
					}
		HT.elem[s1].parent=HT.elem[s2].parent=i;                 //构造子树
		HT.elem[i].lchild=s1;
		HT.elem[i].rchild=s2;
		HT.elem[i].weight=HT.elem[s1].weight+HT.elem[s2].weight;
	}
	HT.num=n;
	HT.root=2*n-2;
}


void Buildcode(HFTree& HT,HFCharCode HC[])
{
    char *flag;
    flag=(char *)malloc(HT.num*sizeof(char *));                    //为哈弗曼编码动态分配空间
	cout<<"该系统中字符对应编码为如下"<<endl;

	int i,j,len;
    for(i=0;i<=HT.num-1; i++) 
	{
        len=0;
		int k=0;
        j=i;
        HC[i].ch = HT.elem[j].data;
        while(HT.elem[j].parent!=-1)
		{                                                 //不是根节点
            if(HT.elem[HT.elem[j].parent].lchild==j) 
			{                                             //判断是否是其双亲的左孩子
                flag[k++]='0';                            //是左孩子则该路径记为0
            }
			else                                          //判断是否是其双亲的右孩子
                flag[k++]='1';                            //是右孩子则该路径记为1
            j=HT.elem[j].parent;                        //往上遍历
        }
        int l=0;
		cout<<HT.elem[i].data<<":";                            //倒序输出这条路径的值即该字符的编码
		for(int j=k-1;j>=0;j--)
		{
     		cout<<flag[j];
			HC[i].code[l++]=flag[j];                           //把该字符的编码记录在HC数组中
		}
		HC[i].code[l]='\0';
    	cout<<endl;
	}
}




void main()
{
	HFTree S;
	HFCharCode HC[leafNumber];
	int r;
	char value[leafNumber]={' ','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    int ft[leafNumber]={186,64,13,22,32,103,21,15,47,57,1,5,32,20,57,63,15,1,48,51,80,23,8,18,1,16,1};
    int n=27;
	createHFTree(S,value,ft,n);
	Buildcode(S,HC);
	cout<<endl;
	cout<<"*********欢迎进入哈夫曼编码系统*********"<<endl;
	cout<<"*********1、对报文内容进行编码**********"<<endl;
	cout<<"*********2、对报文内容进行解码**********"<<endl;
	cout<<"*********3、退出系统********************"<<endl;
	cout<<"\n请输入您的选择：";
	cin>>r;
	while(r&&r<4)
	{
    	switch(r)
		{
    		case 1:TranslateCode(n,HC);break;
    		case 2:DecodeHFCode(S);break;
	    	case 3:exit(0);break;
		}
		cout<<"\n请输入您的选择：";
		cin>>r;
	}

}



