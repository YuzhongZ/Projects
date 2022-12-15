#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#include"Maze.h"
/*void InitMaze(Maze &m,int maze[][Col])
{    
	for(int i=0;i<Row;i++)
		for(int j=0;j<Col;j++)
        	m.maze[i][j]=maze[i][j];
}


void printmaze(int maze[][Row])  
{
	for(int i=0;i<Row;i++)	
	{	for(int j=0;j<Col;j++)
        	if(maze[i][j]==1)
				cout<<"▆";
			else
				cout<<"  ";
		cout<<endl;
	}
}
*/

void Initqueue(Pathqueue &Q)
{
	Q.front=Q.rear=Q.count_Q=0;
}


void Inqueue(Pathqueue &Q,Position &p)
{
	if((Q.rear+1)%maxsize==Q.front)
		cout<<"队列已满！"<<endl;
	else
	{
		Q.rear=(Q.rear+1)%maxsize;
		Q.data[Q.rear]=p;
		
	}

}


void outqueue(Pathqueue &Q,Position &p)
{
	if(Q.front==Q.rear)
		cout<<"队列为空！"<<endl;
	else
	{  
		Q.front=(Q.front+1)%maxsize;
		p=Q.data[Q.front];
		
	}
}

void store(Pathqueue &Q,int x,int y,Maze &m)
{
	Position q={x,y,Q.front};
	m.maze[x][y]=2;
	Inqueue(Q,q);
}


void printpath_Queue(Pathqueue &Q,Position p,Maze &m)
{
	Position q=p;
	while(q.pre!=-1)
	{
		q=Q.data[q.pre];
		m.maze[q.x][q.y]=3;
		Q.count_Q++;
	}
	Q.count_Q++;
//	cout<<"第"<<count<<"种走法为"<<endl;
	cout<<"基于队列实现的迷宫路径如下："<<endl;
	for(int i=0;i<Row;i++)
    {
		for(int j=0;j<Col;j++)
		{
			if(m.maze[i][j]==1)
				cout<<"▆";
			else
			{
				if(m.maze[i][j]==0||m.maze[i][j]==2)
			    	cout<<"  ";
				else
				{	if(m.maze[i][j]==3)
			        	cout<<"--";
				}
			}
		
		}
		cout<<endl;
	}
	cout<<"路径长度为"<<Q.count_Q<<endl;
}


/*void reset(Pathqueue &Q,Position &p)
{
	if(Q.front==Q.rear)
		cout<<"队列为空！"<<endl;
	else
	{
	//	LinkStackPtr a;
	//	a=s.top;
		p=Q.data[p.pre];
		Inqueue(Q,p);
		//p=s.top->Pdata;
		cout<<"????????";
	}

}*/

void Seekpath_Queue(Pathqueue &Q,Position p,Maze &m)
{
	
	while(Q.front!=Q.rear)
	{
		outqueue(Q,p);
		if(p.x==Col-2&&p.y==Row-2)
		{
		//	count++;
			m.maze[p.x][p.y]=3;
	       	printpath_Queue(Q,p,m);
		//	reset(Q,p);
		//	m.maze[p.x][p.y]=0;
            return;
	       	//exit(0);
		}
		if(m.maze[p.x][p.y+1]==0&&p.y+1<Col-1)
			store(Q,p.x,p.y+1,m);
		if(m.maze[p.x+1][p.y]==0&&p.x+1<Row-1)
			store(Q,p.x+1,p.y,m);
        if(m.maze[p.x][p.y-1]==0&&p.y-1>0)
 			store(Q,p.x,p.y-1,m);
        if(m.maze[p.x-1][p.y]==0&&p.x-1>0)
			store(Q,p.x-1,p.y,m);
	}
	cout<<"没有路径！"<<endl;
}