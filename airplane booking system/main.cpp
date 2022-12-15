#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<iomanip>
#include<fstream.h>
static int num=6;


//***********************class flight************************

class flight{
public:
    char start_time[20];   //起飞时间	
	char start_place[20];  //起飞地点
	char end_place[20];    //抵达地点
	int price;             //票价
	char flight_num[20];   //航班号
	char flight_data[10];  //航班日期
	int piaoshu;           //剩余票数


	//带缺省参数的构造函数
    flight(char *num=0,char *d=0,char *st=0,char *sp=0,char *ep=0,int n=0,int p=0)
	{
    	if(num)   strcpy(flight_num,num);
		if(d)     strcpy(flight_data,d);
     	if(st)    strcpy(start_time,st);
    	if(sp)    strcpy(start_place,sp); 
		if(ep)    strcpy(end_place,ep);
    	piaoshu=n;        
		price=p;
	}

    int find(flight F[]);             //查询函数
	int finddata(flight F[]);         //按日期查询
	int findplace(flight F[]);        //按起始地点查找航班
	int findnum(flight F[]);          //按航班号查找航班
	flight &operator=(flight &f);     //运算符重载= 直接赋值
	void print();
};


//输出航班信息
void flight::print()
{
    cout<<"航班号"<<"   "<<"日期"<<"  "<<"起点"<<"  "<<"终点"<<"  "<<"起飞时间"<<"  "<<"剩余票数"<<"  "<<"票价"<<endl;
	cout<<flight_num<<"        "<<flight_data<<"  "<<start_place<<"  "<<end_place<<"   ";
	cout<<start_time<<"      "<<piaoshu<<"       "<<price<<endl;
	cout<<endl;
}


//按日期查找
int flight::finddata(flight F[])      
{	 
	char x[20];
	int flag=-1;
	cout<<"请输入要查询的起飞日期：";
	cin>>x;
    for(int i=0;i<num;i++)
	{
		if(strcmp(x,F[i].flight_data)==0)
		{   flag=100;
			F[i].print();
		}
	}
    return flag;
}


//按地点查找
int flight::findplace(flight F[])      
{	 
	char x[20],y[20];
	int flag=-1;
	cout<<"请输入起飞城市：";
	cin>>x;
    cout<<"请输入抵达城市：";
	cin>>y;
	for(int i=0;i<num;i++)
	{
		if(strcmp(x,F[i].start_place)==0&&strcmp(y,F[i].end_place)==0)
		{   flag=100;
			F[i].print();
		}
	}
    return flag;
}


//按航班号查找
int flight::findnum(flight F[])      
{	char n[15];
    cout<<"请输入要查询的航班号：";
    cin>>n;
	for(int i=0;i<num;i++)
	{
		if(strcmp(n,F[i].flight_num)==0)
		{  F[i].print();
		   return i;
		}
   	}
    return -1;
}


//查询航班菜单
int flight::find(flight F[])         
{
	int opp=0,i=-1;
	cout<<endl;
	cout<<"********************************************"<<endl;
	cout<<endl;
	cout<<"   1、按航班号查询  2、按起飞抵达城市查询   "<<endl;
	cout<<"   3、按日期查询                            "<<endl;
	cout<<endl;
	cout<<"********************************************"<<endl;
	cout<<"请输入操作数(1-3)："<<endl;
	cin>>opp;
	if(opp>0&&opp<=3)
	{   switch(opp)
		{   case 1:i=findnum(F);break;
		    case 2:i=findplace(F);break;
			case 3:i=finddata(F);break;
		}
	}
	else
		cout<<"输入有误！"<<endl;
	return i;
}


//运算符重载= 直接赋值
flight &flight::operator=(flight &f)      
{  
	if(f.flight_num)    strcpy(flight_num,f.flight_num);
	if(f.flight_data) 	strcpy(flight_data,f.flight_data);   
	if(f.start_time)  	strcpy(start_time,f.start_time);      
	if(f.start_place)  	strcpy(start_place,f.start_place);      
	if(f.end_place)   	strcpy(end_place,f.end_place);
   	piaoshu=f.piaoshu;       
	price=f.price;
	return *this;
}


//***********************class person************************


class person{
	char *name;        //姓名
	char tel[15];      //电话号码
	char ID[20];       //身份证	
public:
	flight own;        //已购航班
    person *link;      //指针域

	//构造
	person(char *n=0,char *t=0,char *id=0)
	{
    	if(n)  
		{
	    	name=new char[strlen(n)+1];
	    	strcpy(name,n);
		}
    	else
	    	name=0;
     	if(t)  strcpy(tel,t);
    	if(id)  strcpy(ID,id);
	}

	//拷贝
	void copy(person L)
	{   
		if(L.name)    
		{
	    	name=new char[strlen(L.name)+1];
	    	strcpy(name,L.name);
		}
    	else
	    	name=0;
     	if(L.tel)  strcpy(tel,L.tel);
    	if(L.ID)  strcpy(ID,L.ID);
		
	}
    int dingpiao(flight F[]);       //订票
	void gaiqian(flight F[]);       //改签
	void tuipiao(flight F[]);       //退票
	void printperson();             //输出
	char *Getname() {return name;}
};


//订票
int person::dingpiao(flight F[])           
{
	int j,k,p=0;
	char s[20];
    j=own.find(F);
    if(j>=0)
	{   
     	cout<<"以上为您所查询航班信息"<<endl;
		cout<<"请问是否订票(输入操作数)";
       	cout<<"    1、是   2、否 ：";
        cin>>k;
  	 	switch(k)
		{
	    	case 1:{ 
			    	 if(j==100)
					 {   cout<<"请输入确定要购买的航班号";
			        	 cin>>s;
			             for(p=0;p<num;p++)
			                if(strcmp(s,F[p].flight_num)==0)
						        j=p; 
					 }
					 cout<<endl;
				     if(F[j].piaoshu)
					 {
						 cout<<"请输入一个乘坐人姓名、电话号码、身份证号:";
	                     name=new char[20];
    		     	     cin>>name>>tel>>ID;
			        	 F[j].piaoshu--;
			        	 own=F[j];
			             cout<<endl;
			         	 cout<<"订票成功！"<<endl;
			         	 printperson();
					 }
			    	 else
					 {   cout<<"该航班已无余票！"<<endl;
					     return -1;
					 }
					 }break;
            case 2: 
			default:return -1;break;
		}
	}
	else
		cout<<"没有您所需要的航班！"<<endl;
	return j;
}


//输出订单信息
void person::printperson()            
{
	cout<<"姓名："<<name<<"  "<<"手机号：";
	cout<<tel<<"  "<<"身份证号："<<ID<<endl;
	cout<<"已订航班："<<endl;
	own.print();
	cout<<endl;
}


//改签
void person::gaiqian(flight F[])      
{
	int j,k,temp;
	char s[20];
	for(int r=0;r<num;r++)
		if(strcmp(F[r].flight_num,own.flight_num)==0)
             temp=r;
	j=own.find(F);
    if(j>=0)
	{   
     	cout<<"以上为您所查询航班信息"<<endl;
		cout<<"请问是否改签(输入操作数)";
       	cout<<"    1、是   2、否 ：";
        cin>>k;
  	 	switch(k)
		{
	    	case 1:{ 
				     if(j==100)
					 {   cout<<"请输入确定要改签的航班号";
			        	 cin>>s;
			             for(int p=0;p<num;p++)
						   if(strcmp(s,F[p].flight_num)==0)
								j=p; 
					 }
				     if(F[j].piaoshu)
					 {	 F[temp].piaoshu++;
			        	 F[j].piaoshu--;
			        	 own=F[j];
			         	 cout<<"改签成功！"<<endl;
						 cout<<endl;
			         	 printperson();
					 }
			    	 else
					   cout<<"该航班已无余票！"<<endl;
				 }break;
            case 2: break;
		}
	}
	else
		cout<<"没有您所需要的航班,改签失败！"<<endl;
}


//退票
void person::tuipiao(flight F[])       
{
	for(int r=0;r<num;r++)
	{	if(strcmp(own.flight_num,F[r].flight_num)==0)
        	F[r].piaoshu++;
	}
}


//************************class book****************************

class book
{   person *head;                    //订单链表
    person *p1;                      //辅助插入订单指针
	int size;
public:
	book(int s=0)  {size=s;}         //构造函数
    int insert(flight F[]);          //增加订单信息
	void printlist();                //输出订单信息
	void dellist(flight F[]);        //删除订单
	void modify(flight F[]);         //修改订单
};


//订票-新增订单
int book::insert(flight F[]){
	person *p2,*p3=p1;
	int q;
	if(size==0)
	{   
		p1=new person;
    	q=p1->dingpiao(F);
        if(q>=0)
		{
			head=p1;
		    p1->link=NULL;	
			size++;
		}
	}
	else
	{   
		p2=new person;
		q=p2->dingpiao(F);
		if(q>=0)
		{
			p1->link=p2;	
	    	p1=p1->link;
		    p1->link=NULL;
			size++;
		}
	}
    return size;
}


//退票-删除订单
void book::dellist(flight F[]){ 
	if(size<=0)      
	{	cout<<"订单为空，不能退票"<<endl;
    	return;
	}
	person *p2=head,*p3;
	char n[20],h[20];
	cout<<"请输入退票者姓名：";
	cin>>n;
	cout<<"航班号：";
	cin>>h;	
	if(strcmp(n,head->Getname())==0&&strcmp(h,head->own.flight_num)==0)
	{
		head->tuipiao(F);
		head=head->link;
		size--;        
	    cout<<"退票成功！"<<endl;
		return;
	}
   	p3=p2->link;
	while(p3)
	{
		if(strcmp(n,p3->Getname())==0&&strcmp(h,p3->own.flight_num)==0)
		{
	        p3->tuipiao(F);
			p2->link=p3->link;
	    	size--;        
	    	cout<<"退票成功！"<<endl;
	     	return;
		}
		p2=p3;
		p3=p3->link;
		
	}
	cout<<"没有找到该退票信息！"<<endl;
		
}


//改签-修改订单
void book::modify(flight F[])          
{
	if(size<=0)      
	{	cout<<"订单为空，不能改签！"<<endl;
    	return;
	}
	person *p2=head;
	char n[20],h[20];
	cout<<"请输入要改签的乘坐人姓名：";
	cin>>n;
	cout<<"航班号：";
	cin>>h;
	while(p2)
	{
		if(strcmp(n,p2->Getname())==0&&strcmp(h,p2->own.flight_num)==0)
		{
	        p2->gaiqian(F);
	     	return;
		}
		p2=p2->link;
	}
	cout<<"没有找到该改签信息！"<<endl;
}


//打印所有订单信息
void book::printlist()               
{
	if(size<=0)
		cout<<"暂无订单！"<<endl;
	else
	{
    	cout<<"所有订单信息为："<<endl;
    	cout<<endl;
        person *p2=head;
    	while(p2)
		{
    		p2->printperson();
     		p2=p2->link;
		}
	}
}



//************************others****************************

//总菜单
void menu()
{
	cout<<endl;
	cout<<endl;
    cout<<"************欢迎来到飞机订票系统************"<<endl;
	cout<<"********************************************"<<endl;
	cout<<endl;
	cout<<"       1、以管理员身份进入    2、订票业务   "<<endl;
	cout<<"       3、退票业务            4、改签业务   "<<endl;
	cout<<"       5、查看航班            6、查看订单   "<<endl;
	cout<<"       7、退出系统                          "<<endl;
    cout<<endl;
	cout<<"********************************************"<<endl;
	cout<<"请输入操作数(1-6)：";
}


//按日期输出所有航班信息
void dataprint(flight a[])          
{
	flight t,*p=a;
	for(int j=1;j<num;j++)
		for(int i=0;i<=num-j;i++)
		{	if(strcmp(p[i].flight_data,p[i+1].flight_data)>0)
			{
				t=p[i];
				p[i]=p[i+1];
				p[i+1]=t;
			}
		}
	for(int k=0;k<num;k++)
		p[k].print();
}

//直接输出所有航班信息
void allprint(flight a[])          
{
	for(int k=0;k<num;k++)
		a[k].print();
}


//查询航班信息菜单
void flightprint(flight a[])        
{
	int op;
	cout<<"********************************************"<<endl;
	cout<<endl;
	cout<<"       1、按日期顺序查看（从早到晚）        "<<endl;
	cout<<"       2、查看所有航班（无序）              "<<endl;
	cout<<endl;
	cout<<"********************************************"<<endl;
	cout<<"请输入操作数(1-2)：";
	cin>>op;
	if(op>0&&op<=2)
	{   switch(op)
		{   case 1:dataprint(a);break;
			case 2:allprint(a);break;
		}
	}
}


//******************管理员**********************************


//增添航班信息
void add(flight u[])
{
	int n;
	cout<<"请输入要增加的航班数：";
	cin>>n;
	for(int x=0;x<n;x++)
	{
	cout<<"请输入要增加的航班号、日期和起飞时间：";
	cin>>u[num].flight_num>>u[num].flight_data>>u[num].start_time;
	cout<<"起飞城市和抵达城市：";
	cin>>u[num].start_place>>u[num].end_place;
	cout<<"剩余票数和票价：";
    cin>>u[num].piaoshu>>u[num].price;
	ofstream outfile("hangbang.txt",ios::app);
	outfile<<u[num].flight_num<<"  "<<u[num].flight_data<<"  "<<u[num].start_time<<"  "
	       <<u[num].start_place<<"  "<<u[num].end_place<<"  "<<u[num].piaoshu<<"  "<<u[num].price<<'\n';
	outfile.close();
	num++;
	cout<<endl;
	}
	cout<<"增添成功！"<<endl;
	
}


//删除航班信息
void del(flight u[])
{
	int w;
	char n[20];
	cout<<"请输入要删除的航班数目：";
	cin>>w;
	for(int x=0;x<w;x++)
	{
		int flag=0,i=0;
		cout<<endl;
    	cout<<"请输入要删除的航班号：";
        cin>>n;
    	while(i<num)
		{
    		if(strcmp(n,u[i].flight_num)==0)
			{   
	    		for(int j=i;j<num-1;j++)
		        	u[j]=u[j+1];
		    	num--;
		     	cout<<"删除成功！"<<endl;
		    	flag=1;
				break;
			}
			i++;
		}
    	if(!flag)
            cout<<"没有该航班！"<<endl;
	}
}


//管理员菜单
void guanli_menu()
{
     	cout<<endl;
    	cout<<"***************管理员***********************"<<endl;
    	cout<<endl;
    	cout<<"       1、增添航班     2、删除航班          "<<endl;
     	cout<<"       3、查看所有航班                      "<<endl;
    	cout<<"       4、退出管理员                         "<<endl;
    	cout<<endl;
     	cout<<"********************************************"<<endl;
		cout<<"请输入操作数(1-3)：";
}

//管理员功能选择
void guanliyuan(flight u[])
{
	char x[20];
	cout<<"请输入管理员口令：";
	cin>>x;
	if(strcmp(x,"hh666")==0)
	{
		
    	int op;
     	guanli_menu();
       	cin>>op;
    	while(op>0&&op<=3)
		{  
			switch(op)
			{   case 1:add(u);break;
		     	case 2:del(u);break;
		    	case 3:flightprint(u);break;
			}
			
			system("PAUSE");
         	system("CLS");

	    	guanli_menu();
		    cin>>op;
		}
	}
	else
		cout<<"口令错误！"<<endl;
}


//***********************main****************************

void main()
{   
	flight air[50];
	int op;
	ifstream infile("hangbang.txt");
	for(int k=0;k<num;k++)
	{
		infile>>air[k].flight_num>>air[k].flight_data>>air[k].start_time>>air[k].start_place>>air[k].end_place>>air[k].piaoshu>>air[k].price;
	}
	infile.close();
    
	book A;
	menu();
	cin>>op;
	while(op>=0&&op<=6)
	{
		switch(op)
		{
			case 1:guanliyuan(air);break;
	    	case 2:A.insert(air);break;
	     	case 3:A.dellist(air);break;
	     	case 4:A.modify(air);break;
			case 5:flightprint(air);break;
	    	case 6:A.printlist();break;
		}

    	system("PAUSE");
    	system("CLS");

		menu();
		cin>>op;	
	}
	
}
	




