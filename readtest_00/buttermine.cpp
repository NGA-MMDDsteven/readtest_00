
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h> 

//#define PRECISION double

//Butterworth ��ͨ�˲���ֻ��ϵ������dft�Ժ�ÿ�����������Ӧ��Ƶ���� ��k*50hz/n��k��0��n-1
//butterworth�˲���ϵ��
//a   y��ϵ����13�� //����0��ԭ��Ϊ0.0001���ĳ�1��
float  a[] = { 1.0000, - 6.9662 ,  22.2548 ,- 43.6595  , 59.3173 ,- 59.2794  , 44.7456 ,- 25.6330 ,  11.0419, - 3.4909 ,   0.7668, - 0.1042, 0.0068 };
//b x��ϵ����13��
float  b[] = { 0.0081, 0 ,- 0.0484, 0 , 0.1211, 0, - 0.1615, 0, 0.1211, 0, - 0.0484 ,0 ,0.0081 };

//��ֹ�ʽ��
// a1*yn = b1 *xn +b2*(xn-1)+...+b25*(xn-24) - a2*(yn-1)-a3*y(n-2)-...-a25*(yn-24)      ,x ��y������ȣ���a1Ҫ����1������������������

void Butterworthfilter(float *in1, int datanumline0, float *out1)  //inΪԭʼ���ݣ�outΪ�˲������ݣ�������Ϊdatanumline0
{
	//float *in = NULL; in = (float*)malloc((datanumline0+12) * 1 * sizeof(float));  //// ǰ������12���������0��ʹ����������ж�i-1������
	//float *out = NULL; out = (float*)malloc((datanumline0 + 12) * 1 * sizeof(float));
	//for (int j = 0; j < datanumline0+12; j++)
	//{
	//	if (j < 12)
	//	{
	//		in[j] = 0;
	//		out[j] = 0;
	//	}
	//	else in[j] = in1[j-12]; //��in��12���Ŀ�ʼ����������in1��0��
	//	printf("%f \t", in[j]);
//	}
	int N = 13; //ϵ������ĳ���

	//for (int i = 0; i < N; i++) printf("%f \t %f \n", a[i], b[i]);
							//����ֵ��ʼ��
	for (int i = 0; i < datanumline0; i++)
	{
		out1[i] = 0.0; //����ѭ�����õ�y�ݹ��㷨����Ҫ��ǰ��ʼ��
	}

	//��һ��ѭ��������length��y�����ֵ
	for (int i =0; i < datanumline0; i++)
	{
		out1[i] = b[0] * in1[i];
		if (i == 0)
		{
			printf("%f \t", out1[i]);
			continue;
		}
		else
		{
//			out1[i] = b[0] * in1[i];
			//�ڶ���ѭ��������ÿ��y��ÿһ��
			for (int j = 1; j <= i && j < N; j++)
			{
				out1[i] += b[j] * in1[i - j] - a[j] * out1[i - j];
			}
		}
		out1[i] /= a[0];
		printf("%f \t", out1[i]);
	}
	
	/*
	for (int i = 12; i < datanumline0+12; i++)
	{
	
		if (i==12)
		{
			in[i - 1] = 0; out[i - 1] = 0; in[i - 2] = 0; out[i - 2] = 0; in[i - 3] = 0; out[i - 3] = 0;
			in[i - 4] = 0; out[i - 4] = 0; in[i - 5] = 0; out[i - 5] = 0; in[i - 6] = 0; out[i - 6] = 0;
			in[i - 7] = 0; out[i - 7] = 0; in[i - 8] = 0; out[i - 8] = 0; in[i - 9] = 0; out[i - 9] = 0;
			in[i - 10] = 0; out[i - 10] = 0; in[i - 11] = 0; out[i - 11] = 0; in[i - 12] = 0; out[i - 12] = 0;
			
		}
		if (i == 13)
		{
			in[i - 2] = 0; out[i - 2] = 0; in[i - 3] = 0; out[i - 3] = 0;
			in[i - 4] = 0; out[i - 4] = 0; in[i - 5] = 0; out[i - 5] = 0; in[i - 6] = 0; out[i - 6] = 0;
			in[i - 7] = 0; out[i - 7] = 0; in[i - 8] = 0; out[i - 8] = 0; in[i - 9] = 0; out[i - 9] = 0;
			in[i - 10] = 0; out[i - 10] = 0; in[i - 11] = 0; out[i - 11] = 0; in[i - 12] = 0; out[i - 12] = 0;
			
		}
		if (i == 14)
		{
			in[i - 3] = 0; out[i - 3] = 0;
			in[i - 4] = 0; out[i - 4] = 0; in[i - 5] = 0; out[i - 5] = 0; in[i - 6] = 0; out[i - 6] = 0;
			in[i - 7] = 0; out[i - 7] = 0; in[i - 8] = 0; out[i - 8] = 0; in[i - 9] = 0; out[i - 9] = 0;
			in[i - 10] = 0; out[i - 10] = 0; in[i - 11] = 0; out[i - 11] = 0; in[i - 12] = 0; out[i - 12] = 0;

		}
		if (i == 15)
		{
		in[i - 4] = 0; out[i - 4] = 0; in[i - 5] = 0; out[i - 5] = 0; in[i - 6] = 0; out[i - 6] = 0;
			in[i - 7] = 0; out[i - 7] = 0; in[i - 8] = 0; out[i - 8] = 0; in[i - 9] = 0; out[i - 9] = 0;
			in[i - 10] = 0; out[i - 10] = 0; in[i - 11] = 0; out[i - 11] = 0; in[i - 12] = 0; out[i - 12] = 0;
	
		}
		if (i == 16)
		{
			 in[i - 5] = 0; out[i - 5] = 0; in[i - 6] = 0; out[i - 6] = 0;
			in[i - 7] = 0; out[i - 7] = 0; in[i - 8] = 0; out[i - 8] = 0; in[i - 9] = 0; out[i - 9] = 0;
			in[i - 10] = 0; out[i - 10] = 0; in[i - 11] = 0; out[i - 11] = 0; in[i - 12] = 0; out[i - 12] = 0;
		
		}
		if (i == 17)
		{
	 in[i - 6] = 0; out[i - 6] = 0;
			in[i - 7] = 0; out[i - 7] = 0; in[i - 8] = 0; out[i - 8] = 0; in[i - 9] = 0; out[i - 9] = 0;
			in[i - 10] = 0; out[i - 10] = 0; in[i - 11] = 0; out[i - 11] = 0; in[i - 12] = 0; out[i - 12] = 0;
	
		}
		if (i == 18)
		{
			
			in[i - 7] = 0; out[i - 7] = 0; in[i - 8] = 0; out[i - 8] = 0; in[i - 9] = 0; out[i - 9] = 0;
			in[i - 10] = 0; out[i - 10] = 0; in[i - 11] = 0; out[i - 11] = 0; in[i - 12] = 0; out[i - 12] = 0;
			
		}
		if (i == 19)
		{
			in[i - 8] = 0; out[i - 8] = 0; in[i - 9] = 0; out[i - 9] = 0;
			in[i - 10] = 0; out[i - 10] = 0; in[i - 11] = 0; out[i - 11] = 0; in[i - 12] = 0; out[i - 12] = 0;

		}
			if (i == 20)
			{
				in[i - 9] = 0; out[i - 9] = 0;
				in[i - 10] = 0; out[i - 10] = 0; in[i - 11] = 0; out[i - 11] = 0; in[i - 12] = 0; out[i - 12] = 0;

			}
			if (i == 21)
			{

				in[i - 10] = 0; out[i - 10] = 0; in[i - 11] = 0; out[i - 11] = 0; in[i - 12] = 0; out[i - 12] = 0;
				in[i - 13] = 0; out[i - 13] = 0; in[i - 14] = 0; out[i - 14] = 0; in[i - 15] = 0; out[i - 15] = 0;

			}
			if (i == 22)
			{
				in[i - 11] = 0; out[i - 11] = 0; in[i - 12] = 0; out[i - 12] = 0;

			}
			if (i == 23)
			{
				in[i - 12] = 0; out[i - 12] = 0;


			}


			out[i] = (b[0] * in[i] + b[1] * in[i - 1] + b[2] * in[i - 2] + b[3] * in[i - 3] + b[4] * in[i - 4] + b[5] * in[i - 5] + b[6] * in[i - 6] + b[7] * in[i - 7] + b[8] * in[i - 8]
				+ b[9] * in[i - 9] + b[10] * in[1 - 10] + b[11] * in[i - 11] + b[12] * in[i - 12] - a[1] * out[i - 1]
				- a[2] * out[i - 2] - a[3] * out[i - 3] - a[4] * out[i - 4] - a[5] * out[i - 5] - a[6] * out[i - 6] - a[7] * out[i - 7] - a[8] * out[i - 8] - a[9] * out[i - 9] - a[10] * out[i - 10]
				- a[11] * out[i - 11] - a[12] * out[i - 12]);// / a[0];

			printf("%f \t", out[i]);    ///
		}
		*/
//	for (int j = 0; j < datanumline0; j++)
//	{
//		out1[j] = out[j+12];
//	}
}