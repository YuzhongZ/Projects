#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<iomanip>
#include<fstream.h>
static int num=6;


//***********************class flight************************

class flight{
public:
    char start_time[20];   //���ʱ��	
	char start_place[20];  //��ɵص�
	char end_place[20];    //�ִ�ص�
	int price;             //Ʊ��
	char flight_num[20];   //�����
	char flight_data[10];  //��������
	int piaoshu;           //ʣ��Ʊ��


	//��ȱʡ�����Ĺ��캯��
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

    int find(flight F[]);             //��ѯ����
	int finddata(flight F[]);         //�����ڲ�ѯ
	int findplace(flight F[]);        //����ʼ�ص���Һ���
	int findnum(flight F[]);          //������Ų��Һ���
	flight &operator=(flight &f);     //���������= ֱ�Ӹ�ֵ
	void print();
};


//���������Ϣ
void flight::print()
{
    cout<<"�����"<<"   "<<"����"<<"  "<<"���"<<"  "<<"�յ�"<<"  "<<"���ʱ��"<<"  "<<"ʣ��Ʊ��"<<"  "<<"Ʊ��"<<endl;
	cout<<flight_num<<"        "<<flight_data<<"  "<<start_place<<"  "<<end_place<<"   ";
	cout<<start_time<<"      "<<piaoshu<<"       "<<price<<endl;
	cout<<endl;
}


//�����ڲ���
int flight::finddata(flight F[])      
{	 
	char x[20];
	int flag=-1;
	cout<<"������Ҫ��ѯ��������ڣ�";
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


//���ص����
int flight::findplace(flight F[])      
{	 
	char x[20],y[20];
	int flag=-1;
	cout<<"��������ɳ��У�";
	cin>>x;
    cout<<"������ִ���У�";
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


//������Ų���
int flight::findnum(flight F[])      
{	char n[15];
    cout<<"������Ҫ��ѯ�ĺ���ţ�";
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


//��ѯ����˵�
int flight::find(flight F[])         
{
	int opp=0,i=-1;
	cout<<endl;
	cout<<"********************************************"<<endl;
	cout<<endl;
	cout<<"   1��������Ų�ѯ  2������ɵִ���в�ѯ   "<<endl;
	cout<<"   3�������ڲ�ѯ                            "<<endl;
	cout<<endl;
	cout<<"********************************************"<<endl;
	cout<<"�����������(1-3)��"<<endl;
	cin>>opp;
	if(opp>0&&opp<=3)
	{   switch(opp)
		{   case 1:i=findnum(F);break;
		    case 2:i=findplace(F);break;
			case 3:i=finddata(F);break;
		}
	}
	else
		cout<<"��������"<<endl;
	return i;
}


//���������= ֱ�Ӹ�ֵ
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
	char *name;        //����
	char tel[15];      //�绰����
	char ID[20];       //���֤	
public:
	flight own;        //�ѹ�����
    person *link;      //ָ����

	//����
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

	//����
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
    int dingpiao(flight F[]);       //��Ʊ
	void gaiqian(flight F[]);       //��ǩ
	void tuipiao(flight F[]);       //��Ʊ
	void printperson();             //���
	char *Getname() {return name;}
};


//��Ʊ
int person::dingpiao(flight F[])           
{
	int j,k,p=0;
	char s[20];
    j=own.find(F);
    if(j>=0)
	{   
     	cout<<"����Ϊ������ѯ������Ϣ"<<endl;
		cout<<"�����Ƿ�Ʊ(���������)";
       	cout<<"    1����   2���� ��";
        cin>>k;
  	 	switch(k)
		{
	    	case 1:{ 
			    	 if(j==100)
					 {   cout<<"������ȷ��Ҫ����ĺ����";
			        	 cin>>s;
			             for(p=0;p<num;p++)
			                if(strcmp(s,F[p].flight_num)==0)
						        j=p; 
					 }
					 cout<<endl;
				     if(F[j].piaoshu)
					 {
						 cout<<"������һ���������������绰���롢���֤��:";
	                     name=new char[20];
    		     	     cin>>name>>tel>>ID;
			        	 F[j].piaoshu--;
			        	 own=F[j];
			             cout<<endl;
			         	 cout<<"��Ʊ�ɹ���"<<endl;
			         	 printperson();
					 }
			    	 else
					 {   cout<<"�ú���������Ʊ��"<<endl;
					     return -1;
					 }
					 }break;
            case 2: 
			default:return -1;break;
		}
	}
	else
		cout<<"û��������Ҫ�ĺ��࣡"<<endl;
	return j;
}


//���������Ϣ
void person::printperson()            
{
	cout<<"������"<<name<<"  "<<"�ֻ��ţ�";
	cout<<tel<<"  "<<"���֤�ţ�"<<ID<<endl;
	cout<<"�Ѷ����ࣺ"<<endl;
	own.print();
	cout<<endl;
}


//��ǩ
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
     	cout<<"����Ϊ������ѯ������Ϣ"<<endl;
		cout<<"�����Ƿ��ǩ(���������)";
       	cout<<"    1����   2���� ��";
        cin>>k;
  	 	switch(k)
		{
	    	case 1:{ 
				     if(j==100)
					 {   cout<<"������ȷ��Ҫ��ǩ�ĺ����";
			        	 cin>>s;
			             for(int p=0;p<num;p++)
						   if(strcmp(s,F[p].flight_num)==0)
								j=p; 
					 }
				     if(F[j].piaoshu)
					 {	 F[temp].piaoshu++;
			        	 F[j].piaoshu--;
			        	 own=F[j];
			         	 cout<<"��ǩ�ɹ���"<<endl;
						 cout<<endl;
			         	 printperson();
					 }
			    	 else
					   cout<<"�ú���������Ʊ��"<<endl;
				 }break;
            case 2: break;
		}
	}
	else
		cout<<"û��������Ҫ�ĺ���,��ǩʧ�ܣ�"<<endl;
}


//��Ʊ
void person::tuipiao(flight F[])       
{
	for(int r=0;r<num;r++)
	{	if(strcmp(own.flight_num,F[r].flight_num)==0)
        	F[r].piaoshu++;
	}
}


//************************class book****************************

class book
{   person *head;                    //��������
    person *p1;                      //�������붩��ָ��
	int size;
public:
	book(int s=0)  {size=s;}         //���캯��
    int insert(flight F[]);          //���Ӷ�����Ϣ
	void printlist();                //���������Ϣ
	void dellist(flight F[]);        //ɾ������
	void modify(flight F[]);         //�޸Ķ���
};


//��Ʊ-��������
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


//��Ʊ-ɾ������
void book::dellist(flight F[]){ 
	if(size<=0)      
	{	cout<<"����Ϊ�գ�������Ʊ"<<endl;
    	return;
	}
	person *p2=head,*p3;
	char n[20],h[20];
	cout<<"��������Ʊ��������";
	cin>>n;
	cout<<"����ţ�";
	cin>>h;	
	if(strcmp(n,head->Getname())==0&&strcmp(h,head->own.flight_num)==0)
	{
		head->tuipiao(F);
		head=head->link;
		size--;        
	    cout<<"��Ʊ�ɹ���"<<endl;
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
	    	cout<<"��Ʊ�ɹ���"<<endl;
	     	return;
		}
		p2=p3;
		p3=p3->link;
		
	}
	cout<<"û���ҵ�����Ʊ��Ϣ��"<<endl;
		
}


//��ǩ-�޸Ķ���
void book::modify(flight F[])          
{
	if(size<=0)      
	{	cout<<"����Ϊ�գ����ܸ�ǩ��"<<endl;
    	return;
	}
	person *p2=head;
	char n[20],h[20];
	cout<<"������Ҫ��ǩ�ĳ�����������";
	cin>>n;
	cout<<"����ţ�";
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
	cout<<"û���ҵ��ø�ǩ��Ϣ��"<<endl;
}


//��ӡ���ж�����Ϣ
void book::printlist()               
{
	if(size<=0)
		cout<<"���޶�����"<<endl;
	else
	{
    	cout<<"���ж�����ϢΪ��"<<endl;
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

//�ܲ˵�
void menu()
{
	cout<<endl;
	cout<<endl;
    cout<<"************��ӭ�����ɻ���Ʊϵͳ************"<<endl;
	cout<<"********************************************"<<endl;
	cout<<endl;
	cout<<"       1���Թ���Ա��ݽ���    2����Ʊҵ��   "<<endl;
	cout<<"       3����Ʊҵ��            4����ǩҵ��   "<<endl;
	cout<<"       5���鿴����            6���鿴����   "<<endl;
	cout<<"       7���˳�ϵͳ                          "<<endl;
    cout<<endl;
	cout<<"********************************************"<<endl;
	cout<<"�����������(1-6)��";
}


//������������к�����Ϣ
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

//ֱ��������к�����Ϣ
void allprint(flight a[])          
{
	for(int k=0;k<num;k++)
		a[k].print();
}


//��ѯ������Ϣ�˵�
void flightprint(flight a[])        
{
	int op;
	cout<<"********************************************"<<endl;
	cout<<endl;
	cout<<"       1��������˳��鿴�����絽��        "<<endl;
	cout<<"       2���鿴���к��ࣨ����              "<<endl;
	cout<<endl;
	cout<<"********************************************"<<endl;
	cout<<"�����������(1-2)��";
	cin>>op;
	if(op>0&&op<=2)
	{   switch(op)
		{   case 1:dataprint(a);break;
			case 2:allprint(a);break;
		}
	}
}


//******************����Ա**********************************


//��������Ϣ
void add(flight u[])
{
	int n;
	cout<<"������Ҫ���ӵĺ�������";
	cin>>n;
	for(int x=0;x<n;x++)
	{
	cout<<"������Ҫ���ӵĺ���š����ں����ʱ�䣺";
	cin>>u[num].flight_num>>u[num].flight_data>>u[num].start_time;
	cout<<"��ɳ��к͵ִ���У�";
	cin>>u[num].start_place>>u[num].end_place;
	cout<<"ʣ��Ʊ����Ʊ�ۣ�";
    cin>>u[num].piaoshu>>u[num].price;
	ofstream outfile("hangbang.txt",ios::app);
	outfile<<u[num].flight_num<<"  "<<u[num].flight_data<<"  "<<u[num].start_time<<"  "
	       <<u[num].start_place<<"  "<<u[num].end_place<<"  "<<u[num].piaoshu<<"  "<<u[num].price<<'\n';
	outfile.close();
	num++;
	cout<<endl;
	}
	cout<<"����ɹ���"<<endl;
	
}


//ɾ��������Ϣ
void del(flight u[])
{
	int w;
	char n[20];
	cout<<"������Ҫɾ���ĺ�����Ŀ��";
	cin>>w;
	for(int x=0;x<w;x++)
	{
		int flag=0,i=0;
		cout<<endl;
    	cout<<"������Ҫɾ���ĺ���ţ�";
        cin>>n;
    	while(i<num)
		{
    		if(strcmp(n,u[i].flight_num)==0)
			{   
	    		for(int j=i;j<num-1;j++)
		        	u[j]=u[j+1];
		    	num--;
		     	cout<<"ɾ���ɹ���"<<endl;
		    	flag=1;
				break;
			}
			i++;
		}
    	if(!flag)
            cout<<"û�иú��࣡"<<endl;
	}
}


//����Ա�˵�
void guanli_menu()
{
     	cout<<endl;
    	cout<<"***************����Ա***********************"<<endl;
    	cout<<endl;
    	cout<<"       1��������     2��ɾ������          "<<endl;
     	cout<<"       3���鿴���к���                      "<<endl;
    	cout<<"       4���˳�����Ա                         "<<endl;
    	cout<<endl;
     	cout<<"********************************************"<<endl;
		cout<<"�����������(1-3)��";
}

//����Ա����ѡ��
void guanliyuan(flight u[])
{
	char x[20];
	cout<<"���������Ա���";
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
		cout<<"�������"<<endl;
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
	




