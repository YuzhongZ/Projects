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
	cout<<"�����붥�����ͱ���:";
	cin>>num>>edge;
	int h;
	CreateALGraph(GA,num,edge);
	printALGraph(GA);
	createMGraph(G,GA);
	printMGragh(G);
	cout<<endl;
	cout<<"****************ͨ������滮**************"<<endl;
	cout<<"*********1��ʹ��Prim�㷨���й滮**********"<<endl;
	cout<<"*********2��ʹ��Kruskal�㷨���й滮************"<<endl;
	cout<<"\n����������ѡ��(����-1�˳�)��";
	cin>>h;
	while(h>0&&h<4)
	{
     	switch(h)
		{
    	   	case 1:a=Prim(G);break;
    		case 2:b=Kruskal_MST(G);break;
		}
    	cout<<"\n����������ѡ��(����-1�˳�)��";
    	cin>>h;
	}
	if(a&&b&&a>b)
		cout<<"\nKruskal�㷨����·���̣��ɱ���"<<endl;
	else
		if(a&&b&&a<b)
			cout<<"\nPrim�㷨����·���̣��ɱ���"<<endl;
		else
			if(a&&b)
	    		cout<<"\n���ַ�������·��һ�������ɱ���ͬ"<<endl;


}

