#include <iostream>  
#include <vector>  
using namespace std;
float *Smoothdata(float *num, int count)
{
	float *Smooth = new float[count];
	for (int i = 4; i < count-4; i++)
	{
		Smooth[i] = (num[i] + num[i + 1] + num[i + 2] + num[i + 3] + num[i + 4] + num[i - 1] + num[i - 2] + num[i - 3] + num[i - 4] )/ 9;
	}
	Smooth[0] = num[0];
	Smooth[1] = (num[0]+num[1]+ num[2] )/3;
	Smooth[2] = (num[0] + num[1] + num[2] + num[3] + num[4])/5;
	Smooth[3] = (num[0] + num[1] + num[2] + num[3] + num[4] + num[5] + num[6] )/ 7;
	Smooth[count - 4] =( num[count-1] + num[count - 2] + num[count - 3]  + num[count - 4] + num[count - 5] + num[count - 6] +num[count - 7]) / 7;
	Smooth[count - 3] = (num[count - 1] + num[count - 2] + num[count - 3] + num[count - 4] + num[count - 5]) / 5;
	Smooth[count - 2] = (num[count - 1] + num[count - 2] + num[count - 3]) / 3;
	Smooth[count - 1] = num[count - 1];
	return Smooth;
}
