#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<iomanip.h>

#include"Type.h"


void InitMinSpanTree(MinSpanTree &T)
{
	T.n=0;
}


//�������������
int cmp(const void* a, const void* b)
{
    return (*(Edge*)a).weight - (*(Edge*)b).weight;
}

//�ҵ����ڵ����
int Find(int *parent,int f)
{
    while(parent[f]>0)
	{
        f=parent[f];
    }
    return f;
}

int Kruskal_MST(MGraph G)                	//��³˹�����㷨������ͨ���������С������
{
	cout<<"\nʹ��Kruskal�㷨����·����ÿ��·���ľ���Ϊ\n";
	MinSpanTree T;
	InitMinSpanTree(T);
	int i, j, n, m,sum=0;
    int k=0;
    int parent[MAX];                         //����Ѱ�Ҹ��ڵ������
    for(i=0;i<G.numVertexes-1;i++)           //������С�������еı߼����鲢����
	{
       for(j=i+1;j<G.numVertexes;j++) 
	   {
           if(G.arc[i][j]<MAXWEIGHT) 
		   {
               T.edgeValue[T.n].v1=i;                //���С�Ľ��Ϊ��
               T.edgeValue[T.n].v2=j;                //��Ŵ�Ľ��Ϊβ
               T.edgeValue[T.n].weight=G.arc[i][j];
               T.n++;
            }
        }
	}

	//Ϊ��С�������еı߼���������
    qsort(T.edgeValue,G.numEdges,sizeof(Edge),cmp);

	//��ʼ��parent����
    for(i=0;i<G.numVertexes;i++)
        parent[i]=0;

	for(i=0;i<G.numEdges;i++)
	{
        n=Find(parent,T.edgeValue[i].v1);       //Ѱ�ұ�T.edgeValue[i]��v1������������
        m=Find(parent,T.edgeValue[i].v2);       //Ѱ�ұ�T.edgeValue[i]��v2������������

        //��n��m���ȣ�˵���������㲻��һ�����ڣ���������ߵļ��벻��ʹ�Ѿ�ѡ��ı߼�������·
        if(n!=m)
		{
            parent[n]=m;
			sum+=T.edgeValue[i].weight;
            cout<<"(v"<<T.edgeValue[i].v1<<",v"<<T.edgeValue[i].v2<<")"<<" "<<T.edgeValue[i].weight<<endl;
        }
    }
	return sum;
}
