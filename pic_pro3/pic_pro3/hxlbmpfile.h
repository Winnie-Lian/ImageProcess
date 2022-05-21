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
	int iImagew, iImageh;//ͼƬ��Ⱥ͸߶�
	int iYRGBnum;//1���Ҷȣ�3����ɫ 
	RGBQUAD rgbPalette[256];//��ɫ��
	BYTE* pDataAt(int h, int Y0R0G1B2 = 0);//ָ��ͼ���h�е�λ�ã�YR0G1B2��ʾ�Ҷ�=0��R=0,G=1,B=2
	BOOL IspImageDataOk();//Ϊͼ������ڴ�
	BOOL LoadBMPFile(const char* fname);//��Ӳ�̼���ͼ��
	BOOL SaveBMPFile(const char* fname);//��ͼ�񱣴���Ӳ��
	//void get_data(FILE* f);
	hxlbmpfile();//�����ɫ���ֵ
	~hxlbmpfile();
	
};

