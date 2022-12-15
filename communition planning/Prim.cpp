#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<iomanip.h>

#include"Type.h"

//����prim�㷨ʵ�֣�ʹ�õ��ڽӾ���ķ���ʵ�֡�
int Prim(MGraph G) 
{
	cout<<"\nʹ��Prim�㷨����·����ÿ��·���ľ���Ϊ\n";
	int begin=0,sum=0;                                   //������±�Ϊ0
    Edge path[MAX];                                      //�����¼����ĳ������ĸ������е�Ȩ�������Ǹ���
    int j;

    for(j=0;j<G.numVertexes;j++)                         //��path�����ʼ��,v1����Ϊ���,v2����Ϊ�õ��±�
	{
        if(j!=begin) 
		{
            path[j].v1=begin;
            path[j].v2=j;
            path[j].weight=G.arc[begin][j];
        }
    }
    path[begin].weight=-1;                                //������path�е�ֵ����Ϊ-1�������Ѿ����뵽����U��
    for(j=1;j<G.numVertexes;j++)                          //����ʣ�µĶ��㣬���������μ��뵽����U
	{
        int min=MAXWEIGHT;
        int k;
        int index;

        //Ѱ��������ö���������Ȩ����С���Ǹ���
        for(k=0;k<G.numVertexes;k++)
		{
            if(path[k].weight!=-1)
			{  
                if(path[k].weight<min)
				{
                    min=path[k].weight;
                    index=k;
                }
            }

        }

        //��Ȩ����С�������ߵ��յ�Ҳ���뵽����U
        path[index].weight=-1;

        //�����Ӧ�ıߵ���Ϣ
		cout<<"(v"<<path[index].v1<<",v"<<path[index].v2<<")";
        cout<<" "<<G.arc[path[index].v1][path[index].v2]<<endl;

        sum+=G.arc[path[index].v1][path[index].v2];             //����·������

        //��������
        for(k=0;k<G.numVertexes;k++)
		{ 
			if(G.arc[index][k]<path[k].weight)
			{
                path[k].weight=G.arc[index][k];
                path[k].v1=index;
                path[k].v2=k;
            }
        }
    }
	return sum;
}