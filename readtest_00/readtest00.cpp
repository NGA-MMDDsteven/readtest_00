#define _CRT_SECURE_NO_WARNINGS 
#include "readtest00.h"
#include <opencv2/opencv.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>



#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h> 
#include <iostream>////////////
#include <fstream>

#include"filter.h"           ////后面使用带通滤波器要用！
#include "Iir.h"
#include"LPSD.h"

#include"wavelib.h"
#include"wtmath.h"
#include"windows.h"
#include "WZSerialPort.h"
#include <stdlib.h>
#include <crtdbg.h>


using namespace cv;
using namespace cv::ml;

using namespace std;

extern float save_data_bcg[9000] = { 0 };
extern float save_data_hr[9000] = { 0 };
extern float save_data_rr[9000] = { 0 };
extern int index = 0;
extern int hr_for_ask = 0;
extern int rr_for_ask = 0;
extern Ptr<ANN_MLP> model_1 = ANN_MLP::load("../../NetModel4.xml");
extern Ptr<ANN_MLP> model_2 = ANN_MLP::load("../../NetModel14.xml");
extern Ptr<ANN_MLP> model_3 = ANN_MLP::load("../../NetModel24.xml");

void init_Dataread()
{
	WZSerialPort w;
	w.open("COM3");
	string buff;
	unsigned int bcg;
	int bcg_h;
	unsigned int bcg_l;
	unsigned int hr;
	unsigned int rr;
	time_t tt = time(NULL);
	char ttmp[64];
	strftime(ttmp, sizeof(ttmp), "%Y_%m_%d_%H_%M", localtime(&tt));
	string time_file = ttmp;
	time_file = "d:\\" + time_file + ".txt";
	const char* p = time_file.data();
	while (true)
	{
		buff = w.receive();
		if (buff[6] < 0)
		{
			bcg_l = unsigned int(buff[6] + 256);
		}
		else
			bcg_l = unsigned int(buff[6]);
		if (buff[5] < 0)
		{
			bcg_h = unsigned int(buff[5] + 256);
		}
		else
			bcg_h = unsigned int(buff[5]);

		bcg = bcg_h * 256 + bcg_l;
		hr = buff[8];
		hr_for_ask = hr;
		rr = buff[10];
		rr_for_ask = rr;
		time_t t = time(NULL);
		char tmp[64];
		strftime(tmp, sizeof(tmp), "%Y/%m/%d_%X", localtime(&t));
		string time_now = tmp;
		string bcg_string;
		string hr_string;
		string rr_string;
		bcg_string = to_string(bcg);
		hr_string = to_string(hr);
		rr_string = to_string(rr);
		string textforsave;
		textforsave = time_now + " " + bcg_string + " " + hr_string + " " + rr_string;
		ofstream out(p, ios::app);
		out << textforsave<<endl;
		out.close();
		save_data_bcg[index] = float(bcg);
		save_data_hr[index] = float(hr);
		save_data_rr[index] = float(rr);
		index = (index + 1) % 9000;
	}
}

void GetData(float ECGorRSP[], float HrData[], float RrData[], INT64 timebegin[1], INT64 timeend[1], INT64 timenow[1], int line[1])  //数据每行一个，易取。line用于记录数据的总行数
{
	//float& bcg_buff = (float*)malloc(sizeof(float)*9000);
	//float& hr_buff = (float*)malloc(sizeof(float)*9000);
	//float& rr_buff = (float*)malloc(sizeof(float)*9000);
	//float bcg_buff[9000];
	//float hr_buff[9000];
	//float rr_buff[9000];
	int index_a = index;
	float *bcg_buff = NULL; bcg_buff = (float*)malloc(9000 * 1 * sizeof(float));
	float *hr_buff = NULL; hr_buff = (float*)malloc(9000 * 1 * sizeof(float));
	float *rr_buff = NULL; rr_buff = (float*)malloc(9000 * 1 * sizeof(float));
	memcpy(bcg_buff, save_data_bcg, 9000 * sizeof(float));
	memcpy(hr_buff, save_data_hr, 9000 * sizeof(float));
	memcpy(rr_buff, save_data_rr, 9000 * sizeof(float));
	int time_index;
	int data_length = 0;
	int indexB = (timenow[0] - timebegin[0]) * 50;
	int indexE = (timenow[0] - timeend[0]) * 50;
	for (time_index = ((index_a - indexB) % 9000 + 9000) % 9000; time_index != ((((index_a - indexE) % 9000)+9000) % 9000); time_index = ((time_index + 1) % 9000)) 
	//for (time_index = (9000 + index_a - indexB) % 9000; time_index != (9000 + index_a - indexE + 1) % 9000; time_index = ((9000 + time_index + 1) % 9000))
	{
		ECGorRSP[data_length] = bcg_buff[time_index];
		HrData[data_length] = hr_buff[time_index];
		RrData[data_length] = rr_buff[time_index];
		//cout <<"timeindex:"<< time_index << endl;
		data_length = data_length +1;
		if (data_length == 9000)
			break;
	}
	line[0] = data_length;
	for (; data_length != 9000; time_index = ((time_index + 1) % 9000))
	{
		ECGorRSP[data_length] = bcg_buff[time_index];
		data_length++;
	}
	free(bcg_buff);
	free(hr_buff);
	free(rr_buff);

}
//======================求中位数
int QuickSortOnce(float a[], int low, int high) {
	// 将首元素作为枢轴。
	int pivot = a[low];
	int i = low, j = high;

	while (i < j) {
		// 从右到左，寻找首个小于pivot的元素。
		while (a[j] >= pivot && i < j) {
			j--;
		}

		// 执行到此，j已指向从右端起首个小于或等于pivot的元素。
		// 执行替换。
		a[i] = a[j];

		// 从左到右，寻找首个大于pivot的元素。
		while (a[i] <= pivot && i < j) {
			i++;
		}

		// 执行到此，i已指向从左端起首个大于或等于pivot的元素。
		// 执行替换。
		a[j] = a[i];
	}

	// 退出while循环，执行至此，必定是i=j的情况。
	// i（或j）指向的即是枢轴的位置，定位该趟排序的枢轴并将该位置返回。
	a[i] = pivot;

	return i;
}

void QuickSort(float a[], int low, int high) {
	if (low >= high) {
		return;
	}

	int pivot = QuickSortOnce(a, low, high);

	// 对枢轴的左端进行排序。
	QuickSort(a, low, pivot - 1);

	// 对枢轴的右端进行排序。
	QuickSort(a, pivot + 1, high);
}

float EvaluateMedian(float a[], int n) {
	QuickSort(a, 0, n - 1);

	if (n % 2 != 0) {
		return a[n / 2];
	}
	else {
		return (a[n / 2] + a[n / 2 - 1]) / 2;
	}
}




//===========================
void GetHR(float hr[], int line[1])  //数据每行一个，易取。line用于记录数据的总行数
{

	FILE *fp = fopen("../../data_mine/ecgHRV/HR.txt", "r");//data_BCG\\BCGraw\\bcggood_5min.txt", "r");
	char temp_StrLine[100];             //每行最大读取的字符数

	if (fp == NULL)                               //判断文件是否存在及可读
	{
		printf("error!");
		//return -1;
	}


	int j = 0;                        //记录文件中数据行数，循环!!!!!原先把j=0放到循环里根本不对啊！
	while (!feof(fp))                                  //检测文件结束符，每次循环为一行
	{

		fgets(temp_StrLine, 100, fp);    //读取一行
		char datatemp[10]; // 存每一行数据;"={0}"以及大小比实际所取多1是为了防止乱码

		strncpy(datatemp, temp_StrLine, 9);


		//下面将上面得到的字符串转换为数字
		float datatemp2; // 格式转换过渡

		datatemp2 = atof(datatemp);

		hr[j] = datatemp2;
		//printf("%f\n", Data[j]);

		j++;   //记录文件中数据行数，循环

	}
	line[0] = j - 1;
	fclose(fp);
	
}

//==================
void GetECGrr(float ECGorRSP[], int line[1])  //数据每行一个，易取。line用于记录数据的总行数
{

	FILE *fp = fopen("../../data_mine/ecgrr/ecg021.txt", "r");//data_BCG\\BCGraw\\bcggood_5min.txt", "r");
	char temp_StrLine[100];             //每行最大读取的字符数

	if (fp == NULL)                               //判断文件是否存在及可读
	{
		printf("error!");
		//return -1;
	}

	int j = 0;                        //记录文件中数据行数，循环!!!!!原先把j=0放到循环里根本不对啊！
	while (!feof(fp))                                  //检测文件结束符，每次循环为一行
	{
		fgets(temp_StrLine, 100, fp);    //读取一行
		char datatemp[10]; // 存每一行数据;"={0}"以及大小比实际所取多1是为了防止乱码
		strncpy(datatemp, temp_StrLine, 9);

		//下面将上面得到的字符串转换为数字
		float datatemp2; // 格式转换过渡
		datatemp2 = atof(datatemp);
		ECGorRSP[j] = datatemp2;
		//printf("%f\n", Data[j]);
		j++;   //记录文件中数据行数，循环

	}
	line[0] = j - 1;
	////free(Data); ///
}

void GetECGposi(float ECGorRSP[])  //, int line[1])  //数据每行一个，易取。line用于记录数据的总行数
{

	FILE *fp = fopen("../../data_mine/ecgRposi/ecg021.txt", "r");//data_BCG\\BCGraw\\bcggood_5min.txt", "r");
	char temp_StrLine[100];             //每行最大读取的字符数

	if (fp == NULL)                               //判断文件是否存在及可读
	{
		printf("error!");
		//return -1;
	}

	int j = 0;                        //记录文件中数据行数，循环!!!!!原先把j=0放到循环里根本不对啊！
	while (!feof(fp))                                  //检测文件结束符，每次循环为一行
	{

		fgets(temp_StrLine, 100, fp);    //读取一行
		char datatemp[10]; // 存每一行数据;"={0}"以及大小比实际所取多1是为了防止乱码

		strncpy(datatemp, temp_StrLine, 9);

		//下面将上面得到的字符串转换为数字
		float datatemp2; // 格式转换过渡

		datatemp2 = atof(datatemp);

		ECGorRSP[j] = datatemp2;
		//printf("%f\n", Data[j]);

		j++;   //记录文件中数据行数，循环

	}
	fclose(fp);
}
//void CalcSDNN（）

//===================================================================================================================================================================
//首先明确ecg采集频率（如aubt中256Hz，这样每两个数据之间的间隔为 1/256= 0.00390625 s，算RR'NN interval时会用到）

static int SampFrequency = 50;     ////使用样本中bcg采样频率为256Hz，换了需要改
static int THRESHOLD1 = 600;	//Lowest threshold of Rpeak ,观察后发现的;不用特别高
static int RR_LOW = 120;	//正常安静状态（N）下，Lowest interval of a Rpeak occurrence ，ms
static int RR_HIGH = 200;//246;	//正常安静状态（N）下，Highest interval of a Rpeak occurrence， ms

//=============================================================================================================================================================

float getPeaks_andRpeaks(int numofdata,float *data, float *Rsave,float *RRinterval,int countRpeaks[1]) ////data是原始ECG序列中的每个值，其【i】为和对应的值的序号，后面dataesave用于存储是peak的值与位置
{
	float interval = 0;    //两个可能R峰间隔的时间， ms
	float SUM_interval = 0;    //最后返回RR interval平均值需要

	int possibleRlast = 0;    //记录上一个可能的R峰位置
	int possibleR = 0;        //记录目前循环到的可能R峰的位置
	//int normalbeat = 0;      //正常心跳总数，以正常RRinterval数量计

    //============== find R
	float ecg[150],temp1[149], output[148]; /*array for double differentiation*/  //一般心跳60-100，每秒次数 1——1.67 次/s，RR ms/个为：600-1000；频率256Hz，窗宽最好为 153.6-256
	int iter1 = 0, iter2 = 0, iter3 = 0;
	int window_curr = 0, max = 0;

	for (int i = 1; i < numofdata - 1; i++)
	{
		ecg[iter1] = data[i]; /*amplify the sample value for ease of differentiation*/
		iter1 = iter1 + 1;
		if (iter1 == 150)
		{ /*once a window size of samples has been read in*/
			for (iter2 = 0; iter2 < 149; iter2++) 
			{
				temp1[iter2] = ecg[iter2 + 1] - ecg[iter2]; /*first difference*/
			}
			for (iter3 = 0; iter3 < 148; iter3++) 
			{
				output[iter3] = (temp1[iter3 + 1] - temp1[iter3])*(temp1[iter3 + 1] - temp1[iter3]); /*second difference and squaring*/
			}
			max = 0;
			for (iter3 = 0; iter3 < 148; iter3++)
			{ /*determine peak value in each window*/
				if (output[iter3] > output[max])
				{
					max = iter3;
				}
			}
			if (data[150 * window_curr + max] > THRESHOLD1)  //要大于阈值，才可存储
			{
				//// 输出Rpeak的位置                  //fprintf(outputfile, "%d \n", 150 * window_curr + max + 1); 
				possibleR = 150 * window_curr + max; //// +1;
				interval = ((possibleR - possibleRlast) * 1000) / SampFrequency;    //单位为ms
				RRinterval[countRpeaks[0]] = interval;            ////存储每个RRinterval，注意RRinterval的下标，存储时是从头【0】开始连续的

				Rsave[countRpeaks[0] * 2] = possibleR; //存下普通R峰位置与值，注意两个一对，下标 ；此为位置
				Rsave[countRpeaks[0] * 2 + 1] = data[possibleR];   //此为值


				SUM_interval = SUM_interval + interval;        ////准备最后求RRinterval 平均值

				possibleRlast = possibleR;    //赋值，用于下一次interval计算

				countRpeaks[0]++;   //R峰数量总计
			}


			iter1 = 0;
			window_curr = window_curr + 1;
		}
	}

     //============

	float RRaver = SUM_interval / countRpeaks[0];
	return RRaver;                                             ////RR interval的平均值，单位：  ms
	
}
//===============================================================
//下面函数是计算频域方面HRV特征值的，需要先明确几个频段的范围

 float uLF_range = 0.003;                                  ////ultra low frequency range < 0.003 Hz
 float vLF_range1 = 0.003; float vLF_range2 = 0.04;   ////         0.003 Hz  < =   very low frequency range   <   0.04 Hz
 float LF_range1 = 0.04;  float LF_range2 = 0.15;     ////        0.04 Hz  < =    low frequency range   <   0.15 Hz
 float HF_range1 = 0.15;  float HF_range2 = 0.4;       ////        0.15 Hz  < =    high frequency range   <   0.4 Hz

 extern float *LPSD(float *num, float *tnn, float *nn, int count_fft, int count_rr);
//================================================================
 float ECG_LFHFRatio(float *JJinterval, float *Rposi,int countJminus1, double LF[1], double HF[1], double LFn[1], double HFn[1])  //对N个点的信号做fft，其频率范围就是 0-fs，第k点代表的频率就是 k*fs /N 。N个点FFT值关于 N/2 对称。设fs为0.5
 {
	 // 得到DFT后的每一个Xk的Re和Im
	 int N = countJminus1;
	 float fs = 0.5;
	 float space = fs / N;   ////每两点中间的频率差
	 float fk = 0;  ////每个点对应的的频率值

	 double ulf = 0;
	 double vlf = 0;
	 double lf = 0;  ////是lf段所有功率的总和
	 double hf = 0;  ////是lf段所有功率的总和
	 double totalp = 0;  ////四频段总功率
	 double lfn = 0;
	 double hfn = 0;    ////normalized
	 float lfhfratio = 0;   ////返回值
	 int countfft = 512;   ////countfft的数量要大于JJ序列中的数量N，因为5min中顶多有300多个JJ值，所以512是足够的
							////////////////////  Lomb法，尝试LPSD函数
	 float *fre_vec = NULL; fre_vec = (float*)malloc(countfft * 1 * sizeof(float));  ////频率向量！
	 memset(fre_vec, 0, countfft * 1 * sizeof(float));
	 for (int i = 0; i < countfft; i++)
	 {
		 fre_vec[i] = i*space;
	 }

	 float *tnn = NULL; tnn = (float*)malloc(countJminus1 * 1 * sizeof(float));
	 for (int i = 0; i < N; i++)
	 {
		 tnn[i] = Rposi[i]/256;   ////时间位置，n/hz，单位为s
	 }

	

	 float *psd = NULL; psd = LPSD(fre_vec, tnn, JJinterval, countfft, N);

	 for (int i = 0; i < N; i++)
	 {
		 if (fre_vec[i] < uLF_range)  ulf = ulf + psd[i] * space;
		 if ((fre_vec[i] > vLF_range1) && (fre_vec[i] < vLF_range2))  vlf = vlf + psd[i] * space;
		 if ((fre_vec[i] > LF_range1) && (fre_vec[i] < LF_range2))  lf = lf + psd[i] * space;
		 if ((fre_vec[i] > HF_range1) && (fre_vec[i] < HF_range2))  hf = hf + psd[i] * space;
		//// printf("\n\n 第%d个 ：ulf=%f \t vlf=%f \t lf=%f \t hf=%f \n", i, ulf, vlf, lf, hf);
	 }
	 totalp = ulf + vlf + lf + hf;
	 lfn = lf / totalp;
	 hfn = hf / totalp;
	 lfhfratio = lf / hf;
	//// printf("\n\n 总能量： %f \n", totalp);

	 LF[0] = lf;
	 HF[0] = hf;
	 LFn[0] = lfn;
	 HFn[0] = hfn;
	 free(fre_vec);
	 free(tnn);
	 free(psd);
	 return lfhfratio * 100;  ////*100因为出去之后有百分号

 }
 //================================================================
//求频域特征前对数据进行离散傅里叶变换
#define PI 3.1415926535 // 円周率
//#define DELTA 0.00002   // サンプリング周期

// フーリエ変換
float DFT_LFHFRatio(float *JJinterval,int countJminus1, double LF[1], double HF[1], double LFn[1], double HFn[1])  //对N个点的信号做fft，其频率范围就是 0-fs，第k点代表的频率就是 k*fs /N 。N个点FFT值关于 N/2 对称。设fs为0.5
{
	// 得到DFT后的每一个Xk的Re和Im
	int N = countJminus1;
	float fs = 0.5;
	float space = fs / N;   ////每两点中间的频率差
	float fk = 0;  ////每个点对应的的频率值

	double ulf = 0;
	double vlf = 0;
	double lf = 0;  ////是lf段所有功率的总和
	double hf = 0;  ////是lf段所有功率的总和
	double totalp = 0;  ////四频段总功率
	double lfn = 0;
	double hfn = 0;    ////normalized
	float lfhfratio = 0;   ////返回值
	
	////////////////////  Lomb法，尝试LPSD函数
	float *fre_vec = NULL; fre_vec = (float*)malloc(countJminus1 * 1 * sizeof(float));  ////频率向量！
	for (int i = 0; i < N; i++)
	{
		fre_vec[i] = i*space;
	}

	float *tnn = NULL; tnn = (float*)malloc(countJminus1 * 1 * sizeof(float));
	for (int i = 0; i < N; i++)
	{
		tnn[i] = i;
	}

	int countfft = 512;   ////countfft的数量要大于JJ序列中的数量N，因为5min中顶多有300多个JJ值，所以512是足够的

	float *psd = NULL; psd = (float*)malloc(countJminus1 * 1 * sizeof(float));

	psd = LPSD(fre_vec, tnn, JJinterval, countfft, N);

	for (int i = 0; i < N; i++)
	{
		if (fre_vec[i] < uLF_range)  ulf = ulf + psd[i]*space;
		if ((fre_vec[i] >vLF_range1) && (fre_vec[i] < vLF_range2))  vlf = vlf + psd[i] * space;
		if ((fre_vec[i] > LF_range1) && (fre_vec[i] < LF_range2))  lf = lf + psd[i] * space;
		if ((fre_vec[i] > HF_range1) && (fre_vec[i] < HF_range2))  hf = hf + psd[i] * space;
	}
/////////////////////////
	totalp = ulf + vlf + lf + hf;
	lfn = lf / totalp;
	hfn = hf / totalp;
	lfhfratio = lf / hf;

	LF[0] = lf;
	HF[0] = hf;
	LFn[0] = lfn;
	HFn[0] = hfn;
	return lfhfratio * 100;  ////*100因为出去之后有百分号

}
//==============================================

//===============================================================
//庞加莱图
void Poincare(float SDNN,float SDSD,double SD1[1],double SD2[1])
{
	SD1[0] = sqrt(SDSD*SDSD / 2);
	SD2[0] = sqrt(2 * SDNN*SDNN - SDSD*SDSD / 2);
}

//=============================================
float Hexinput2deci(char databit)
{
	float deci_output=0;
	if (databit == '0' || databit == '1' || databit == '2' || databit == '3' || databit == '4' || databit == '5' || databit == '6' || databit == '7' || databit == '8' || databit == '9')
		deci_output = databit - '0';
	if (databit == 'A') deci_output = 10;
	if (databit == 'B') deci_output = 11;
	if (databit == 'C') deci_output = 12;
	if (databit == 'D') deci_output = 13;
	if (databit == 'E') deci_output = 14;
	if (databit == 'F') deci_output = 15;
	return deci_output;
}
//===========================================================================
//改进版J峰提取中需要的函数


void detectmovement(int startpoint, int endpoint, int windowshift, float *bcgHR, float *afterdata)
{
	int limit = floor(9000 / windowshift);    // limit = int(math.floor(data.size / win_size))
	int *flag = NULL; flag= (int*)malloc(9000 * 1 * sizeof(int));
	int *eventflag = NULL; eventflag = (int*)malloc(limit * 1 * sizeof(int));
	float *subdata = NULL; subdata = (float*)malloc(windowshift * 1 * sizeof(float));
	float *segment_sd = NULL; segment_sd = (float*)malloc(limit * 1 * sizeof(float));
	memset(flag, 0, 9000 * 1 * sizeof(int));
	memset(eventflag, 0, limit * 1 * sizeof(int));
	memset(subdata, 0, windowshift * 1 * sizeof(float));
	memset(segment_sd, 0, limit * 1 * sizeof(float));

	for (int i = 0; i < limit; i++)
	{
		int sum = 0, mean = 0, SD = 0;
		for (int j = i*windowshift; j < i*windowshift + windowshift; j++)
		{
			subdata[j- i*windowshift] = bcgHR[j];
			sum = sum + subdata[i];
		}
		mean = sum / windowshift;
		for (int jj = 0; jj < windowshift; jj++)
		{
			SD += pow(subdata[jj] - mean, 2);
		}
		SD = sqrt(SD / windowshift);
		segment_sd[i] = SD;
	}
	////
	float segmentsd_mean = 0;
	for (int i = 0; i < limit; i++)
	{
		segmentsd_mean = segmentsd_mean + segment_sd[i];
	}
	segmentsd_mean = segmentsd_mean / limit;
	float segabssum = 0;
	for (int i = 0; i < limit; i++)
	{
		segabssum = segabssum + abs(segment_sd[i] - segmentsd_mean);
	}
	////
	float mad = segabssum / limit;
	float thresh1 = 15, thresh2 = 2*mad;
////下面寻找有活动的点
	for (int j = 0; j < limit; j++)
	{
		float std_fos = round(segment_sd[j]);
		for (int jj = j*windowshift; jj <  j*windowshift+windowshift; jj++)
		{
			if (std_fos < thresh1)  //no movement
			{
				flag[jj] = 3; eventflag[jj- j*windowshift] = 3;

			}
			if (std_fos > thresh2)  // movement
			{
				flag[jj] = 2; eventflag[jj - j*windowshift] = 2;
			}
			if (std_fos >= thresh1 && std_fos <= thresh2)  //sleeping
			{
				flag[jj] = 1; eventflag[jj - j*windowshift] = 1;
			}
		}
	}
////除去移动的点
	for (int i = 0; i < 9000; i++)
	{
		afterdata[i] = bcgHR[i];
	}
	for (int j = 0; j < limit; j++)
	{
		for (int jj = j*windowshift; jj < j*windowshift + windowshift; jj++)
		{
			if (flag[jj]==2 || flag[jj]==3) //no movement or movement
			{
				afterdata[jj] = 0;    //该点的值被除去
			}
		}
	}
//printf出处理后的值

	free(eventflag); free(flag); free(subdata); free(segment_sd);
}

void butterFilter(float *rawbcg, float *afterfilter)   //采样频率 fs=50Hz
{
	//float *afterbutter = NULL; afterbutter = (float*)malloc(40000 * 1 * sizeof(float));
	//Butterworthfilter(bcgHR, line[0], afterbutter);
	const int order = 6; // 6th order (=2 biquads)
	Iir::Butterworth::BandPass<order> butter;
	const float samplingrate = 50; // Hz
	const float center_frequency = 5.35; // Hz  0.7-10
	const float width_frequency = 4.65; // Hz
	butter.setup(samplingrate, center_frequency, width_frequency);
	for (int i = 0; i < 9000; i++)  ////共9000个数据
	{
		afterfilter[i] = butter.filter(rawbcg[i]);
	}
}

//=================小波变换！=======================================================================
double absmax1(double *array, int N)
{
	double max;
	int i;

	max = 0.0;
	for (i = 0; i < N; ++i) {
		if (fabs(array[i]) >= max) {
			max = fabs(array[i]);
		}
	}

	return max;
}

void wavelet(float *afterfilter, float level, float *wcycle) //最后一项为输出
{
	char *name = "sym8";    ////////////选用的name
	wave_object obj = wave_init(name);
	wave_summary(obj);

	wt_object wt;

	double *inp=NULL, *out = NULL, *diff = NULL;
	int N, i, J;
	
	//double temp[1200];
	

	N = 9000;
	inp = (double*)malloc(sizeof(double)* N);
	out = (double*)malloc(sizeof(double)* N);
	diff = (double*)malloc(sizeof(double)* N);
	memset(inp, 0, sizeof(double)* N);
	memset(out, 0, sizeof(double)* N);
	memset(diff, 0, sizeof(double)* N);



	for (int j = 0; j < 9000; j++)
	{
		inp[j] = afterfilter[j];
	}
	//wmean = mean(temp, N);

	J = level;        ///////改成4
	wt = wt_init(obj, "modwt", N, J);// ///////Initialize the wavelet transform object
    modwt(wt, inp);// Perform MODWT
					   //MODWT output can be accessed using wt->output vector. Use wt_summary to find out how to extract appx and detail coefficients
	wt_summary(wt);// Prints the full summary.
	double *mra = NULL; //mra = (double*)malloc(5 * 9000 * sizeof(double));
	//double *wavecoeffs = NULL;
	mra=getMODWTmra(wt, NULL);
	for (int j = 0; j < 9000; j++)
	{
		wcycle[j] = mra[4 * 9000 + j];
	}
	wave_free(obj);
	wt_free(wt);
	free(inp);
	free(out);
	free(diff);

	free(mra);
	//free(wavecoeffs);
	
}

//==============================================================
//Butterworth 带通滤波器只算系数。在dft以后，每个数据下面对应的频率是 （k*50hz/n）k从0到n-1
//extern void Butterworthfilter(float *in, int datanumline0, float *out); //in为原始数据，out为滤波后数据，个数均为datanumline0
extern vector<int> findPeaks(float *num, int count,float *position, int numofposition[1]);
extern float *Smoothdata(float *num, int count);
//=================================================
float GetBCGJJinterval(float bcgHR[], float Hr[], float Rr[], int Hr_max[1], int Hr_min[1], float Hr_aver[], float Rr_aver[], int line[1], float *jsave, float *jjinterval, int countjpeaks[1], INT64 timebegin[1], INT64 timeend[1], INT64 timenow[1])//,int countRespeaks[1])  //数据每行一个，易取。line用于记录数据的总行数
{
	GetData(bcgHR, Hr, Rr, timebegin, timeend, timenow, line);   ////"E:\\NEW_HAQ_5.11\\New_文献\\00_中期后工作\\00_MY_TEST\\data_mine\\data_BCG\\BCGraw\\bcggood_5min.txt"

	int window;
	window = line[0] - 1 ;


	float buff_max = int(Hr[0]);
	float buff_min = int(Hr[0]);
	float buff_sum = int(Hr[0]);
	float buff_sum_rr = int(Rr[0]);
	float Hr_buff;
	for (int csgo = 1; csgo < window; csgo++)
	{
		Hr_buff = float(Hr[csgo]);
		buff_sum = buff_sum + Hr_buff;
		buff_sum_rr = buff_sum_rr + Rr[csgo];
		if (Hr_buff > buff_max && 140> Hr_buff)
			buff_max = Hr_buff;
		if (Hr_buff < buff_min && 1< Hr_buff)
			buff_min = Hr_buff;
	}
	Hr_max[0] = int(buff_max);
	Hr_min[0] = int(buff_min);
	Hr_aver[0] = buff_sum / window;
	Rr_aver[0] = buff_sum_rr / window;
	float *afterfilter = NULL; afterfilter = (float*)malloc(9000 * 1 * sizeof(float));
	memset(afterfilter, 0, 9000 * 1 * sizeof(float));
	butterFilter(bcgHR, afterfilter);
	float *wcycle = NULL; wcycle = (float*)malloc(9000 * 1 * sizeof(float));
	memset(wcycle, 0, 9000 * 1 * sizeof(float));
	wavelet(afterfilter, 4, wcycle);                                       ///////小波变换！要看一下上面的函数
	findPeaks(wcycle, window,jsave,countjpeaks);   ////jsave保存的是J峰在原数据的位置下标
//=======================================

	float sum_interval = 0;
	//这个是取中间JJ数据算特征值，3min理论上会有200多个JJinterval.前面掐掉一些后面掐掉一些不好的，用中间的算
	for (int j = 0; j < countjpeaks[0]; j++)
	{
		jjinterval[j] = ((jsave[j + 1] - jsave[j]) * 1000) / SampFrequency;    // 求jj间隔序列，单位为ms

		sum_interval = sum_interval + jjinterval[j];  ////和用于返回求AVNN
	}
//	countjpeaks[0] = countjpeaks[0] - 10 - 10;     /////就不是真实的J峰数量了，只是后面显示 的“Jpeaks used”
	
	float AVNN = sum_interval / countjpeaks[0];

	free(afterfilter);
	free(wcycle);

	return AVNN;                   ////返回avnn

}
////==========================


float AVNN_ECG(float *jjinterval,int line[1])//,int countRespeaks[1])  //数据每行一个，易取。line用于记录数据的总行数
{

	float sum_interval = 0;

	//这个是取中间几百个数据算特征值，5min理论上会有300多个JJinterval
	for (int j = 0; j < line[0]; j++)
	{
		
		sum_interval = sum_interval + jjinterval[j];  ////和用于返回求AVNN
	}

	////
	float AVNN = sum_interval / line[0];
	return AVNN;                   ////返回avnn

}
//===================================================
//Sampen
/*
"where OPTIONS may include:",
" -h          print this usage summary",
" -m M        set the maximum epoch length to M (default: 2)",
" -n          normalize such that the mean of the input is 0 and the sample",
"             variance is 1",
" -r R        set the tolerance to R (default: 0.2)",
" -v          output an estimate of the standard deviation of each SampEn",
"TEXT-FILE should contain the time series (a column of decimal numbers);",
"if omitted, sampen reads its standard input.  The output contains values of",
"SampEn(k,r,N) where k is the epoch length, r is the tolerance, and N is the",
"length of the input series.",
*/
float Sampen(float *y, int M, float r, int n)

// calculates an estimate of sample entropy but does NOT caclulate the variance of the estimate 
{
	float *p = NULL;
	//float *e = NULL;
	long *run = NULL, *lastrun = NULL, N;
	float *A = NULL, *B = NULL;
	int M1, j, nj, jj, m;
	int i;
	float y1;

	if ((run = (long*)calloc(n, sizeof(long))) == NULL)	exit(1);
	if ((lastrun = (long*)calloc(n, sizeof(long))) == NULL)	exit(1);
	if ((A = (float*)calloc(M, sizeof(float))) == NULL)	exit(1);
	if ((B = (float*)calloc(M, sizeof(float))) == NULL)	exit(1);
	if ((p = (float*)calloc(M, sizeof(float))) == NULL)	exit(1);

	/*start running*/
	for (i = 0; i<n - 1; i++)
	{
		nj = n - i - 1;
		y1 = y[i];
		for (jj = 0; jj<nj; jj++)
		{
			j = jj + i + 1;
			if (((y[j] - y1)<r) && ((y1 - y[j])<r))
			{
				run[jj] = lastrun[jj] + 1;
				M1 = M<run[jj] ? M : run[jj];
				for (m = 0; m<M1; m++)
				{
					A[m]++;
					if (j<n - 1) B[m]++;
				}
			}
			else  run[jj] = 0;
		} /* for jj*/
		for (j = 0; j<nj; j++)  lastrun[j] = run[j];
	} /* for i*/

	N = (long)(n*(n - 1) / 2);
	p[0] = A[0] / N;
	float pp = -log(p[0]);
	free(A);
	free(B);
	free(p);
	free(run);
	free(lastrun);

	return pp;


}
int ReturnLabel(INT64 time_begin, INT64 time_end)
{
	INT64 timebegin[1] = { time_begin };          //收到的参数起始时间 整型
	INT64 timeend[1] = { time_end };			//收到的参数终止时间 整型
	time_t rightnow = time(NULL);
	INT64 A = (INT64)rightnow;
	INT64 timenow[1] = { A };            //函数调用获取的当前时间 整型

	////准备缓存区
	int line[1] = { 0 };  //文件中数据总条数
	int peakcount[1] = { 0 };  //所有普通peak数
	int jcount[1] = { 0 };  //所有普通Rpeak数
	int normalbeat[1] = { 0 };  //正常心跳数，即正常RR间隔总数
	int Hr_max[1] = { 0 };
	int Hr_min[1] = { 0 };
	float Hr_aver[1] = { 0 };
	float Rr_aver[1] = { 0 };
	float *Data = NULL; Data = (float*)malloc(10000 * 1 * sizeof(float));    ////每组ecg信号2min，30720 个
	float *Hr_Data = NULL; Hr_Data = (float*)malloc(10000 * 1 * sizeof(float));
	float *Rr_Data = NULL; Rr_Data = (float*)malloc(10000 * 1 * sizeof(float));
	float *jsave = NULL; jsave = (float*)malloc(10000 * 1 * sizeof(float));          ////用来存储R-peak的位置
	float *jjinterval = NULL; jjinterval = (float*)malloc(10000 * 1 * sizeof(float));          ////用来存储序列中一般 RR间隔时间


	////j峰检测
	float AVNN = GetBCGJJinterval(Data, Hr_Data, Rr_Data, Hr_max, Hr_min, Hr_aver, Rr_aver, line, jsave, jjinterval, jcount, timebegin, timeend, timenow);//getPeaks_andRpeaks(line[0], Data,  Rsave, RRinterval,  jcount);  ////返回的是括号内的内容 和 RR的平均值；注意RRinterval是从【0】开始连续保存的
	//float AVNN = AVNN_ECG(jjinterval, line);
	////HRV时域计算：SDNN、NN50、PNN50、rmssd
	float numerator_SDNN = 0;              //sum [ (RRi-RRaver) ^2 ]
	float numerator_RMSSD = 0;             //sum [ (RRi-RR[i-1]) ^2 ]
	int NN50 = 0;                       //相差超过50ms的相邻RR对的数量，整数


	if (jcount[0] <= 3)
	{
		free(Data);
		free(Hr_Data);
		free(Rr_Data);
		free(jsave);
		free(jjinterval);
		return 0;
	}

	for (int i = 0; i < jcount[0]; i++)
	{
		numerator_SDNN = numerator_SDNN + (jjinterval[i] - AVNN)*(jjinterval[i] - AVNN);
		////RMSSD的分子从第二个RR开始
		if (i != 0)
		{
			numerator_RMSSD = numerator_RMSSD + (jjinterval[i] - jjinterval[i - 1])*(jjinterval[i] - jjinterval[i - 1]);

			////NN50要比较间隔大小，也是从第二个RR开始
			if ((jjinterval[i] - jjinterval[i - 1]) > 50)
				NN50++;
		}

	}

	float SDNN = sqrt(numerator_SDNN / jcount[0]);    ////所有窦性NN间期的标准差
	float RMSSD = sqrt(numerator_RMSSD / (jcount[0] - 1));
	float PNN50 = (NN50 * 100) / (jcount[0] - 2);          ////表示时后面加上百分号

	float numerator_SDSD = 0;

	float jjinter_aver = 0;
	for (int i = 1; i < jcount[0]; i++)
	{
		jjinter_aver = jjinter_aver + jjinterval[i] - jjinterval[i - 1];
	}
	jjinter_aver = jjinter_aver / (jcount[0] - 1);
	for (int i = 0; i < jcount[0]; i++)
	{
		if (i != 0)
		{
			////numerator_SDSD = numerator_SDSD + (jjinterval[i] - jjinterval[i - 1] - RMSSD*RMSSD)* (jjinterval[i] - jjinterval[i - 1] - RMSSD*RMSSD);
			numerator_SDSD = numerator_SDSD + (jjinterval[i] - jjinterval[i - 1] - jjinter_aver)* (jjinterval[i] - jjinterval[i - 1] - jjinter_aver);
		}
	}
	float SDSD = sqrt(numerator_SDSD / (jcount[0] - 1));   ////相邻NN间期差值（长度差异）的标准差


	////HRV频域特征====================================
	double LF[1] = { 0 };
	double HF[1] = { 0 };
	double LFn[1] = { 0 };
	double HFn[1] = { 0 };

	//float lfhf_ratio = DFT_LFHFRatio(jjinterval,jcount[0]-1,LF, HF, LFn, HFn);
	float lfhf_ratio = ECG_LFHFRatio(jjinterval, jsave, jcount[0] - 1, LF, HF, LFn, HFn);  ////ECG_LFHFRatio(jjinterval, jsave,line[0], LF, HF, LFn, HFn);


	////非线性HRV特征
	double SD1[1] = { 0 };
	double SD2[1] = { 0 };
	Poincare(SDNN, SDSD, SD1, SD2);

	float m = 1;
	float r = 0.25*SDNN;
	int n = jcount[0];
	float sampen = Sampen(jjinterval, m, r, n);
	float SDNN_min = 63.686817;
	float SDNN_max2min = 96.321976 - 63.686817;
	float L2H_min = 0.000056;
	float L2H_max2min = 571.078918- 0.000056;
	float SD2_min = 62.39998;
	float SD2_max2min = 104.413231- 62.39998;
	float SampEn_min = 1.151969;
	float SampEn_max2min = 2.422365 - 1.151969;
	float HRaver_min = 60.37966667;
	float HRaver_max2min = 23.21933333;
	float Rraver_min = 10.52066667;
	float Rraver_max2min = 9.25844444;
	float SDNN_g;
	float L2H_g;
	float SD2_g;
	float SampEn_g;
	float HRaver_g;
	float Rraver_g;
	float Hr_max_g = float(Hr_max[0]) / 100.0;
	float Hr_min_g = float(Hr_min[0]) / 100.0;

	if (SDNN < SDNN_min)
		SDNN_g = 0;
	else
		SDNN_g = (SDNN - SDNN_min) / SDNN_max2min;

	if (lfhf_ratio < L2H_min)
		L2H_g = 0;
	else
		L2H_g = (lfhf_ratio - L2H_min) / L2H_max2min;

	if (SD2[1] < SD2_min)
		SD2_g = 0;
	else
		SD2_g = (SD2[1] - SD2_min) / SD2_max2min;

	if (sampen < SampEn_min)
		SampEn_g = 0;
	else
		SampEn_g = (sampen - SampEn_min) / SampEn_max2min;

	if (Hr_aver[1] < HRaver_min)
		HRaver_g = 0;
	else
		HRaver_g = (Hr_aver[1] - HRaver_min) / HRaver_max2min;

	if (Rr_aver[1] < Rraver_min)
		Rraver_g = 0;
	else
		Rraver_g = (Rr_aver[1] - Rraver_min) / Rraver_max2min;
	/*
	Ptr<ANN_MLP> model_1 = ANN_MLP::load("../../NetModel1.xml");
	Ptr<ANN_MLP> model_2 = ANN_MLP::load("../../NetModel11.xml");
	Ptr<ANN_MLP> model_3 = ANN_MLP::load("../../NetModel21.xml");
	Ptr<ANN_MLP> model_4 = ANN_MLP::load("../../NetModel15.xml");
	*/

	
	Mat sampleMat = (Mat_<float>(1, 8) << SDNN_g, L2H_g, SD2_g, SampEn_g, HRaver_g, Rraver_g, Hr_max_g, Hr_min_g);
	Mat responseMat_1;
	Mat responseMat_2;
	Mat responseMat_3;
	Mat responseMat_4;
	model_1->predict(sampleMat, responseMat_1);
	model_2->predict(sampleMat, responseMat_2);
	model_3->predict(sampleMat, responseMat_3);
	/*
	float rr_1 = responseMat_1.ptr<float>(0)[0];
	float rr_2 = responseMat_2.ptr<float>(0)[0];
	float rr_3 = responseMat_3.ptr<float>(0)[0];
	float rr_4 = responseMat_4.ptr<float>(0)[0];
	float rr_mean = rr_1 + rr_2 + rr_3 + rr_4 / 4;
	int label;
	label = 100 - int(rr_mean * 100);
	if (label > 100)
		label = 100;
	else if (label < 0)
		label = 0;
	*/
	float rr_1_0 = responseMat_1.ptr<float>(0)[0];
	float rr_1_1 = responseMat_1.ptr<float>(0)[1];
	float rr_1_2 = responseMat_1.ptr<float>(0)[2];
	float rr_2_0 = responseMat_2.ptr<float>(0)[0];
	float rr_2_1 = responseMat_2.ptr<float>(0)[1];
	float rr_2_2 = responseMat_2.ptr<float>(0)[2];
	float rr_3_0 = responseMat_3.ptr<float>(0)[0];
	float rr_3_1 = responseMat_3.ptr<float>(0)[1];
	float rr_3_2 = responseMat_3.ptr<float>(0)[2];
	float rr_mean = 0.5;
	float rr_1 = (0.5*abs(rr_1_1) + 1 * abs(rr_1_2)) / (abs(rr_1_0) + abs(rr_1_1) + abs(rr_1_2));
	float rr_2 = (0.5*abs(rr_2_1) + 1 * abs(rr_2_2)) / (abs(rr_2_0) + abs(rr_2_1) + abs(rr_2_2));
	float rr_3 = (0.5*abs(rr_3_1) + 1 * abs(rr_3_2)) / (abs(rr_3_0) + abs(rr_3_1) + abs(rr_3_2));
	float rr_array[5];
	rr_array[0] = rr_1;
	rr_array[1] = rr_2;
	rr_array[2] = rr_3;
	rr_array[3] = rr_1;
	rr_array[4] = rr_2;
	for (int i = 0; i < 3; i++)
	{
		if (abs(rr_array[i] - rr_array[i + 1]) > 0.333)
		{
			if (abs(rr_array[i] - rr_array[i + 2]) > 0.333)
			{
				rr_mean = (rr_array[i + 1] + rr_array[i + 2]) / 2;
				break;
			}
			else
			{
				rr_mean = (rr_array[i] + rr_array[i + 2]) / 2;
				break;
			}
		}
	}
	if(abs(rr_1-rr_2) <0.33 && abs(rr_1 - rr_3) < 0.33 && abs(rr_3 - rr_2) < 0.33)
		rr_mean = (rr_1 + rr_2 + rr_3) / 3;
	
	
	int label;
	label = int(rr_mean * 100);
	if (label > 100)
		label = 100;
	else if (label < 0)
		label = 0;
		
	/*
	int label1 = 0;
	int label2 = 0;
	int label3 = 0;

	if (rr_1 < 0.3)
		label1 = label1 + 1;
	else if (rr_1 > 0.7)
	{
		label3 = label3 + 1;
	}
	else
	{
		label2 = label2 + 1;
	}
	if (rr_2 < 0.3)
		label1 = label1 + 1;
	else if (rr_2 > 0.7)
	{
		label3 = label3 + 1;
	}
	else
	{
		label2 = label2 + 1;
	}
	if (rr_3 < 0.3)
		label1 = label1 + 1;
	else if (rr_3 > 0.7)
	{
		label3 = label3 + 1;
	}
	else
	{
		label2 = label2 + 1;
	}

	int label;
	if (label1 > 1)
		label = 0;
	else if (label2 > 1)
		label = 1;
	else if (label3 > 1)
		label = 2;
	else
	{
		if (rr_4 < 0.3)
			label = 0;
		else if (rr_3 > 0.7)
		{
			label = 2;
		}
		else
		{
			label = 1;
		}
	}
	*/
	free(Data);
	free(Hr_Data);
	free(Rr_Data);
	free(jsave);
	free(jjinterval);
	return label;
}
int Return_Hr()
{
	return hr_for_ask;
}
int Return_Rr()
{
	return rr_for_ask;
}

//===================================================
int detect_rr_abnormal()
{
	float bcg_buff[9000];
	float bcg_buf[9000];
	memcpy(bcg_buff, save_data_bcg, 9000 * sizeof(float));
	int index_now = index;
	for (int i = 0; i < 9000; i++)  ////共9000个数据
	{
		bcg_buf[i] = bcg_buff[((index + i + 9000) % 9000)];
	}
	float afterfilter[9000];
	const int order = 6; // 6th order (=2 biquads)
	Iir::Butterworth::BandPass<order> butter2;
	const float samplingrate = 50; // Hz
	const float center_frequency = 0.4; // Hz  0.7-10
	const float width_frequency = 0.3; // Hz
	butter2.setup(samplingrate, center_frequency, width_frequency);
	for (int i = 0; i < 9000; i++)  ////共9000个数据
	{
		afterfilter[i] = -butter2.filter(bcg_buf[i]);
	}
	float *rrsave = NULL; rrsave = (float*)malloc(10000 * 1 * sizeof(float));
	float *rrinterval = NULL; rrinterval = (float*)malloc(10000 * 1 * sizeof(float));
	int rrcounts[1];
	findPeaks(afterfilter, 9000, rrsave, rrcounts);
	for (int j = 0; j < rrcounts[0] - 1; j++)
	{
		rrinterval[j] = ((rrsave[j + 1] - rrsave[j]) * 1000) / SampFrequency;    // 求jj间隔序列，单位为ms
	}
	float median_rrinterval;
	int abnormal_pos = 0;
	median_rrinterval = EvaluateMedian(rrinterval, rrcounts[0] - 1);
	for (int j = rrcounts[0] - 2; j >= 0; j--)
	{
		abnormal_pos = 0;
		if ((abs(median_rrinterval - rrinterval[j]) / median_rrinterval) > 1)
		{
			/*
			if (rrsave[j + 1] > 8500)
			{
				abnormal_pos = (9000 - rrsave[j + 1]) / 50;
				break;
			}
			*/
			/* 上边是算10秒内的具体异常位置，下面是算每1秒是否出现异常，选择其中一种方式*/
			if (rrsave[j + 1] > 8950)
			{
				abnormal_pos = 1;
				break;
			}
		}
		else
			abnormal_pos = 0;
	}
	free(rrsave);
	free(rrinterval);

	return abnormal_pos;
}

//===================================================


int main()
{
	HANDLE handel = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)init_Dataread, NULL, 1, 0);
	time_t t1 = time(NULL);
	Sleep(120000);
	//time_t t2 = time(NULL);
	while (TRUE)
	{
		//int label = ReturnLabel((INT64)t1, ((INT64)t1 + 60));
		int Hrnow = Return_Hr();
		int abnormal_rr = 0;
		abnormal_rr = detect_rr_abnormal();
		cout << "hr" << Hrnow << endl;
		cout <<"呼吸是否异常"<< abnormal_rr << endl;
		Sleep(1000);
	}
	//printf("%d\n", label);
	//int abnormal_rr;
	//while (TRUE)
	//{
	//	Sleep(1000);
	//	abnormal_rr = detect_rr_abnormal();
	//	cout << abnormal_rr << endl;
	//}
	//int Hr_rightnow = Return_Hr();
	_CrtDumpMemoryLeaks();
	return 0;
}

