#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define leafNumber 27
#define totalNumber 53
#define Maxvalue 3000

#include"Huffman.h"

void TranslateCode(int n,HFCharCode HC[])                           //对报文内容进行编码
{
	char EnterStr[Maxvalue];
	HFCharCode p[Maxvalue];
	cout<<"请输入要编码的内容（大写字母）：";
	cin.ignore();
	cin.getline(EnterStr,100);
    int len=strlen(EnterStr);
	int flag=0;
	cout<<"编码结果：";
    for(int i=0;i<len;i++)                          //对报文中每个字符进行查找储存哈夫曼编码
	{
        for(int j=0;j<n;j++)
		{
            if(EnterStr[i]==HC[j].ch)
			{  
				strcpy(p[i].code,HC[j].code);
				flag++;
			}
        }
    }
	if(flag<len)                                      //报文内容有误
	{
     	cout<<"\n报文内容错误，报文仅能含有大写字母和空格!\n\n";	        	
		return;
	}
	for(i=0;i<len;i++)                             //输出报文内容的哈夫曼编码
	{
	   	cout<<p[i].code;
	}
    cout<<endl;
	return;
}
