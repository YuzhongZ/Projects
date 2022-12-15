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


//快速排序的条件
int cmp(const void* a, const void* b)
{
    return (*(Edge*)a).weight - (*(Edge*)b).weight;
}

//找到根节点迭代
int Find(int *parent,int f)
{
    while(parent[f]>0)
	{
        f=parent[f];
    }
    return f;
}

int Kruskal_MST(MGraph G)                	//克鲁斯卡尔算法，在连通网中求出最小生成树
{
	cout<<"\n使用Kruskal算法所得路径和每条路径的距离为\n";
	MinSpanTree T;
	InitMinSpanTree(T);
	int i, j, n, m,sum=0;
    int k=0;
    int parent[MAX];                         //用于寻找根节点的数组
    for(i=0;i<G.numVertexes-1;i++)           //构建最小生成树中的边集数组并排序
	{
       for(j=i+1;j<G.numVertexes;j++) 
	   {
           if(G.arc[i][j]<MAXWEIGHT) 
		   {
               T.edgeValue[T.n].v1=i;                //编号小的结点为首
               T.edgeValue[T.n].v2=j;                //编号大的结点为尾
               T.edgeValue[T.n].weight=G.arc[i][j];
               T.n++;
            }
        }
	}

	//为最小生成树中的边集数组排序
    qsort(T.edgeValue,G.numEdges,sizeof(Edge),cmp);

	//初始化parent数组
    for(i=0;i<G.numVertexes;i++)
        parent[i]=0;

	for(i=0;i<G.numEdges;i++)
	{
        n=Find(parent,T.edgeValue[i].v1);       //寻找边T.edgeValue[i]的v1所在树的树根
        m=Find(parent,T.edgeValue[i].v2);       //寻找边T.edgeValue[i]的v2所在树的树根

        //若n与m不等，说明两个顶点不在一棵树内，因此这条边的加入不会使已经选择的边集产生回路
        if(n!=m)
		{
            parent[n]=m;
			sum+=T.edgeValue[i].weight;
            cout<<"(v"<<T.edgeValue[i].v1<<",v"<<T.edgeValue[i].v2<<")"<<" "<<T.edgeValue[i].weight<<endl;
        }
    }
	return sum;
}
