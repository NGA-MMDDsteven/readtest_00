#include <iostream>  
#include <vector>  
using namespace std;

//  
//C++ʵ����һά��ɢ���ݼ�ֵ�����塢���ȣ�  
//  
//���������飬�����С  

vector<int> findPeaks(float *num, int count, float *position, int numofposition[1])
{
	vector<float> sign;
	for (int i = 1; i < count; i++)
	{
		//����ֵ���С��0����-1;����0����1;����0����0

		float diff = num[i] - num[i - 1];   //�����������м�Ĳ�ֵ���ж�����֧
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
	//�ٶ�sign����λ����  
	//���漫��ֵ�ͼ�Сֵ��λ��  
	vector<int> indMax;
	vector<float> valueMax;
	for (int j = 1; j < sign.size(); j++)
	{
		float diff = sign[j] - sign[j - 1];    //����0��С��0��sign�м��Ƿ�ֵ
		if (diff <0)
		{
			indMax.push_back(j);     //�����������ֻ��һ����ֵ��λ�ã�����ʵ����ߵ�J��
			valueMax.push_back(num[indMax.back()]);  //��ֵ��ʵ��ֵ

		}
	
	}
	////����һ���еķ�ֵ�У�30��30���ıȽϣ�ֻҪ�м��Ǹ���ߵ�   ���ǵ�9000����3min���Ҳ��100*3��������ƽ��30��һ��;��Ϊ90��������Ӧ��33
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
			float max2 = -1000000;   ////��ΪС�����и�ֵ  �����Բ���0                  ////ÿ�ҵ�һ�����ֵ���ʹ��������Ǹ������¿�ʼ��
			for (int jj = 0 + j; jj < 30 + j; jj++)
			{
				if (valueMax[jj] > max2)
				{
					max2 = valueMax[jj]; max1 = jj;
				}
			}

		//	j = j + max1;
			if (lastposi != max1 &&lastvalue != max2 && (indMax[max1] - indMax[lastposi])>30 && max2>0 )   ////���������ظ������һ�Ҫ���ټ��5����
			{
				indjMax.push_back(indMax[max1]);
				valuejMax.push_back(max2);
			}
				lastposi = max1;
				lastvalue = max2;  ////�´�ѭ��������ظ��ˣ����ᱻ��������

			}

		*/

		printf("Total: %d \t   ���弫��ֵΪ��\n", indjMax.size());
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
		//����ֵ���С��0����-1;����0����1;����0����0
		
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
	//�ٶ�sign����λ����  
	//���漫��ֵ�ͼ�Сֵ��λ��  
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
	
	printf("Total: %d \t   ���弫��ֵΪ��\n", indMax.size());
	numofposition[0] = indMax.size();
	for (int m = 0; m < indMax.size(); m++)
	{
		printf("position: %d \t  energy: %f \n",indMax[m], num[indMax[m]]);
		position[m] = indMax[m];
	}
	
	return indMax;
}
*/