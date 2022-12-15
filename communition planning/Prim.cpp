#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<iomanip.h>

#include"Type.h"

//进行prim算法实现，使用的邻接矩阵的方法实现。
int Prim(MGraph G) 
{
	cout<<"\n使用Prim算法所得路径和每条路径的距离为\n";
	int begin=0,sum=0;                                   //设起点下标为0
    Edge path[MAX];                                      //数组记录到达某个顶点的各个边中的权重最大的那个边
    int j;

    for(j=0;j<G.numVertexes;j++)                         //对path数组初始化,v1设置为起点,v2设置为该点下标
	{
        if(j!=begin) 
		{
            path[j].v1=begin;
            path[j].v2=j;
            path[j].weight=G.arc[begin][j];
        }
    }
    path[begin].weight=-1;                                //把起点的path中的值设置为-1，代表已经加入到集合U中
    for(j=1;j<G.numVertexes;j++)                          //访问剩下的顶点，并加入依次加入到集合U
	{
        int min=MAXWEIGHT;
        int k;
        int index;

        //寻找数组与该顶点连接中权重最小的那个边
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

        //将权重最小的那条边的终点也加入到集合U
        path[index].weight=-1;

        //输出对应的边的信息
		cout<<"(v"<<path[index].v1<<",v"<<path[index].v2<<")";
        cout<<" "<<G.arc[path[index].v1][path[index].v2]<<endl;

        sum+=G.arc[path[index].v1][path[index].v2];             //计算路径长度

        //更新数组
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