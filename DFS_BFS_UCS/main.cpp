#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include "time.h"

using namespace std;

vector<int> final_state = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };

// 求逆序数判断是否有解
int getInversions(vector<int> num)
{   
	int count = 0;
	for (int i = 0; i < 9; i++)
	{
		if (num[i] == 0)
		{
			continue;
		}
		for (int j = i; j < 9; j++)
		{
			if (num[j] != 0 && num[i] > num[j])
			{
				count++;
			}
		}
	}
	return count;
} 

//找0的位置
int Location0(vector<int> temple_state) {
	for (int i = 0; i < temple_state.size(); i++) {
		if (temple_state[i] == 0)
			return i;
	}
} 

//表示每个状态信息
class status
{
public:
	vector<int> num;
	int nowId, parentId,depth;      // 自身序号、父结点序号，深度
	status(vector<int> num, int nowId, int parentId,int depth)
	{
		this->num = num;
		this->nowId = nowId;
		this->parentId = parentId;
		this->depth = depth;
	}
}; 

//重载优先队列顺序
struct cmp
{
	bool operator()(const status& a, const status& b)
	{
		return a.depth > b.depth;
	}
}; 

// the puzzle class
class AI_Puzzle
{
public:
	AI_Puzzle() { ; };
	AI_Puzzle(const vector<int>& init_state) {
		m_init_state.assign(init_state.begin(), init_state.end());
	};
	~AI_Puzzle() { ; };

public:
	// set the init state
	void setInitState(const vector<int>& init_state) {
		m_init_state.assign(init_state.begin(), init_state.end());
	};

	//
	// please fill three types of solver to find the solution
	// 
	// return true if the problem has solution
	// return false if the problem does not have solution
	// results: the output sequences of states
	// 
	// We want the final state is
	//
	//		1	2	3
	//		4	5	6
	//		7	8	0
	//
	bool solution(vector<vector<int>>& results)
	{
		// TODO: please fill in the code
		//
		// Yangang Wang, 2021/8/15, @SEU

		int inverStart, inverEnd;
		inverStart = getInversions(m_init_state);
		inverEnd = getInversions(final_state);
		if (inverStart % 2 == inverEnd % 2)
		{
			return BFS(results);
		     //return DFS(results);
		    //return UCS(results);
		}
		else
			return false;
	}



private:
	// bfs to find the solution
	bool BFS(vector<vector<int>>& results)
	{
		// TODO: please fill in the code
		//
		// Yangang Wang, 2021/8/15, @SEU

		int total_step = 0;
		int n = 0;
		queue <status> que;
		vector<status> store;
		vector<int> move = { -1, -3, 1, 3 };
		vector<int> final_state = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };
		status init_status(m_init_state, 0, 0, 0);//初始状态
		que.push(init_status);
		//store.push_back(init_status);

		while (!que.empty())
		{
			status now = que.front();//用now表示队列头元素
			que.pop();              //弹出队头
			store.push_back(now);
			results.push_back(now.num);  //results中添加访问过的状态相当于步骤中的一步
			if (now.num == final_state)
				return true;           
			int zero = Location0(now.num);
			for (int i = 0; i < 4; i++)
			{
				vector<int> now_num = now.num;//移动后的新排列
				
				if ((zero + move[i]) >= 0 && (zero + move[i]) <= 8 && !(zero == 2 && (zero + move[i]) == 3)&& !(zero == 3 && (zero + move[i]) == 2)&& !(zero == 5 && (zero + move[i]) == 6)&& !(zero == 6 && (zero + move[i]) == 5))
				{
					int temp = now_num[zero];
					now_num[zero] = now_num[zero + move[i]];
					now_num[zero + move[i]] = temp;

					if (now_num == final_state)       //判断新状态是否为目标状态
					{
						results.push_back(now_num);
						n++;
						status new_now(now_num, n, now.nowId,0);
						store.push_back(new_now);

						vector<vector<int>> temp_results;
						int y = results.size();
						temp_results.push_back(results[y - 1]);
						int q = store[y-1].parentId;
						while(q)
						{
							   temp_results.push_back(results[q]);
							   q = store[q].parentId;
						}
						temp_results.push_back(results[q]);
						results.clear();
						for (int i = temp_results.size()-1; i>=0; i--)
						{
							results.push_back(temp_results[i]);
						}
						results.push_back({ total_step });
						return true;
					}

					int flag = 0;
					for (int i = 0; i < results.size(); i++) //判断新状态是否已经得到过
					{
						if (now_num == results[i])
							flag = 1;
					}
					if (!flag)
					{
						n = n + 1;
						status new_now(now_num, n, now.nowId,0);  //移动后的新排列构成的新状态
						que.push(new_now);
						total_step++;
					}
				}
			}
		}
		return false;
	}

	// dfs to find the solution
	bool DFS(vector<vector<int>>& results)
	{
		// TODO: please fill in the code
		//
		// Yangang Wang, 2021/8/15, @SEU

		stack<status> open;
		vector<int> move = { -1, -3, 1, 3 };

		vector<int> final_state = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };
		status init_status(m_init_state, 0, 0,0); //初始状态
		open.push(init_status);
		results.push_back(m_init_state);
		int total_step = 0;
		int depth = 0;

		while (!open.empty())
		{
			status now = open.top();    //用now表示队列头元素
			int zero = Location0(now.num);
			int p = 0;
			int n = now.nowId;
			
			while (n >= 0 && n < 4)
			{
				vector<int> now_num = now.num;//移动后的新排列
				if ((zero + move[n]) >= 0 && (zero + move[n]) <= 8 && !(zero == 2 && (zero + move[n]) == 3) && !(zero == 3 && (zero + move[n]) == 2) && !(zero == 5 && (zero + move[n]) == 6) && !(zero == 6 && (zero + move[n]) == 5))
				{
					int temp = now_num[zero];
					now_num[zero] = now_num[zero + move[n]];
					now_num[zero + move[n]] = temp;
					n++;
					//total_step++;
					if (now_num == final_state)       //判断新状态是否为目标状态，是就添加到results里并返回结束
					{
						results.push_back(now_num);
						depth++;
						status new_now(now_num, 0, 0, depth);
						open.push(new_now);
						total_step++;
						vector<vector<int>> temp_results;
						while (!open.empty())
						{
							new_now = open.top();
							temp_results.push_back(new_now.num);
							open.pop();
						}
						results.clear();
						for (int i = temp_results.size() - 1; i >= 0; i--)
						{
							results.push_back(temp_results[i]);

						}
						results.push_back({total_step});
						return true;
					}
					int flag = 0;
					for (int i = 0; i < results.size(); i++) //判断新状态是否已经得到过
					{
						if (now_num == results[i])
							flag = 1;
					}
					if (!flag)
					{
						p = 1;
						depth++;
						total_step++;
						status new_now(now_num, 0, 0, depth);  //移动后的新排列构成的新状态
						open.push(new_now);
						results.push_back(now_num);
						break;
					}
				}
				else
				{
					n++;
				}
			}
			now.nowId = n;
			if ((now.nowId == 4 && p == 0)||(depth>10))
			{
				open.pop();
				depth--;
				//results.pop_back();
			}
		}
		return false;
	}

	// ucs to find the solution
	bool UCS(vector<vector<int>>& results)
	{
		// TODO: please fill in the code
		//
		// Yangang Wang, 2021/8/15, @SEU

		int total_step = 0;
		int time = 0;
		int n = 0;
		int depth = 0;

		priority_queue<status, vector<status>, cmp> que; //优先队列
		vector<status> store;
		vector<int> move = { -1, -3, 1, 3 };
		vector<int> final_state = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };
		status init_status(m_init_state, 0, 0,0);//初始状态
		que.push(init_status);


		while (!que.empty())
		{
			status now = que.top();//用now表示队列头元素
			que.pop();              
			store.push_back(now);
			results.push_back(now.num);  //results中添加访问过的状态相当于步骤中的一步
			depth = now.depth;
			if (now.num == final_state)//判断新状态是否为目标状态
			{
				//results.push_back(now.num);
				//n++;
				//status new_now(now.num, n, now.nowId);
				//store.push_back(new_now);

				vector<vector<int>> temp_results;
				int y = results.size();
				temp_results.push_back(results[y - 1]);
				int q = store[y - 1].parentId;
				while (q)
				{
					temp_results.push_back(results[q]);
					q = store[q].parentId;
				}
				temp_results.push_back(results[q]);
				results.clear();
				for (int i = temp_results.size() - 1; i >= 0; i--)
				{
					results.push_back(temp_results[i]);
				}
				results.push_back({ total_step });
				return true;
			}
			total_step++;
			int zero = Location0(now.num);
			depth++;
			for (int i = 0; i < 4; i++)
			{
				vector<int> now_num = now.num;//移动后的新排列

				if ((zero + move[i]) >= 0 && (zero + move[i]) <= 8 && !(zero == 2 && (zero + move[i]) == 3) && !(zero == 3 && (zero + move[i]) == 2) && !(zero == 5 && (zero + move[i]) == 6) && !(zero == 6 && (zero + move[i]) == 5))
				{
					int temp = now_num[zero];
					now_num[zero] = now_num[zero + move[i]];
					now_num[zero + move[i]] = temp;
					time++;
					int flag = 0;
					for (int i = 0; i < results.size(); i++) //判断新状态是否已经得到过
					{
						if (now_num == results[i])
							flag = 1;
					}
					if (!flag)
					{
						n = n + 1;
						now.depth++;
						//total_step++;
						status new_now(now_num, n, now.nowId, depth);  //移动后的新排列构成的新状态
						que.push(new_now);
						//results.push_back(now_num);
					}
				}
			}
		}
		return false;
	}

private:
	vector<int> m_init_state;
};

// test function for 8-puzzle problem
void test(const vector<int>& init_state) {
	AI_Puzzle puzzle;
	puzzle.setInitState(init_state);
	vector<vector<int>> results;
	int i, j;
	if (puzzle.solution(results)) {
		printf("solution for the initial state: ");
		for (i = 0; i < init_state.size(); i++)
			printf("%d ", init_state[i]);
		printf("\n");
		for (i = 0; i < results.size()-1; i++) {
			printf("step: %03d:", i);
			for (j = 0; j < results[i].size(); j++) {
				printf(" %d", results[i][j]);
			}
			printf("\n");
			
		}
		printf("\n");
		printf("total step:%d", results[i][0]);
	}
	else {
		printf("no solution for the initial state: ");
		for (int i = 0; i < init_state.size(); i++)
			printf("%d ", init_state[i]);
		printf("\n");
	}
	printf("\n");
}

int main(int argc, char** argv)
{
	// This is the code for testing the 8-puzzle problem in
	//
	//  'The first project homework for Artifical Intelligence'
	// 
	// Please fill in the code of class 'AI_Puzzle'
	// and write your design, running tests, analysis and etc. into your report
	// Be careful: in each step, we can ONLY exchange with ONE neighbor of digit 0, horizontally or vertically
	//
	// Good Luck!
	//
	// Yangang Wang, 2021/8/15, @SEU

	// Test 1
	//	5   1   2
	//	6   3   0
	//	4   7   8

	clock_t start, finish;
	double duration;
	start = clock();
	 vector<int> init_state1 = { 5, 1, 2, 6, 3, 0, 4, 7, 8 };
     test(init_state1);

	// Test 2
	//
	//	2   8   3
	//	1   6   4
	//	7   5   0
	vector<int> init_state_2 = { 2, 8, 3, 1, 6, 4, 7, 5, 0 };
   // test(init_state_2);

	// Test 3
	//
	//	1   0   2
	//	3   4   5
	//	6   7   8
	vector<int> init_state_3 = { 1, 0, 2, 3, 4, 5, 6, 7, 8 };
	//test(init_state_3);

	// Test 4
	//
	//	4   3   5
	//	6   0   8
	//	2   1   7
	vector<int> init_state_4 = { 4, 3, 5, 6, 0, 8, 2, 1, 7 };
	//test(init_state_4);

	// Test 5
	//
	//	1   7   3
	//	0   5   4
	//	6   2   8
	vector<int> init_state_5 = { 1, 7, 3, 0, 5, 4, 6, 2, 8 };
	//test(init_state_5);

	//long end_time = GetTickCount64();     //获取结束时间
	//long Times = end_time - start_time;
	//printf("seconds: %03d",Times);

	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("%f seconds\n", duration);
	return 0;
}