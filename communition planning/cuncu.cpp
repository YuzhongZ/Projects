#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<iomanip.h>

#include"Type.h"

void CreateALGraph(GraphAdjList &GA,int n,int e)   //生成邻接表
{
	GA.numEdges=e;
	GA.numVertexes=n;
	Edgenode *p,*q;
	int i,j,w,k;
	char g[MAX];
    for(i=0;i<GA.numVertexes;i++)        //读入顶点信息，建立顶点表
	{
		cout<<"请输入顶点信息:";
		cin>>g;                          //输入顶点信息
		strcpy(GA.adjlist[i].data,g);
        GA.adjlist[i].firstarc=NULL;     //将边表置为空表
	} 
	cout<<endl;
 	k=0;
    while(k<GA.numEdges)                //建立边表
	{ 
		cout<<"输入边(vi,vj)的顶点序号i,j和权值w:";      //输入图的信息
        cin>>i>>j>>w;
		if(i>=0&&j>=0&&i<GA.numEdges&&j<GA.numEdges)         //若输入符合条件
		{
			p=GA.adjlist[i].firstarc;                    //p指向顶点表的头指针
			while(p!=NULL&&p->adjvex!=j)                 //循环找到顶点表尾
				p=p->nextarc;
			if(p==NULL)                                  //若p不为空
			{
				q=new Edgenode;                          //将输入的信息存入p中
				q->adjvex=j;
				q->weight=w;
				q->nextarc=GA.adjlist[i].firstarc;       //p头插入表中
				GA.adjlist[i].firstarc=q;
				q=new Edgenode;                          //由于是无向表，所以在两个顶点表中都存储
				q->adjvex=i;
				q->weight=w;
				q->nextarc=GA.adjlist[j].firstarc;
				GA.adjlist[j].firstarc=q;
		    	k++;
			}
			else
				cout<<"边重复，请重新输入！"<<endl;
		}
		else
			cout<<"端点输入有误，请重新输入！"<<endl;
	}
}

void printALGraph(GraphAdjList &GA)
{
	cout<<"\n邻接表为"<<endl;
	Edgenode *p;
	for(int i=0;i<GA.numVertexes;i++)
	{
		cout<<"顶点 邻接点 权值 邻接点 权值 邻接点 权值 邻接点 权值 邻接点 权值"<<endl;
	    cout<<GA.adjlist[i].data;
		for(p=GA.adjlist[i].firstarc;p!=NULL;p=p->nextarc)
		{
			cout<<setw(6)<<p->adjvex<<setw(6)<<p->weight;

		}
		cout<<endl;
	}
}

void createMGraph(MGraph &G,GraphAdjList GA)      //生成邻接矩阵
{
	int i,j; 
	Edgenode *q;
	G.numEdges=GA.numEdges;
	G.numVertexes=GA.numVertexes;
	
	for(i=0;i<G.numVertexes;i++)                  //存入顶点信息并将邻接矩阵初始化
	{
		strcpy(G.vexs[i],GA.adjlist[i].data);
		for(j=0;j<G.numVertexes;j++)
			G.arc[i][j]=(i==j)?0:MAXWEIGHT;
	}
	for(i=0;i<G.numVertexes;i++)                  //利用邻接表将每条通路的信息存入矩阵中
	{
		q=new Edgenode;
    	q=GA.adjlist[i].firstarc;
    	while(q!=NULL)
		{
    		
    		G.arc[i][q->adjvex]=q->weight;
	    	q=q->nextarc;
		}

	}
}


/*
void createMGraph(MGraph &G,int n,int e)
{
	int i,j,k,w; 
	G.numEdges=e;
	G.numVertexes=n;
	for(i=0;i<G.numVertexes;i++)
	{
		cout<<"请输入顶点下标:";
		cin>>G.vexs[i];
		for(j=0;j<G.numVertexes;j++)
			G.arc[i][j]=(i==j)?0:MAXWEIGHT;
	}
	k=0;
	while(k<G.numEdges)
	{
		cout<<"请输入边(vi,vj)两端点的下标i,j和权值w（输入-1结束）:";
		cin>>i>>j>>w;
		if(i>=0&&j>=0&&w>=0)
		{
    		G.arc[i][j]=w;
	    	G.arc[j][i]=w;
			k++;
		}
		else
			cout<<"端点输入有误，请重新输入！"<<endl;
	}
}*/

void printMGragh(MGraph &G)
{
	cout<<"\n邻接矩阵为"<<endl;
	cout<<"  ";
	for(int i=0;i<G.numVertexes;i++)
		cout<<setw(5)<<G.vexs[i];
	cout<<endl;
	for(i=0;i<G.numVertexes;i++)
	{
		cout<<G.vexs[i];
		for(int j=0;j<G.numVertexes;j++)
		{
			
			if(G.arc[i][j]>=0&&G.arc[i][j]<MAXWEIGHT)
		    	cout<<setw(5)<<G.arc[i][j];
			else
				cout<<setw(5)<<"∞";
		}
		cout<<endl;
	}
}
