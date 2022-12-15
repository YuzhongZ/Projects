function [new_pop_intercross]=Mating_pool(population_num,population,Pc)
%%
%����:population,population_num,Pc
%�����1.new_popopulation_intercross
%     2.c3����Գأ��������Ⱥpopulation�������
%     3.pool
%%
pl=randperm(population_num);
num=population_num/2;
c3=zeros(2,num);
pool=[];
new_pop_intercross=population;
for kj=1:num
    c3(1,kj)=pl(2*kj-1);
    c3(2,kj)=pl(2*kj);
end%���ɡ���Գ�c3��

%%�жϡ���Գ�c3��ÿһ�Ը����������Ƿ�С�ڽ������Pc
rd=rand(1,num);
for kj=1:num
    if rd(kj)<Pc
       pool=[pool,c3(:,kj)];
    end
end
%%�ж���Գ�ÿһ�Ը����������Ƿ�С�ڽ������Pc,��С�ڣ����浽�����ӳ�pool��

pool_num=size(pool,2);
for kj=1:pool_num
    c1=population(pool(1,kj),:);
    c2=population(pool(2,kj),:);
    [new_c1,new_c2]=cross(c1,c2);
    new_pop_intercross(pool(1,kj),:)=new_c1;
    new_pop_intercross(pool(2,kj),:)=new_c2;
end
end

    