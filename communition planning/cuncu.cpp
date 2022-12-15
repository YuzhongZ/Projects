#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<iomanip.h>

#include"Type.h"

void CreateALGraph(GraphAdjList &GA,int n,int e)   //�����ڽӱ�
{
	GA.numEdges=e;
	GA.numVertexes=n;
	Edgenode *p,*q;
	int i,j,w,k;
	char g[MAX];
    for(i=0;i<GA.numVertexes;i++)        //���붥����Ϣ�����������
	{
		cout<<"�����붥����Ϣ:";
		cin>>g;                          //���붥����Ϣ
		strcpy(GA.adjlist[i].data,g);
        GA.adjlist[i].firstarc=NULL;     //���߱���Ϊ�ձ�
	} 
	cout<<endl;
 	k=0;
    while(k<GA.numEdges)                //�����߱�
	{ 
		cout<<"�����(vi,vj)�Ķ������i,j��Ȩֵw:";      //����ͼ����Ϣ
        cin>>i>>j>>w;
		if(i>=0&&j>=0&&i<GA.numEdges&&j<GA.numEdges)         //�������������
		{
			p=GA.adjlist[i].firstarc;                    //pָ�򶥵���ͷָ��
			while(p!=NULL&&p->adjvex!=j)                 //ѭ���ҵ������β
				p=p->nextarc;
			if(p==NULL)                                  //��p��Ϊ��
			{
				q=new Edgenode;                          //���������Ϣ����p��
				q->adjvex=j;
				q->weight=w;
				q->nextarc=GA.adjlist[i].firstarc;       //pͷ�������
				GA.adjlist[i].firstarc=q;
				q=new Edgenode;                          //���������������������������ж��洢
				q->adjvex=i;
				q->weight=w;
				q->nextarc=GA.adjlist[j].firstarc;
				GA.adjlist[j].firstarc=q;
		    	k++;
			}
			else
				cout<<"���ظ������������룡"<<endl;
		}
		else
			cout<<"�˵������������������룡"<<endl;
	}
}

void printALGraph(GraphAdjList &GA)
{
	cout<<"\n�ڽӱ�Ϊ"<<endl;
	Edgenode *p;
	for(int i=0;i<GA.numVertexes;i++)
	{
		cout<<"���� �ڽӵ� Ȩֵ �ڽӵ� Ȩֵ �ڽӵ� Ȩֵ �ڽӵ� Ȩֵ �ڽӵ� Ȩֵ"<<endl;
	    cout<<GA.adjlist[i].data;
		for(p=GA.adjlist[i].firstarc;p!=NULL;p=p->nextarc)
		{
			cout<<setw(6)<<p->adjvex<<setw(6)<<p->weight;

		}
		cout<<endl;
	}
}

void createMGraph(MGraph &G,GraphAdjList GA)      //�����ڽӾ���
{
	int i,j; 
	Edgenode *q;
	G.numEdges=GA.numEdges;
	G.numVertexes=GA.numVertexes;
	
	for(i=0;i<G.numVertexes;i++)                  //���붥����Ϣ�����ڽӾ����ʼ��
	{
		strcpy(G.vexs[i],GA.adjlist[i].data);
		for(j=0;j<G.numVertexes;j++)
			G.arc[i][j]=(i==j)?0:MAXWEIGHT;
	}
	for(i=0;i<G.numVertexes;i++)                  //�����ڽӱ�ÿ��ͨ·����Ϣ���������
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
		cout<<"�����붥���±�:";
		cin>>G.vexs[i];
		for(j=0;j<G.numVertexes;j++)
			G.arc[i][j]=(i==j)?0:MAXWEIGHT;
	}
	k=0;
	while(k<G.numEdges)
	{
		cout<<"�������(vi,vj)���˵���±�i,j��Ȩֵw������-1������:";
		cin>>i>>j>>w;
		if(i>=0&&j>=0&&w>=0)
		{
    		G.arc[i][j]=w;
	    	G.arc[j][i]=w;
			k++;
		}
		else
			cout<<"�˵������������������룡"<<endl;
	}
}*/

void printMGragh(MGraph &G)
{
	cout<<"\n�ڽӾ���Ϊ"<<endl;
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
				cout<<setw(5)<<"��";
		}
		cout<<endl;
	}
}
