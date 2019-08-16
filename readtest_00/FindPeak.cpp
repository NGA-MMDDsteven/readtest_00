#include <iostream>  
#include <vector>  
using namespace std;

//  
//C++实现求一维离散数据极值（波峰、波谷）  
//  
//参数：数组，数组大小  

vector<int> findPeaks(float *num, int count, float *position, int numofposition[1])
{
	vector<float> sign;
	for (int i = 1; i < count; i++)
	{
		//相邻值做差：小于0，赋-1;大于0，赋1;等于0，赋0

		float diff = num[i] - num[i - 1];   //计算两个峰中间的差值，判断升降支
		if (diff > 0)
		{
			sign.push_back(diff);
		}
		else if (diff < 0)
		{
			sign.push_back(diff);
		}
		else
		{
			sign.push_back(diff);
		}
	}
	//再对sign相邻位做差  
	//保存极大值和极小值的位置  
	vector<int> indMax;
	vector<float> valueMax;
	for (int j = 1; j < sign.size(); j++)
	{
		float diff = sign[j] - sign[j - 1];    //大于0和小于0的sign中间是峰值
		if (diff <0)
		{
			indMax.push_back(j);     //这个存下来的只是一个峰值的位置，不是实际最高的J峰
			valueMax.push_back(num[indMax.back()]);  //峰值的实际值

		}
	
	}
	////在上一步中的峰值中，30个30个的比较，只要中间那个最高的   考虑到9000点中3min最多也就100*3次心跳，平均30点一次;设为90次心跳对应的33
	vector<int> indjMax;
	vector<float> valuejMax;
	int lastposi=0;
	float lastvalue=0;
	if (indMax.size() >= 2)
	{
		for (int j = 2; j < indMax.size() - 2; j++)
		{
			if (valueMax[j] > valueMax[j - 1] && valueMax[j] > valueMax[j - 2] && valueMax[j] > valueMax[j + 1] && valueMax[j] > valueMax[j + 2] && indMax[j] - lastposi > 33)
			{
				indjMax.push_back(indMax[j]);
				valuejMax.push_back(valueMax[j]);
				lastposi = indMax[j];
			}
		}
		/*
		for (int j = 0; j < indMax.size() - 30;j++)//; j++)
		{
			int max1 = 0;
			float max2 = -1000000;   ////因为小波后有负值  ，所以不用0                  ////每找到一个最大值，就从他后面那个峰重新开始找
			for (int jj = 0 + j; jj < 30 + j; jj++)
			{
				if (valueMax[jj] > max2)
				{
					max2 = valueMax[jj]; max1 = jj;
				}
			}

		//	j = j + max1;
			if (lastposi != max1 &&lastvalue != max2 && (indMax[max1] - indMax[lastposi])>30 && max2>0 )   ////不仅不能重复，而且还要至少间隔5个峰
			{
				indjMax.push_back(indMax[max1]);
				valuejMax.push_back(max2);
			}
				lastposi = max1;
				lastvalue = max2;  ////下次循环后，如果重复了，不会被记入总数

			}

		*/

		printf("Total: %d \t   具体极大值为：\n", indjMax.size());
		numofposition[0] = indjMax.size();
		for (int m = 0; m < indjMax.size(); m++)
		{
			printf("position: %d \t  energy: %f \n", indjMax[m], num[indjMax[m]]);
			position[m] = indjMax[m];
		}

		return indjMax;
	}
	else
	{
		return indjMax;
	}
	
}
/*
vector<int> findPeaks(float *num, int count,float *position,int numofposition[1])
{
	vector<float> sign;
	for (int i = 1; i < count; i++)
	{
		//相邻值做差：小于0，赋-1;大于0，赋1;等于0，赋0
		
		float diff = num[i] - num[i - 1];
		if (diff > 0)
		{
			sign.push_back(1);
		}
		else if (diff < 0)
		{
			sign.push_back(-1);
		}
		else
		{
			sign.push_back(0);
		}
	}
	//再对sign相邻位做差  
	//保存极大值和极小值的位置  
	vector<int> indMax;
	vector<float> valueMax;
	for (int j = 1; j < sign.size(); j++)
	{
		int diff = sign[j] - sign[j - 1];
		if (diff == -2)
		{
			indMax.push_back(j);
			valueMax.push_back(num[indMax.back()]);

		}
		else if (diff == -1)
		{
			if (sign[j] == -1)
			{
				indMax.push_back(j);
				valueMax.push_back(num[indMax.back()]);
			}

		}
	}
	
	printf("Total: %d \t   具体极大值为：\n", indMax.size());
	numofposition[0] = indMax.size();
	for (int m = 0; m < indMax.size(); m++)
	{
		printf("position: %d \t  energy: %f \n",indMax[m], num[indMax[m]]);
		position[m] = indMax[m];
	}
	
	return indMax;
}
*/