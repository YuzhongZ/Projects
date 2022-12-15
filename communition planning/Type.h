#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<iomanip.h>

typedef int EdgeType;
#define MAX 100
#define MAXWEIGHT 6500

//�ڽӾ���
typedef struct MGraph            
{
	char vexs[MAX][MAX];         //���涥����Ϣ
	EdgeType arc[MAX][MAX];      //���󣬴洢Ȩֵ
	int numVertexes,numEdges;    //�������ͱ���
}MGraph;

//�߱���
typedef struct Edgenode
{
	int adjvex;                      //�ڽӵ��򣬴洢�ö����±�
	EdgeType weight;                 //Ȩֵ
	struct Edgenode *nextarc;        //����ָ����һ���ڽӵ�
}Edgenode;


//�������
typedef struct Vertexnode           
{ 
	char data[MAX];             //�����򣬴洢������Ϣ
	Edgenode *firstarc;         //�߱�ͷָ��
}Vertexnode,Adjlist[MAX];

//�ڽӱ�
typedef struct 
{
	Adjlist adjlist;           //����
	int numVertexes,numEdges;  //�������ͱ���
}GraphAdjList;

//�߼�����
typedef struct      
{
	int v1,v2;                 //ͨ·���˵�
	EdgeType weight;          //Ȩֵ
}Edge;

//��С������
typedef struct
{
	Edge edgeValue[MAX];      //�����Ϣ
	int n;                    //�����
}MinSpanTree;



void CreateALGraph(GraphAdjList &GA,int n,int e);  //�����ڽӱ�
void printALGraph(GraphAdjList &GA);//����ڽӱ�
void createMGraph(MGraph &G,GraphAdjList GA);//�����ڽӾ���
void printMGragh(MGraph &G);//����ڽӾ���
void InitMinSpanTree(MinSpanTree &T);//��ʼ��������
int cmp(const void* a, const void* b);//������
int Find(int *parent,int f);//Ѱ�Ҹ����
int Kruskal_MST(MGraph G);//��³˹�����㷨
int Prim(MGraph G);//prim�㷨
