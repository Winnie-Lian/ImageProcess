#include"hxlbmpfile.h"
#include"myfunctions.h"
using namespace std;


//数组数据文本生成函数
template<typename T>
bool write_array_in_txt(T &A, const char *filename)//读数组进到txt文件中,filename是文件名，A为数组名
{
	ofstream outf;
	outf.open(filename, ios::out);
	if (!outf.is_open())//打不开文件
	{
		cout << "the file can not open" << endl;
		outf.close();
		return 0;
	}

	for (int i = 0; i < (sizeof(A) / sizeof(A[0])); i++)
	{
		outf << A[i] << endl;
	}
	outf.close();
	return 1;
}

template<typename T>

bool write_array_in_txt(string H, const char* cFilename)	//针对字符数组的输出函数
{
	ofstream outfile;
	outfile.open(cFilename, ios::out);
	if (!outfile.is_open())
	{
		cout << "Open file failure" << endl;
		outfile.close();
		return false;
	}

	for (int i = 0; H[i] != '\0'; i++)	//遍历数组写入txt
	{
		outfile << H[i];
	}
	outfile.close();
	return true;
}


//统计图像直方图的函数
float* gray_histogram(HXLBMPFILE *bmp,const char* filename)
{
	
	float A[256] = { 0 };//初始化一个数组用于存储灰度
	float* pichistogram = (float*)malloc(256 * sizeof(float));//存txt的一个数组
	int sum = bmp->iImageh*bmp->iImagew;//像素点个数
	/*for (int k = 0; k < 256; k++)
	{
		A[k] = 0;
		for (int i = 0; i < pic.iImageh; i++)
			for (int j = 0; j < pic.iImagew; j++)
			{
				if (pic.pDataAt(i)[j] == k)
					A[k] += 1.0;
			}
	}//统计灰度阶，pDataAt（int h）函数定位到第i行首元素位置
	*/
	for(int i=0;i<bmp->iImageh;i++)
		for (int j = 0; j < bmp->iImagew; j++)
		{
			A[bmp->pDataAt(i)[j]] +=1.0;
		}//另一种统计灰度阶的写法
	

	for (int i = 0; i < 256; i++)
	{
		A[i] /= (float)sum;
		pichistogram[i] = A[i];
	}

	if (!write_array_in_txt(A, filename))//图像的直方图写入txt文件
	{
		cout << "can not out put gray histogram" << endl;
		exit(0);
	}

	return pichistogram;
}

//存均衡化后图像，存取均衡化变换函数
float* hist_equalize_outpic(HXLBMPFILE &pic, HXLBMPFILE &pic2, float *picturehg,const char *filename)
{
	//picturebg数组已经记录了原图像的各灰阶个数了，到时候直接调用上面一个函数就行
	float *outcome = (float*)malloc(256 * sizeof(float));
	float A[256] = { 0 };
	float equal[256] = { 0 };
	float origiondata[256] = { 0 };
	float total = pic.iImageh*pic.iImagew;
	for (int i = 0; i < 256; i++)
	{
		outcome[i] = equal[i];
	}//初始化outcome

	for (int i = 0; i < 256; i++)
	{
		origiondata[i] = picturehg[i] * total;
	}//原图像灰度级计数

	for (int i = 0; i < 256; i++)
		for (int j = 0; j <= i; j++)
		{
			A[i] += picturehg[i];
		}//求原图的灰度累计分布频率


	for (int i = 0; i < 256; i++)
	{
		A[i] = int(255 * picturehg[i] + 0.5);//利用tk=int((L-1)*tk+0.5)将tk拓展范围
		outcome[i] = A[i];
	}
	//A[i]就是均衡化变化函数
	if (!write_array_in_txt(outcome, "./output/Tse.txt"))
	{
		cout << "can not out put the Tse" << endl;
		exit(0);
	}

	for (int i = 0; i < pic.iImageh; i++)
		for (int j = 0; j < pic.iImagew; j++)
		{
			pic2.pDataAt(i)[j] = A[pic.pDataAt(i)[j]];	//将新的值赋予给输出图像
		}

	pic2.SaveBMPFile(filename);
	return outcome;
}

//存图像均衡化数组
float* hist_equalize_histarray(HXLBMPFILE &pic, float *picturehg)
{
	//picturebg数组已经记录了原图像的各灰阶个数了，到时候直接调用上面一个函数就行
	float *outcome = (float*)malloc(256 * sizeof(float));
	float A[256] = { 0 };
	float equal[256] = { 0 };
	float origiondata[256] = { 0 };
	float total = pic.iImageh*pic.iImagew;
	for (int i = 0; i < 256; i++)
	{
		outcome[i] = equal[i];
	}//初始化outcome

	for (int i = 0; i < 256; i++)
	{
		origiondata[i] = picturehg[i] * total;
	}//原图像灰度级计数

	for (int i = 0; i < 256; i++)
		for (int j = 0; j <= i; j++)
		{
			A[i] += picturehg[i];
			outcome[i] = A[i];
		}//求原图的灰度累计分布频率

	return outcome;
}

//存目标图像的均衡化变换函数Tde
float* hist_equalize_outfunc(HXLBMPFILE &pic, float *picturehg, const char *filename)
{
	//picturebg数组已经记录了原图像的各灰阶个数了，到时候直接调用上面一个函数就行
	float *outcome = (float*)malloc(256 * sizeof(float));
	float A[256] = { 0 };
	float equal[256] = { 0 };
	float origiondata[256] = { 0 };
	float total = pic.iImageh*pic.iImagew;
	for (int i = 0; i < 256; i++)
	{
		outcome[i] = equal[i];
	}//初始化outcome

	for (int i = 0; i < 256; i++)
	{
		origiondata[i] = picturehg[i] * total;
	}//原图像灰度级计数

	for (int i = 0; i < 256; i++)
		for (int j = 0; j <= i; j++)
		{
			A[i] += picturehg[i];
		}//求原图的灰度累计分布频率


	for (int i = 0; i < 256; i++)
	{
		A[i] = int(255 * picturehg[i] + 0.5);//利用tk=int((L-1)*tk+0.5)将tk拓展范围
		outcome[i] = A[i];
	}
	//A[i]就是均衡化变化函数
	if (!write_array_in_txt(outcome, filename))
	{
		cout << "can not out put the Tde" << endl;
		exit(0);
	}
	return outcome;
}

//存取目标变换函数
float* get_Id()
{
	float a = 6;//组员手算的
	float x[256];
	float y[256];
	float *outcome = (float*)malloc(256 * sizeof(float));
	for (int i = 0; i < 256; i++)
	{
		x[i] = i * (float)(1 / 256);//利用缩放的思想将其划分为256份
	}
	for (int i = 0; i < 256; i++)
	{
		y[i] = 6 * (0.25 - pow((x[i] - 0.5), 2));
		y[i] /= 256;
		outcome[i] = y[i];
	}

	/*if (!write_array_in_txt(outcome, "C:/Users/刘英吉/source/repos/图像2/图像2/Id_change_function.txt"))
	{
		cout << "can not out put the Id_change_function" << endl;
		exit(0);
	}//存下目标变换函数*/

	return outcome;
}

//输出目标图像(图像变化函数)
float* out_put_targetpic(HXLBMPFILE pic, HXLBMPFILE pic2, float *change)
{
	change = get_Id();
	
	for(int i=0;i<pic.iImageh;i++)
		for (int j = 0; j < pic.iImagew; j++)
		{
			pic2.pDataAt(i)[j] = change[pic.pDataAt(i)[j]];//用上面的变换函数
		}

	pic2.SaveBMPFile("./output/Id.bmp");
	return 0;
}

//直方图规定化，存取规定化增强变化函数
int* HistSpecificate(HXLBMPFILE& src,float *yuantu,float *target,const char* cFilename)	//直方图规定化
{
	int i, j, index = 0;
	float* SQH_tk = hist_equalize_histarray(src,yuantu);	//获取原图像的均衡化tk数组
	float* EQH_tk = hist_equalize_histarray(src,target);//获取目标图像的均衡化tk数组
	int T[256];	//灰阶对应关系矩阵
	int* result = (int*)malloc(256 * sizeof(int));
	float min, temp;
	for (i = 0; i < 256; i++)
	{
		min = fabs(SQH_tk[i] - EQH_tk[0]);	//假设最小值
		for (j = index; j < 256; j++)	//由于两个数组都是递增的，所以我们每次从上一次的结果继续即可
		{
			temp = fabs(SQH_tk[i] - EQH_tk[j]);	//依次做差找到最小值
			if (temp < min)
			{
				min = temp;
				index = j;	//找到最小值的下标
			}
			if (SQH_tk[i] < EQH_tk[j])
				break;
		}
		T[i] = index;	//保存映射结果
		result[i] = index;
	}
	if (!write_array_in_txt(T, "./output/Td.txt"))
	{
		printf("error!can not out put Td\n");
		exit(0);
	}//存取规定化增强的变换函数Td
	return result;
}