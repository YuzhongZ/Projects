#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include"Maze.h"

void InitMaze(Maze &m,int maze[][Col],int mark[][Col])
{
    
	for(int i=0;i<Row;i++)
		for(int j=0;j<Col;j++)
		{	
			m.maze[i][j]=maze[i][j];
		    m.mark[i][j]=mark[i][j];
		}
}
/*
void creatmaze(Maze &m)
{
//	Maze m;
	
	int en,i,j;
	int g[4];
	for(i=0;i<Row;i++)
		for(j=0;j<Col;j++)
		{
			m.mark[i][j]=1;
			m.maze[i][j]=1;
		}
	
	printmaze(m.maze);
	cout<<"?????????";
	Pathqueue Q;
		
	Initqueue(Q);

	Position p,v[4];
	
	p.x=1;
	p.y=1;
	
	Position up,down,left,right;
	Inqueue(Q,p);
	m.maze[1][1]=m.maze[Row-2][Col-2]=2;
	srand(unsigned(time(0)));
	while(p.x!=Row-2||p.y!=Col-2)
	{
	    int num=0,d=0;
		en=10;
		v[0]=v[1]=v[2]=v[3]=p;
		//Position *v;
	//	int num;
	//	outqueue(Q,p);
		if(m.maze[p.x][p.y+1]==1&&p.y+1<Col-1)
		{	
			right.x=p.x;
			right.y=p.y+1;
            right.pre=Q.rear;
			v[0]=right;
			Inqueue(Q,right);
		//	m.maze[right.x][right.y]=3;
			num++;
		}
		if(m.maze[p.x+1][p.y]==1&&p.x+1<Row-1)
		{	
			down.x=p.x+1;
			down.y=p.y;
            down.pre=Q.rear;
			v[1]=down;
		//	m.maze[down.x][down.y]=3;
			Inqueue(Q,down);p=down;
			num++;
		}	
        if(m.maze[p.x][p.y-1]==1&&p.y-1>0)
		{	
			left.x=p.x;
			left.y=p.y-1;
            left.pre=Q.rear;
			v[2]=left;
			//m.maze[left.x][left.y]=3;
			Inqueue(Q,left);p=left;
			num++;
		}	
        if(m.maze[p.x-1][p.y]==1&&p.x-1>0)
		{	
			up.x=p.x-1;
			up.y=p.y;
            up.pre=Q.rear;
			v[3]=up;
			//m.maze[up.x][up.y]=3;
			Inqueue(Q,up);p=up;
			num++;
		}
		if(num>0)
		{

			for(i=0;i<4;i++)
			{
				if(v[i].x!=p.x||v[i].y!=p.y)
				{
					g[d++]=i;
				}
			}
			//cout<<n<<endl;
		
        	en=rand()%d;
			switch(g[en])
			{
		    	case 0: {m.maze[right.x][right.y]=3;p=right;}break;
				case 1: {m.maze[down.x][down.y]=3;p=down;}break;//store(Q,p.x-1,p.y,m);
				case 2: {m.maze[left.x][left.y]=3;p=left;}break;//store(Q,p.x+1,p.y,m);break;
				case 3: {m.maze[up.x][up.y]=3;p=up;}break;//store(Q,p.x,p.y-1,m);break;
			}
			//p=v[en];
			cout<<en<<endl;
			//cout<<p.x<<p.y;
		//	p=Q.data[rand()%Q.rear];
		//	cout<<"hhh"<<rand()%Q.rear<<p.x<<p.y;


		}
		else
		/*{
			for(i=1;i<Row-1;i++)
			{
	        	for(j=1;j<Col-1;j++)
				{
					if(m.maze[i][j]==3)
					{
						p.x=i;
						p.y=j;
						break;
					}
				}
				break;
			}
		}
			
			p=Q.data[(rand()%Q.rear)+1];
	//	break;
		
		

	}
	if(p.x==Col-2&&p.y==Row-2)
	{
		//	count++;
		  for(i=1;i<Row-1;i++)
		  {	for(j=1;j<Col-1;j++)
			{
				m.maze[i][j]=(m.maze[i][j]==3)?0:1;
				m.mark[i][j]=1;
			}
		  }
          return;
	       	//exit(0);
	}
	
	
}
*/


void printmaze(int maze[][Col])  
{
	for(int i=0;i<Row;i++)	
	{	
		for(int j=0;j<Col;j++)
		{
     		if(maze[i][j]==1)
				cout<<"▆";
			else
				cout<<"  ";
		}
		cout<<endl;
	}
	return;

}

void resetmaze(Maze &m,int maze[][Col],int mark[][Col])
{
	for(int i=0;i<Row;i++)
		for(int j=0;j<Col;j++)
		{	
			m.maze[i][j]=maze[i][j];
		    m.mark[i][j]=mark[i][j];
		}

}
/*void resetmaze(Maze &m,int maze[][50],int mark[][50])
{
	for(int i=0;i<Row;i++)
		for(int j=0;j<Col;j++)
		{	
			m.maze[i][j]=maze[i][j];
		    m.mark[i][j]=mark[i][j];
		}

}*/

void main()
{
    int i,j;
	Maze m;
	
//	cout<<"请输入迷宫行数和列数（小于50）：";
//	cin>>Col>>Row;
	
	
//	m.mark[Row][Col];
	//m.maze[Row][Col]={{0},{0}};
//	creatmaze(m);
	
	//int maze[Row][Col];
	//maze=m.maze;
	
	
	
	
/*	srand(unsigned(time(0)));
	for(i=1;i<Row-1;i++)
		for(j=1;j<Col-1;j++)
		{
	    	maze[i][j]=rand()%2;
			mark[i][j]=0;	
		}
    maze[Row-2][Col-2]=maze[1][1]=maze[1][2]=maze[2][1]=maze[Row-3][Col-2]=maze[Row-2][Col-3]=maze[(Row+1)/2][(Col+1)/2]=0;*/
	int maze[Row][Col]={
		{1,1,1,1,1,1,1,1,1,1},
		{1,0,0,1,0,0,0,1,0,1},
		{1,0,0,1,0,0,0,1,0,1},
		{1,0,0,0,0,1,0,0,0,1},
		{1,0,1,1,0,1,0,0,0,1},
		{1,0,0,1,0,0,0,1,0,1},
		{1,0,1,1,0,0,1,0,0,1},
		{1,0,1,1,1,0,1,1,0,1},
		{1,1,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1}};
	int mark[Row][Col];
	for(j=0;j<Row;j++)
	{	
		mark[0][j]=mark[Col-1][j]=1;
		//maze[0][j]=maze[Col-1][j]=1;
	}
	for(i=0;i<Col;i++)
	{
		mark[i][0]=mark[i][Row-1]=1;
		//maze[i][0]=maze[i][Row-1]=1;
	}
	for(i=1;i<Row-1;i++)
		for(j=1;j<Col-1;j++)
		{
			mark[i][j]=0;
			//maze[i][j]=m.maze[i][j];
		}
    Position start={1,1,-1};
	m.maze[start.x][start.y]=3;
	InitMaze(m,maze,mark);
	cout<<endl;
	printmaze(maze);
	cout<<endl;
	LinkStack s;
	SeekPath_Stack(s,m,start);
    cout<<endl;
	resetmaze(m,maze,mark);
	Pathqueue Q;
	Initqueue(Q);
	Inqueue(Q,start);
	Seekpath_Queue(Q,start,m);
	cout<<endl;
	if(Q.count_Q<s.count_S&&Q.count_Q&&s.count_S)
		cout<<"基于队列搜索所得路径短！"<<endl;
	else
		if(Q.count_Q>s.count_S&&Q.count_Q&&s.count_S)
			cout<<"基于栈搜索所得路径短！"<<endl;
		else
			if(Q.count_Q&&s.count_S)
		    	cout<<"两种搜索方法路径一样长！"<<endl;

}


