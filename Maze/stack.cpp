#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#include"Maze.h"


	
void InitStack(LinkStack &s)
{
	s.top=NULL;
    s.count_S=0;
}

void Push(LinkStack &s,Position p)
{
    StackNode *a;
	a=(StackNode*)malloc(sizeof(StackNode));
	a->Pdata=p;
	a->next=s.top;
	s.top=a;
	s.count_S++;
}

void Pop(LinkStack &s,Position p)
{
	LinkStackPtr a;
	if(s.top==NULL)
    	cout<<"没有路径！";
		//exit(0);
	else
	{
		p=s.top->Pdata;
		a=s.top;
		s.top=a->next;
	//	s.top--;
		free(a);
		s.count_S--;
	}
}

void Gettop(LinkStack s,Position &p)
{
	if(s.top==NULL)
		cout<<"没有路径！";
	else
		p=s.top->Pdata;
}





void printpath_Stack(LinkStack s,Maze &m)
{
//	cout<<"第"<<num<<"种走法为"<<endl;
	cout<<"基于栈实现的迷宫路径如下："<<endl;
	while(s.top!=NULL)
	{
		m.maze[s.top->Pdata.x][s.top->Pdata.y]=3;
       // cout<<s.top->Pdata.x<<"  "<<s.top->Pdata.y<<endl;
     	s.top=s.top->next;
	
	}
	for(int i=0;i<Row;i++)	
	{
		for(int j=0;j<Col;j++)
		{
        	if(m.maze[i][j]==1)
				cout<<"▆";
			else
			{
				if(m.maze[i][j]==0)
			    	cout<<"  ";
				else
				{	
					if(m.maze[i][j]==3)
			        	cout<<"--";
				}
			}			
		}
		cout<<endl;
	}
	cout<<"路径长度为"<<s.count_S<<endl;
}


/*void reset(LinkStack &s,Position &p)
{
	if(s.top==NULL)
		cout<<"栈为空！";
	else
	{
	//	LinkStackPtr a;
	//	a=s.top;
		s.top--;
		p=s.top->Pdata;
		cout<<"????????";
	}





}*/

void SeekPath_Stack(LinkStack &s,Maze &m,Position p)
{
	int dir=0;
	InitStack(s);
	Position a=p;
    Push(s,p);
	m.mark[p.x][p.y]=2;
	a.y=p.y+1;
	while(s.top!=NULL)
	{
		if(m.maze[a.x][a.y]==0&&m.mark[a.x][a.y]==0)
		{

	    	Push(s,a);
			m.mark[a.x][a.y]=2;
			dir=1;
			if(a.x==Col-2&&a.y==Row-2)
			{
	        	printpath_Stack(s,m);
	         	return;
			}
			else
			{	Gettop(s,p);
			    a=p;
				a.y=p.y+1;
				dir=1;
			}
		}
	    else
		{
			m.mark[a.x][a.y]=2;
			if(s.top!=NULL&&dir<4)
			{
	     		switch(dir)
				{
				case 3: {a.x=p.x-1;a.y=p.y;}break;
				case 1: {a.x=p.x+1;a.y=p.y;}break;
				case 2: {a.x=p.x;a.y=p.y-1;}break;
				}
  		    	dir++;
			}
	    	else
			{
	    		if(s.top!=NULL)
				{
					Pop(s,p);
		    		Gettop(s,p);
		     		a=p;
		    		a.y=p.y+1;
		     		dir=1;
				}	
			}
		}
	}
}
			
