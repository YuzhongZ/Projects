#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define leafNumber 27
#define totalNumber 53
#define Maxvalue 3000

#include"Huffman.h"

void DecodeHFCode(HFTree& HT)                        //�Ա��Ľ��н���
{               
    int k=HT.root;                                   //�Ӹ������ſ�ʼ
    int len=0,i=0;
	char EnterCode[Maxvalue];
	char RealStr[Maxvalue];
	cout<<"������Ҫ��������ݣ�";
	cin>>EnterCode;
    while(EnterCode[i]!='\0')
	{
        if(EnterCode[i]=='0')                        //�ж������Ƿ�Ϊ0,��Ϊ0����һ��·��ͨ������
            k=HT.elem[k].lchild;
        else 
			if(EnterCode[i]=='1')                    //�ж������Ƿ�Ϊ1,��Ϊ1����һ��·��ͨ���Һ���
               k=HT.elem[k].rchild;
            else                                         //���Ĵ��ڴ���
			{
                cout<<"\n����! �����н��ܺ���1��0!\n\n";
                return;
			}
        if(HT.elem[k].lchild==-1&&HT.elem[k].rchild==-1) //���ý�㲻�������Һ��ӣ�˵��Ϊ�õ��Ӧ�ַ�
		{
            RealStr[len++]=HT.elem[k].data;
            k=HT.root;                                   //k������Ϊ�����
        }
        i++;
    }
    RealStr[len]='\0';
    if(k==HT.root)                                      //���������
	{
        cout<<"������:"<<RealStr<<endl;
        return;
    }
    cout<<"\n����! ���������޷�����!\n\n";               
    return;
}