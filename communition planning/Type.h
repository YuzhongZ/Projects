#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<iomanip.h>

typedef int EdgeType;
#define MAX 100
#define MAXWEIGHT 6500

//邻接矩阵
typedef struct MGraph            
{
	char vexs[MAX][MAX];         //储存顶点信息
	EdgeType arc[MAX][MAX];      //矩阵，存储权值
	int numVertexes,numEdges;    //顶点数和边数
}MGraph;

//边表结点
typedef struct Edgenode
{
	int adjvex;                      //邻接点域，存储该顶点下标
	EdgeType weight;                 //权值
	struct Edgenode *nextarc;        //链域，指向下一个邻接点
}Edgenode;


//顶点表结点
typedef struct Vertexnode           
{ 
	char data[MAX];             //顶点域，存储顶点信息
	Edgenode *firstarc;         //边表头指针
}Vertexnode,Adjlist[MAX];

//邻接表
typedef struct 
{
	Adjlist adjlist;           //链表
	int numVertexes,numEdges;  //顶点数和边数
}GraphAdjList;

//边集数组
typedef struct      
{
	int v1,v2;                 //通路两端点
	EdgeType weight;          //权值
}Edge;

//最小生成树
typedef struct
{
	Edge edgeValue[MAX];      //结点信息
	int n;                    //结点数
}MinSpanTree;



void CreateALGraph(GraphAdjList &GA,int n,int e);  //生成邻接表
void printALGraph(GraphAdjList &GA);//输出邻接表
void createMGraph(MGraph &G,GraphAdjList GA);//生成邻接矩阵
void printMGragh(MGraph &G);//输出邻接矩阵
void InitMinSpanTree(MinSpanTree &T);//初始化生成树
int cmp(const void* a, const void* b);//排序函数
int Find(int *parent,int f);//寻找根结点
int Kruskal_MST(MGraph G);//克鲁斯卡尔算法
int Prim(MGraph G);//prim算法
