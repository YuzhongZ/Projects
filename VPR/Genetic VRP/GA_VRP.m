%%结合该问题建立的VRP模型的假设，
%%我们需要满足一下的条件：每个零售商的需求必须满足，且只能由一台配送车辆送货，
%%可以以客户编号的排序进行编码，保证每个客户编号能且只能出现一次。
%%根据解码出来的车辆行驶路径，计算目标值即为总行驶距离。
%%但需要判断解码出来的行驶路径的数量是否超过总车辆数，若未超过则按所有车辆路径相加得到总配送里程，
%%若超过总车辆数，则说明该配送路径不可行。

%%输出结果：第1列表示客户编号；第2列表示车辆的编号；
%%第3列表示车辆在该路径中，并从该行客户返回物流中心的距离；第4列表示车辆在该路径中的载货量。
clc
clear
close all;
tic;
%%
%算法参数
population_num=80;%种群规模
Max_gen=200;%迭代次数
Pc=0.9;%交叉概率
Pm=0.09;%变异概率
%模型选用参数
%车辆数量Car_num=2
%客户数量Customer_num=8
%车辆容量capacity_max=8
%行驶距离distance_max=50
Car_num=8;
Customer_num=30;
capacity_max=800;
distance_max=500;
%load Demand %客户的需求
%load Distance %客户间的距离
%load X %物流中心到客户间的距离
%%
%种群初始化
population=zeros(population_num,Customer_num);
for i=1:population_num
     population(i,:)=randperm(Customer_num);
end
%%
y=1;%循环计数器
 while y<Max_gen
     %交叉
     [new_pop_intercross]=Mating_pool(population_num,population,Pc);
     %变异
     [new_pop_mutation]=Mutation(new_pop_intercross,Pm);
     %计算目标函数
     mutation_num=size(new_pop_mutation,1);
     Total_Dis=[];
     for k=1:mutation_num
     [Result]=decode(new_pop_mutation(k,:),distance_max,capacity_max);
     [Total_Dis(k,1)]=parameter(Result,Customer_num,Car_num);
     end
     %更新种群
     new_pop_new=zeros(population_num,Customer_num);
     [Total_Dissort, index] = sort(Total_Dis);
     for k=1:population_num
         new_pop_new(k,:)=new_pop_mutation(index(k),:);
     end
     population=new_pop_new;
     %迭代次数加一
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
