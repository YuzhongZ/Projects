#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<iomanip.h>

#include"Type.h"



void main()
{
	MGraph G;
	GraphAdjList GA;
	int num,edge,a=0,b=0;
	cout<<"请输入顶点数和边数:";
	cin>>num>>edge;
	int h;
	CreateALGraph(GA,num,edge);
	printALGraph(GA);
	createMGraph(G,GA);
	printMGragh(G);
	cout<<endl;
	cout<<"****************通信网络规划**************"<<endl;
	cout<<"*********1、使用Prim算法进行规划**********"<<endl;
	cout<<"*********2、使用Kruskal算法进行规划************"<<endl;
	cout<<"\n请输入您的选择(输入-1退出)：";
	cin>>h;
	while(h>0&&h<4)
	{
     	switch(h)
		{
    	   	case 1:a=Prim(G);break;
    		case 2:b=Kruskal_MST(G);break;
		}
    	cout<<"\n请输入您的选择(输入-1退出)：";
    	cin>>h;
	}
	if(a&&b&&a>b)
		cout<<"\nKruskal算法所得路径短，成本低"<<endl;
	else
		if(a&&b&&a<b)
			cout<<"\nPrim算法所得路径短，成本低"<<endl;
		else
			if(a&&b)
	    		cout<<"\n两种方法所得路径一样长，成本相同"<<endl;


}

