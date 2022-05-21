#pragma once
#include "stdio.h"
#include "windows.h"
class hxlbmpfile
{
	BYTE* pImageData;
	BOOL IsBMPFile(FILE* f);
	int GetBytes1Line();
	BOOL GetPara(FILE* f);
	BOOL Get8BMPData(FILE* f);
	BOOL Get24BMPData(FILE* f);
	void SaveFileHeader(FILE* f);
	void SaveInfoHeader(FILE* f);
	BOOL Save8BMPData(FILE* f);
	BOOL Save24BMPData(FILE* f);
public:
	int iImagew, iImageh;//图片宽度和高度
	int iYRGBnum;//1：灰度，3：彩色 
	RGBQUAD rgbPalette[256];//调色板
	BYTE* pDataAt(int h, int Y0R0G1B2 = 0);//指向图像第h行的位置，YR0G1B2表示灰度=0，R=0,G=1,B=2
	BOOL IspImageDataOk();//为图像分配内存
	BOOL LoadBMPFile(const char* fname);//从硬盘加载图像
	BOOL SaveBMPFile(const char* fname);//将图像保存至硬盘
	//void get_data(FILE* f);
	hxlbmpfile();//赋予调色板初值
	~hxlbmpfile();
	
};

