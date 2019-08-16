#define pi 3.1415926
#include <iostream>  
#include <vector>  
#include<math.h>
using namespace std;
float *LPSD(float *num,float *tnn, float *nn, int count_fft, int count_rr )
{
	float *PSD = new float[count_fft];
	float *twt = new float[count_rr];
	float *wtmt = new float[count_rr];
	float tau;

	for (int i = 0; i < count_fft; i++)
	{
		PSD[i] = 0;
	}
	for (int i = 0; i < count_fft; i++)
	{
		float w = 2 * pi *num[i];
		if (w > 0)
		{
			for (int j = 0; j < count_rr; j++)
			{
				twt[j] = 2 * pi*tnn[j];
			}
			float sum1 = 0;
			float sum2 = 0;
			float sum3 = 0;
			float sum4 = 0;
			float sum5 = 0;
			float sum6 = 0;
			for (int j = 0; j < count_rr; j++)
			{
				sum1 += sin(twt[j]);
				sum2 += cos(twt[j]);
			}
			tau = atan2(sum1, sum2)/(2*w);
			for (int j = 0; j < count_rr; j++)
			{
				wtmt[j] = w * (tnn[j] - tau);
			}
			for (int j = 0; j < count_rr; j++)
			{
				sum3 += (nn[j] * sin(wtmt[j]));
				sum4 += (nn[j] * cos(wtmt[j]));
				sum5 += sin(wtmt[j]);
				sum6 += cos(wtmt[j]);
			}
			PSD[i] = (sum3*sum3) / (sum5*sum5) + (sum4*sum4) / (sum6*sum6);
		}
		else
		{
			float sum7 = 0;
			float sum8 = 0;
			for (int j = 0; j < count_rr; j++)
			{
				sum7 += (nn[j] * tnn[j]);
				sum8 += (tnn[j] * tnn[j]);
			}
			PSD[i] = sum7 * sum7 / sum8;
		}
	}

	delete(twt);
	delete(wtmt);

	return PSD;
}