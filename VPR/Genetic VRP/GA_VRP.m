%%��ϸ����⽨����VRPģ�͵ļ��裬
%%������Ҫ����һ�µ�������ÿ�������̵�����������㣬��ֻ����һ̨���ͳ����ͻ���
%%�����Կͻ���ŵ�������б��룬��֤ÿ���ͻ��������ֻ�ܳ���һ�Ρ�
%%���ݽ�������ĳ�����ʻ·��������Ŀ��ֵ��Ϊ����ʻ���롣
%%����Ҫ�жϽ����������ʻ·���������Ƿ񳬹��ܳ���������δ���������г���·����ӵõ���������̣�
%%�������ܳ���������˵��������·�������С�

%%����������1�б�ʾ�ͻ���ţ���2�б�ʾ�����ı�ţ�
%%��3�б�ʾ�����ڸ�·���У����Ӹ��пͻ������������ĵľ��룻��4�б�ʾ�����ڸ�·���е��ػ�����
clc
clear
close all;
tic;
%%
%�㷨����
population_num=80;%��Ⱥ��ģ
Max_gen=200;%��������
Pc=0.9;%�������
Pm=0.09;%�������
%ģ��ѡ�ò���
%��������Car_num=2
%�ͻ�����Customer_num=8
%��������capacity_max=8
%��ʻ����distance_max=50
Car_num=8;
Customer_num=30;
capacity_max=800;
distance_max=500;
%load Demand %�ͻ�������
%load Distance %�ͻ���ľ���
%load X %�������ĵ��ͻ���ľ���
%%
%��Ⱥ��ʼ��
population=zeros(population_num,Customer_num);
for i=1:population_num
     population(i,:)=randperm(Customer_num);
end
%%
y=1;%ѭ��������
 while y<Max_gen
     %����
     [new_pop_intercross]=Mating_pool(population_num,population,Pc);
     %����
     [new_pop_mutation]=Mutation(new_pop_intercross,Pm);
     %����Ŀ�꺯��
     mutation_num=size(new_pop_mutation,1);
     Total_Dis=[];
     for k=1:mutation_num
     [Result]=decode(new_pop_mutation(k,:),distance_max,capacity_max);
     [Total_Dis(k,1)]=parameter(Result,Customer_num,Car_num);
     end
     %������Ⱥ
     new_pop_new=zeros(population_num,Customer_num);
     [Total_Dissort, index] = sort(Total_Dis);
     for k=1:population_num
         new_pop_new(k,:)=new_pop_mutation(index(k),:);
     end
     population=new_pop_new;
     %����������һ
     y=y+1;
 end
 Dis_min1=min(Total_Dis);
for k=1:mutation_num
    if Total_Dis(k,1)==Dis_min1
       position1= k;
       break
    end
end
X_Best=new_pop_mutation(position1,:)
Y_Obj=Total_Dis(position1,1)
t=toc;
T=X_Best;
%distance_max=50;
%capacity_max=8;
[Result]=decode(T,distance_max,capacity_max)
