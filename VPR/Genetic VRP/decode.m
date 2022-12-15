function [Result]=decode(T,distance_max,capacity_max)
%distance_max=50;
%capacity_max=8;
%%：最优解已知的情况下：根据本模型参数求得最优路径为[1 3 5 6 8 4 7 2]
%%约束条件为每条配送路径上各客户的需求量之和不超过配送车辆的载重量，同时设置最大行车距离。
%%将染色体解码为车辆的配送路径，为计算目标值做好准备。
%%根据排列顺序进行解码，主要判断该客户加入到车辆配送路径后
%%是否满足该车的最大行驶距离，是否满足该车的最大载重量。
%%比如说染色体1-2-3-4-5-6-7-8，第1辆车路径：将客户1加入到该车的路径中，则第1辆车的行驶路线为0-1-0，表示该车从物流中心0出发，将货物运到客户1，再返回物流中心，计算行驶距离是否满足第1辆车的最大行驶距离，同时计算运货量是否满足第1辆车的最大载货量，再确定客户2如果加入第1辆车的路径是否满足以上2个条件，如果满足，则第1辆车的行驶路线为0-1-2-0，依次类推，如果不满足其中的1个条件，则需要增加1辆车即为从物流中心出发的第2条行驶路线。

%%输出结果：第1列表示客户编号；第2列表示车辆的编号；
%%第3列表示车辆在该路径中，并从该行客户返回物流中心的距离；第4列表示车辆在该路径中的载货量。

%load Demand
%load Distance
%load X
xy = 10*rand(30,2);
N = size(xy,1);
a = meshgrid(1:N);
Distance = reshape(sqrt(sum((xy(a,:)-xy(a',:)).^2,2)),N,N); %城市间距离
Demand = [1 2 1 2 2 4 2 2 1 3 2 1 2 1 2 1 4 2 2 1 3 2 1 2 1 2 1 4 2 2 1 3 2];
X = [4 6 7.5 9 20 10 16 8 9 6 4 6 7.5 9 20 10 16 8 9 6 4 6 7.5 9 20 10 16 8 9 6];
WS=1;WT=1;
a1=0;b1=0;
for i=1:size(T,2)
    if WT==1
       a1=2*X(T(i));
       b1=Demand(T(i));
    elseif WT==2
        a1=X(T(i-1))+X(T(i))+Distance(T(i),T(i-1));
        b1=Demand(T(i-1))+Demand(T(i));
    else
        a1=0;b1=0;
        for j=i-WT+1:i-1
            a1=a1+Distance(T(j+1),T(j));
            b1=b1+Demand(T(j));
        end
        b1=b1+Demand(T(i));
        a1=a1+X(T(i-WT+1))+X(T(i));
    end
    if (a1>distance_max)|(b1>capacity_max)
        a1=2*X(T(i));
        b1=Demand(T(1));
        WS=WS+1;
        Result(i,:)=[T(i),WS,a1,b1];
        WT=2;
    else
        Result(i,:)=[T(i),WS,a1,b1];
        WT=WT+1;
    end
   
end

