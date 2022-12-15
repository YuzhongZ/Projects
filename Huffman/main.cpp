#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define leafNumber 27
#define totalNumber 53
#define Maxvalue 3000

#include"Huffman.h"

void createHFTree(HFTree& HT,char value[],int ft[],int n)      //�����������
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
		HT.elem[i].parent=HT.elem[i].lchild=HT.elem[i].rchild=-1;        //����ָ����Ϊ-1
	}
	for(i=n;i<2*n-1;i++)
	{
		min1=min2=Maxvalue;
		s1=s2=0;
		for(k=0;k<i;k++)                                    //��ʼѭ������
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
		HT.elem[s1].parent=HT.elem[s2].parent=i;                 //��������
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
    flag=(char *)malloc(HT.num*sizeof(char *));                    //Ϊ���������붯̬����ռ�
	cout<<"��ϵͳ���ַ���Ӧ����Ϊ����"<<endl;

	int i,j,len;
    for(i=0;i<=HT.num-1; i++) 
	{
        len=0;
		int k=0;
        j=i;
        HC[i].ch = HT.elem[j].data;
        while(HT.elem[j].parent!=-1)
		{                                                 //���Ǹ��ڵ�
            if(HT.elem[HT.elem[j].parent].lchild==j) 
			{                                             //�ж��Ƿ�����˫�׵�����
                flag[k++]='0';                            //���������·����Ϊ0
            }
			else                                          //�ж��Ƿ�����˫�׵��Һ���
                flag[k++]='1';                            //���Һ������·����Ϊ1
            j=HT.elem[j].parent;                        //���ϱ���
        }
        int l=0;
		cout<<HT.elem[i].data<<":";                            //�����������·����ֵ�����ַ��ı���
		for(int j=k-1;j>=0;j--)
		{
     		cout<<flag[j];
			HC[i].code[l++]=flag[j];                           //�Ѹ��ַ��ı����¼��HC������
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
	cout<<"*********��ӭ�������������ϵͳ*********"<<endl;
	cout<<"*********1���Ա������ݽ��б���**********"<<endl;
	cout<<"*********2���Ա������ݽ��н���**********"<<endl;
	cout<<"*********3���˳�ϵͳ********************"<<endl;
	cout<<"\n����������ѡ��";
	cin>>r;
	while(r&&r<4)
	{
    	switch(r)
		{
    		case 1:TranslateCode(n,HC);break;
    		case 2:DecodeHFCode(S);break;
	    	case 3:exit(0);break;
		}
		cout<<"\n����������ѡ��";
		cin>>r;
	}

}



