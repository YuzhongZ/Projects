#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define leafNumber 27
#define totalNumber 53
#define Maxvalue 3000

#include"Huffman.h"

void TranslateCode(int n,HFCharCode HC[])                           //�Ա������ݽ��б���
{
	char EnterStr[Maxvalue];
	HFCharCode p[Maxvalue];
	cout<<"������Ҫ��������ݣ���д��ĸ����";
	cin.ignore();
	cin.getline(EnterStr,100);
    int len=strlen(EnterStr);
	int flag=0;
	cout<<"��������";
    for(int i=0;i<len;i++)                          //�Ա�����ÿ���ַ����в��Ҵ������������
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
	if(flag<len)                                      //������������
	{
     	cout<<"\n�������ݴ��󣬱��Ľ��ܺ��д�д��ĸ�Ϳո�!\n\n";	        	
		return;
	}
	for(i=0;i<len;i++)                             //����������ݵĹ���������
	{
	   	cout<<p[i].code;
	}
    cout<<endl;
	return;
}
