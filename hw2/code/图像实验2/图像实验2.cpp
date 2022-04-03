#include"hxlbmpfile.h"
#include<iostream>
#include<fstream>
#include<math.h>
using namespace std;
#include"myfunctions.h"
#define _CRT_SECURE_NO_WARNINGS 1

int main(int argc, char* argv[])
{
	HXLBMPFILE origionfile;
	if (!origionfile.LoadBMPFile("./input/test.bmp"))
	{
		cout << "cannot load the picture!" << endl;
		exit(1);
	}
	HXLBMPFILE outbmp;
	outbmp.iImageh = origionfile.iImageh;
	outbmp.iImagew = origionfile.iImagew;
	outbmp.iYRGBnum = origionfile.iYRGBnum;
	if (outbmp.iYRGBnum == 1)
	{
		*outbmp.rgbPalette = *origionfile.rgbPalette;
	}
	if (!outbmp.IspImageDataOk())return 1;
	
	float *A = (float*)malloc(256 * sizeof(float));//用一个辅助数组接收Is直方图
	//3.图像Is的直方图（原图像直方图）
	A=gray_histogram(&origionfile, "./output/Hs.txt");

	//4.存原图Is均衡化后图像Ise、5.存原图Is的均衡化变换函数Tse
	hist_equalize_outpic(origionfile,outbmp,A,"./output/Ise");


	
	//1.输出目标图像Id
	float *B= (float*)malloc(256 * sizeof(float));//用一个辅助数组接收目标图像的变换函数
	B = get_Id();
	out_put_targetpic(origionfile,outbmp,A);


	
	//8.输出目标图像Id的直方图Hd
	float *C = (float*)malloc(256 * sizeof(float));//用一个辅助数组接收Id直方图
	C =gray_histogram(&outbmp, "./output/Hd.txt");

	//7.目标图像Id的均衡化变换函数Tde
	hist_equalize_outfunc(outbmp, C, "./output/Tde.txt");

	//6.Ise的直方图Hse
    HXLBMPFILE Isepic;
	if (!Isepic.LoadBMPFile("./output/Ise.bmp"))exit(1);
	float *D = (float*)malloc(256 * sizeof(float));//用一个辅助数组接收Ise直方图
	D=gray_histogram(&Isepic, "./output/Hse.txt");

	//2.直方图规定化增强的变换函数Td
	HistSpecificate(origionfile,A,C,"./output/Td.txt");

	
	return 0;

}