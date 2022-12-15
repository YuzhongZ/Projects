function [Total_Dis]=parameter(Result,Customer_num,Car_num)
%% ½âÂë
if Result(Customer_num,2)<=Car_num
Current_Workstation=1;
Total_Dis=0;
for i=1:Customer_num
    if Result(i,2)==Current_Workstation;
        continue
    else
        Total_Dis=Total_Dis+Result(i-1,3);
        Current_Workstation=Current_Workstation+1;
    end  
end
Total_Dis=Total_Dis+Result(Customer_num,3);
else
    Total_Dis=10000;          %³Í·£
end