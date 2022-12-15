// This is the code for the BNs inference
//
//  'The second project homework for Artifical Intelligence'
// 
// Yangang Wang, 2021/11/9, @SEU
//

#include <fstream>
#include <string>
#include <iostream>
#include <regex>

using namespace std;
void main(int argc, char** argv)
{
	// read the mat
	// the missing element is indicated by -1
	//
	int Mat_missing[100][100];
	{
		ifstream infile("mat_missing.txt");
		if (infile.is_open()) {
			string line;
			regex term(" ");
			int lineIdx = 0;
			while (!infile.eof()) {
				if (lineIdx >= 100) break;
				getline(infile, line);
				sregex_token_iterator it(line.begin(), line.end(), term, -1);
				sregex_token_iterator end;
				for (int i = 0; it != end; it++, i++) {
					string str = *it;
					Mat_missing[lineIdx][i] = atoi(str.c_str());
				}
				lineIdx++;
			}
			infile.close();
		}
	}

	//======================
    // You can compare the result with groudtruth data
    // read the groudtruth data
	int Mat_gd[100][100];
	{
		ifstream infile("mat_groudtruth.txt");
		if (infile.is_open()) {
			string line;
			regex term(" ");
			int lineIdx = 0;
			while (!infile.eof()) {
				if (lineIdx >= 100) break;
				getline(infile, line);
				sregex_token_iterator it(line.begin(), line.end(), term, -1);
				sregex_token_iterator end;
				for (int i = 0; it != end; it++, i++) {
					string str = *it;
					Mat_gd[lineIdx][i] = atoi(str.c_str());
				}
				lineIdx++;
			}
			infile.close();
		}
	}

	//======================
	// please fill the following code by Bayes Network training and inference
	//
	// Yangang Wang, 2021/11/9, @SEU
	//

	// (1) todo...
	double n[11] = { 0 };
	int nm[11] = { 0 };
	double py[11] = { 0 };
	ouble ya[11][9] = { 0 }, ay0[11][9] = { 0 }, ay1[11][9] = { 0 }, ay2[11][9] = { 0 },ay3[11][9] = { 0 },ay4[11][9] = { 0 }, ay5[11][9] = { 0 }, ay6[11][9] = { 0 }, ay7[11][9] = { 0 }, ay8[11][9] = { 0 }, ay9[11][9] = { 0 }, ay10[11][9] = { 0 }, xy_all[11] = {1,1,1,1,1,1,1,1,1,1,1};
	//double ya[11][5] = { 0 }, ay0[11][5] = { 0 }, ay1[11][5] = { 0 }, ay2[11][5] = { 0 }, ay3[11][5] = { 0 }, ay4[11][5] = { 0 }, ay5[11][5] = { 0 }, ay6[11][5] = { 0 }, ay7[11][5] = { 0 }, ay8[11][5] = { 0 }, ay9[11][5] = { 0 }, ay10[11][5] = { 0 };
	int y_label[] = { 0,1,2,3,4,5,6,7,8,9,10 };
	int record[99][2] = { 0 };
	int num = 10000,r=0;
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			int m = 0;
			/*int m = 0, l=i,k=j;
			for (int x = 0; x < 4; x++)
			{
				switch(x)
				{
				case 0: {l = i - 1;  k = j; }break;
				case 1: {l = i;  k = j-1; }break;
				case 2: { l = i;  k = j+1; }break;
				case 3: { l = i + 1;  k = j; }break;
					
				}
				if (l >= 0 && l < 100 && k >= 0 && k < 100)
				{
					if (Mat_missing[l][k] == Mat_missing[i][j])
						m++;
					switch (Mat_missing[l][k])
					{
					case 0: nm[0]++; break;
					case 1: nm[1]++;  break;
					case 2: nm[2]++;   break;
					case 3: nm[3]++;   break;
					case 4: nm[4]++;  break;
					case 5: nm[5]++;  break;
					case 6: nm[6]++;  break;
					case 7: nm[7]++; break;
					case 8: nm[8]++;  break;
					case 9: nm[9]++;  break;
					case 10: nm[10]++; break;
					}
				}
			}*/
			//double a = 0;
			for (int l = -1; l < 2; l++)//向8个方向遍历
			{
				for (int k = -1; k < 2; k++)
				{
					if ((i + l) >= 0 && (i + l) < 100 && (j + k) >= 0 && (j + k) < 100)//判断是否在100*100边界内
					{
						if (Mat_missing[i + l][j + k] == Mat_missing[i][j])
							m++;
						switch (Mat_missing[i + l][j + k])    //记录周围数据情况
						{
						case 0: nm[0]++; break;
						case 1: nm[1]++;  break;
						case 2: nm[2]++;   break;
						case 3: nm[3]++;   break;
						case 4: nm[4]++;  break;
						case 5: nm[5]++;  break;
						case 6: nm[6]++;  break;
						case 7: nm[7]++; break;
						case 8: nm[8]++;  break;
						case 9: nm[9]++;  break;
						case 10: nm[10]++; break;
						}
						if (l == 0 && k == 0)         //将前面把自身算进去的减掉
							nm[Mat_missing[i + l][j + k]]--;
					}
				}
			}
	        m = m - 1;
			switch (Mat_missing[i][j])           //判断样本数据类别
			{
			case 0: {
				n[0]++; ya[0][m]++;             //累计该类别个数
				for (int u = 0; u < 11; u++)    //根据上述周围数据情况存入数组ay中，不断累加，记录整个训练集中样本周围有相同情况的频数
				{
					int q = nm[u];
					if (nm[u] > 0)
						ay0[u][q]++;            //训练集中周围有q个u的标签为0的样本个数+1
					nm[u] = 0;                 //将数组nm清0以便下一个循环
				}
			} break;
			case 1: {
				n[1]++; ya[1][m]++;
				for (int u = 0; u < 11; u++)
				{
					int q = nm[u];
					if (nm[u] > 0)
						ay1[u][q]++;
					nm[u] = 0;
				}
			} break;
			case 2: {
				n[2]++; ya[2][m]++;
				for (int u = 0; u < 11; u++)
				{
					int q = nm[u];
					if (nm[u] > 0)
						ay2[u][q]++;
					nm[u] = 0;
				}
			} break;
			case 3: {
				n[3]++; ya[3][m]++;
				for (int u = 0; u < 11; u++)
				{
					int q = nm[u];
					if (nm[u] > 0)
						ay3[u][q]++;
					nm[u] = 0;
				}
			} break;
			case 4: {
				n[4]++; ya[4][m]++;
				for (int u = 0; u < 11; u++)
				{
					int q = nm[u];
					if (nm[u] > 0)
						ay4[u][q]++;
					nm[u] = 0;
				}
			}break;
			case 5: {
				n[5]++; ya[5][m]++;
				for (int u = 0; u < 11; u++)
				{
					int q = nm[u];
					if (nm[u] > 0)
						ay5[u][q]++;
					nm[u] = 0;
				}
			}break;
			case 6: {
				n[6]++; ya[6][m]++;
				for (int u = 0; u < 11; u++)
				{
					int q = nm[u];
					if (nm[u] > 0)
						ay6[u][q]++;
					nm[u] = 0;
				}
			}break;
			case 7: {
				n[7]++; ya[7][m]++;
				for (int u = 0; u < 11; u++)
				{
					int q = nm[u];
					if (nm[u] > 0)
						ay7[u][q]++;
					nm[u] = 0;
				}
			}break;
			case 8: {
				n[8]++; ya[8][m]++;
				for (int u = 0; u < 11; u++)
				{
					int q = nm[u];
					if (nm[u] > 0)
						ay8[u][q]++;
					nm[u] = 0;
				}
			}break;
			case 9: {
				n[9]++; ya[9][m]++;
				for (int u = 0; u < 11; u++)
				{
					int q = nm[u];
					if (nm[u] > 0)
						ay9[u][q]++;
					nm[u] = 0;
				}
			}break;
			case 10: {
				n[10]++; ya[10][m]++;
				for (int u = 0; u < 11; u++)
				{
					int q = nm[u];
					if (nm[u] > 0)
						ay10[u][q]++;
					nm[u] = 0;
				}
			}break;
			case -1: {
				num--;                  //记录训练集样本个数
				record[r][0] = { i };   //记录待分类样本位置
				record[r][1] = { j }; 
				r++;
			} break;
			}

		}
	}
	cout << endl << "************先验概率***********" << endl;
	for (int i = 0; i < 11; i++)
	{
		py[i] = double((n[i]+1) / (num+ n[i]));
		cout << "p(y= " << i << ") = " << py[i] << endl;
	}

	double sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0, sum6 = 0, sum7 = 0, sum8 = 0, sum9 = 0, sum10 = 0;
	cout << endl << "************条件概率***********" << endl;
	cout << "p(a = i |y = 8 ) = " << endl;
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j <9; j++)
		{
			ya[i][j] = double(ya[i][j] / n[i]);
			sum0= sum0 + ay0[i][j];
			sum1 = sum1 + ay1[i][j];
			sum2 = sum2 + ay2[i][j];
			sum3 = sum3 + ay3[i][j];
			sum4 = sum4 + ay4[i][j];
			sum5 = sum5 + ay5[i][j];
			sum6 = sum6 + ay6[i][j];
			sum7 = sum7 + ay7[i][j];
			sum8 = sum8 + ay8[i][j];
			sum9 = sum9 + ay9[i][j];
			sum10 = sum10 + ay10[i][j];
			
		}
	}
    for (int i = 0; i < 11; i++)
    {
		for (int j = 0; j < 9; j++)//计算条件概率，用Laplace校准
		{
			ay0[i][j] = (ay0[i][j] + 1) / (sum0 + ay0[i][j] * 1);
			ay1[i][j] = (ay1[i][j]+1) / (sum1 + ay1[i][j]*1);
			ay2[i][j] = (ay2[i][j]+ 1) / (sum2 + ay2[i][j]*1);
			ay3[i][j] = (ay3[i][j]+ 1) / (sum3 + ay3[i][j]*1);
			ay4[i][j] = (ay4[i][j]+ 1) / (sum4 + ay4[i][j]*1);
			ay5[i][j] = (ay5[i][j]+ 1) / (sum5 + ay5[i][j]*1);
			ay6[i][j] = (ay6[i][j] + 1) / (sum6 + ay6[i][j]*1);
			ay7[i][j] = (ay7[i][j] + 1) / (sum7 + ay7[i][j]*1);
			ay8[i][j] = (ay8[i][j] + 1) / (sum8 + ay8[i][j]*1);
			ay9[i][j] = (ay9[i][j] + 1) / (sum9 + ay9[i][j]*1);
			ay10[i][j] = (ay10[i][j] + 1) / (sum10 + ay10[i][j]*1);
			/*ay0[i][j] = ay0[i][j]/ n[0] ;
			ay1[i][j] = ay1[i][j] / n[1];
			ay2[i][j] = ay2[i][j] / n[2];
			ay3[i][j] = ay3[i][j] / n[3];
			ay4[i][j] = ay4[i][j] / n[4];
			ay5[i][j] = ay5[i][j] / n[5];
			ay6[i][j] = ay6[i][j] / n[6];
			ay7[i][j] = ay7[i][j] / n[7];
			ay8[i][j] = ay8[i][j] / n[8];
			ay9[i][j] = ay9[i][j] / n[9];
			ay10[i][j] = ay10[i][j] / n[10];*/
			cout << ay8[i][j] << "  ";
			
		}
		cout << endl;
	}

	cout << endl << "************测试***********" << endl;
	int col = sizeof(record) / sizeof(record[0]);
	double xy[11];
	int nn[11] = { 0 };
	int maxy[99] = { 0 };
	for (int r = 0; r < col; r++)      
	{
		int i = record[r][0];       //从记录的未分类样本位置获取i,j
		int j = record[r][1];
		//double a = 0;
		/*for (int x = 0; x < 4; x++)
		{
			int l = i, k = j;
			switch (x)
			{
			case 0: {l = i - 1;  k = j; }break;
			case 1: {l = i;  k = j - 1; }break;
			case 2: { l = i;  k = j + 1; }break;
			case 3: { l = i + 1;  k = j; }break;

			}
			if (l >= 0 && l < 100 && k >= 0 && k < 100)
			{
				switch (Mat_missing[l][k])
				{
				case 0: {nn[0]++; } break;
				case 1: {nn[1]++; } break;
				case 2: {nn[2]++; } break;
				case 3: {nn[3]++; } break;
				case 4: {nn[4]++; } break;
				case 5: {nn[5]++; }break;
				case 6: {nn[6]++; }break;
				case 7: {nn[7]++; }break;
				case 8: {nn[8]++; }break;
				case 9: {nn[9]++; }break;
				case 10: {nn[10]++; }break;
					//case -1:num--; break;
				}
			}
		}*/
		for(int l = -1; l < 2; l++)       //遍历8个方向获取该样本周围数据情况即数据特征
			for (int k = -1; k < 2; k++)
			{
				
				if ((i + l) >= 0 && (i + l) < 100 && (j + k) >= 0 && (j + k) < 100)
					switch (Mat_missing[i + l][j + k])
					{
					case 0: {nn[0]++; } break;
					case 1: {nn[1]++; } break;
					case 2: {nn[2]++; } break;
					case 3: {nn[3]++; } break;
					case 4: {nn[4]++; } break;
					case 5: {nn[5]++; }break;
					case 6: {nn[6]++; }break;
					case 7: {nn[7]++; }break;
					case 8: {nn[8]++; }break;
					case 9: {nn[9]++; }break;
					case 10: {nn[10]++;}break;
					//case -1:num--; break;
					}
			}
		for (int w = 0; w < 11; w++)
			xy[w] = py[w];
		for (int i = 0; i < 11; i++)        //计算公式分子
		{
			if (nn[i] > 0)
			{
				xy[0] = xy[0] * ay0[i][nn[i]];
				xy[1] = xy[1] * ay1[i][nn[i]];
				xy[2] = xy[2] * ay2[i][nn[i]];
				xy[3] = xy[3] * ay3[i][nn[i]];
				xy[4] = xy[4] * ay4[i][nn[i]];
				xy[5] = xy[5] * ay5[i][nn[i]];
				xy[6] = xy[6] * ay6[i][nn[i]];
				xy[7] = xy[7] * ay7[i][nn[i]];
				xy[8] = xy[8] * ay8[i][nn[i]];
				xy[9] = xy[9] * ay9[i][nn[i]];
				xy[10] = xy[10] * ay10[i][nn[i]];
			}
			nn[i] = 0;
		}
		double px = 0, maxp[99] = { 0 }, pyx[11] = { 0 };
		
		for (int w = 0; w < 11; w++)   //计算公式分母
			px = px+xy[w];
		for (int w = 0; w < 11; w++)   //计算后验概率
		{ 
			pyx[w] = xy[w]/px;
			cout << "p(y = " << w << "|x) = " << pyx[w] << endl;
			if (pyx[w] > maxp[r])      //判断最大值
			{
				maxp[r] = pyx[w];     //最大概率
				maxy[r] = w;          //所属类别
			}
		}
		cout <<"("<< record[r][0] << ","<< record[r][1]<<")处最大后验概率为" << maxp[r] << "，得到类别y为" << maxy[r] << "，真值矩阵中为" << Mat_gd[record[r][0]][record[r][1]] << endl;
	}
		
			
	// fill the comparing code here
	//
	// Yangang Wang, 2021/11/9, @SEU
	//

	
	// (2) todo...
	double correct = 0;
	for (int w = 0; w < 99; w++)
	{
		if (maxy[w] == Mat_gd[record[w][0]][record[w][1]])//比较
			correct++;
	}
	double co = correct / 99;        //计算
	cout << "测试准确率为"<<co<<endl;
}