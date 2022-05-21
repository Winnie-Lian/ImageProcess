#include "hxlbmpfile.h"
#include<iostream>
#include<fstream>
#include<math.h>
using namespace std;

BITMAPINFOHEADER ih;

hxlbmpfile::hxlbmpfile()
{
	pImageData = NULL;
	for (int i = 0; i < 256; i++)
	{
		rgbPalette[i].rgbBlue = rgbPalette[i].rgbGreen = rgbPalette[i].rgbRed = i;
		rgbPalette[i].rgbReserved = 0;
	}
	iYRGBnum = 0;
	iImagew = iImageh = 0;
}

hxlbmpfile::~hxlbmpfile()
{
	if (pImageData) delete[] pImageData;
}

BOOL hxlbmpfile::IsBMPFile(FILE* f)
{
	fseek(f, 0, SEEK_END);
	int flen = ftell(f);
	BITMAPFILEHEADER fh;
	fseek(f, 0, SEEK_SET);
	fread(&fh, sizeof(BITMAPFILEHEADER), 1, f);
	if (fh.bfType != 0x4d42)
	{//不是bmp文件退出
		printf("it's not a bmp file.\n");
		fclose(f);
		return false;
	}
	cout << fh.bfSize << "  it's fh.bfsize" << endl;
	cout << flen << "  it's flen" << endl;
	/*if (fh.bfSize != flen)
	{
		printf("flen wrong\n");
		fclose(f);
		return false;
	}*/
	return true;
}

BOOL hxlbmpfile::IspImageDataOk()
{
	int size = iImagew * iImageh * iYRGBnum + 100;
	if (pImageData) { delete[] pImageData; pImageData = NULL; }
	pImageData = new BYTE[size];
	if (pImageData) memset(pImageData, 0, size);
	return pImageData != NULL;
}//分配空间

int hxlbmpfile::GetBytes1Line()
{
	return (iImagew * iYRGBnum + 3) / 4 * 4;
}

BOOL hxlbmpfile::GetPara(FILE* f)
{
	BITMAPINFOHEADER ih;
	fread(&ih, sizeof(BITMAPINFOHEADER), 1, f);
	if (ih.biBitCount != 8)
	{//只调用8位和24位 
		if (ih.biBitCount != 24) { fclose(f); return FALSE; }
	}
	iYRGBnum = ih.biBitCount / 8;//1 
	iImagew = ih.biWidth;
	iImageh = ih.biHeight;
	fseek(f, 14 + ih.biSize, SEEK_SET);
	if (iYRGBnum == 1) fread(rgbPalette, sizeof(RGBQUAD), 256, f);
	return TRUE;
}

BYTE* hxlbmpfile::pDataAt(int h, int Y0R0G1B2)//指向图像第h行位置，Y0R0G1B2表示灰度=0，R=0,G=1,B=2
{
	int iRGB = Y0R0G1B2 * iImagew;
	if (iYRGBnum <= Y0R0G1B2) iRGB = 0; //?
	return pImageData + h * iImagew + iRGB;//返回首元素地址
}

BOOL hxlbmpfile::Get8BMPData(FILE* f)
{
	int w4b = GetBytes1Line();
	BYTE* ptr = NULL;
	ptr = new BYTE[w4b];
	if (!ptr) return FALSE;
	for (int i = iImageh - 1; i >= 0; i--)
	{
		fread(ptr, w4b, 1, f); memmove(pDataAt(i), ptr, iImagew);
	}
	delete[] ptr;
	return TRUE;
}

BOOL hxlbmpfile::Get24BMPData(FILE* f)
{
	int w4b = GetBytes1Line();
	BYTE* ptr = NULL;
	ptr = new BYTE[w4b];
	if (!ptr) return FALSE;
	for (int i = iImageh - 1; i >= 0; i--)
	{
		fread(ptr, w4b, 1, f);
		for (int j = 0; j < iImagew; j++)
		{
			*(pDataAt(i, 0) + j) = *(ptr + j * 3 + 2);
			*(pDataAt(i, 1) + j) = *(ptr + j * 3 + 1);
			*(pDataAt(i, 2) + j) = *(ptr + j * 3 + 0);
		}
	}
	delete[] ptr;
	return TRUE;
}

BOOL hxlbmpfile::LoadBMPFile(const char* cFname)
{
	if (strlen(cFname) < 5)return FALSE;

	FILE* f = NULL;
	f = fopen(cFname, "r+b");
	if (f == NULL)
	{
		cout << "f is null!" << endl;
		return FALSE;
	}
	if (!IsBMPFile(f))
	{
		cout << "is bmp file wrong" << endl;
		return FALSE;
	}

	if (!GetPara(f))
	{
		fclose(f);
		cout << "get para wrong!" << endl;
		return FALSE;
	}

	if (!IspImageDataOk())
	{
		fclose(f);
		cout << "is pimage data ok wrong" << endl;
		return FALSE;
	}
	BOOL res = FALSE;
	if (iYRGBnum == 1)
		res = Get8BMPData(f);
	else if (iYRGBnum == 3)
		res = Get24BMPData(f);

	fclose(f);
	return res;
}

void hxlbmpfile::SaveFileHeader(FILE* f)
{
	BITMAPFILEHEADER fh;
	memset(&fh, 0, sizeof(BITMAPFILEHEADER));
	fh.bfType = 0x4d42;
	fh.bfOffBits = 14 + 40 + ((iYRGBnum == 1) ? 256 * sizeof(RGBQUAD) : 0);
	fh.bfSize = fh.bfOffBits + iImageh * GetBytes1Line();
	fwrite(&fh, sizeof(BITMAPFILEHEADER), 1, f);
}
void hxlbmpfile::SaveInfoHeader(FILE* f)
{
	//BITMAPINFOHEADER ih;
	memset(&ih, 0, sizeof(BITMAPINFOHEADER));
	ih.biSize = 40;
	ih.biPlanes = 1;
	ih.biWidth = iImagew;
	ih.biHeight = iImageh;
	ih.biBitCount = 8 * iYRGBnum;
	ih.biSizeImage = iImageh * GetBytes1Line();
	fwrite(&ih, sizeof(BITMAPINFOHEADER), 1, f);
	if (iYRGBnum == 1) fwrite(rgbPalette, sizeof(RGBQUAD), 256, f);
}

BOOL hxlbmpfile::Save8BMPData(FILE* f)
{
	//BITMAPINFOHEADER ih;
	int w4b = GetBytes1Line();
	BYTE* ptr = new BYTE[w4b];
	if (ptr == NULL) return FALSE;
	memset(ptr, 0, w4b);
	for (int i = ih.biHeight - 1; i >= 0; i--)
	{
		memmove(ptr, pDataAt(i), ih.biWidth);
		fwrite(ptr, w4b, 1, f);
	}
	delete[] ptr;
	return TRUE;
}

BOOL hxlbmpfile::Save24BMPData(FILE* f)
{
	//BITMAPINFOHEADER ih;
	int w4b = GetBytes1Line();
	BYTE* ptr = new BYTE[w4b];
	if (ptr == NULL) return FALSE;
	memset(ptr, 0, w4b);
	for (int i = ih.biHeight - 1; i >= 0; i--)
	{
		for (int j = 0; j < ih.biWidth; j++)
		{
			*(ptr + j * 3 + 2) = *(pDataAt(i, 0) + j);
			*(ptr + j * 3 + 1) = *(pDataAt(i, 1) + j);
			*(ptr + j * 3 + 0) = *(pDataAt(i, 2) + j);
		}
		fwrite(ptr, w4b, 1, f);
	}
	delete[] ptr;
	return TRUE;
}

BOOL hxlbmpfile::SaveBMPFile(const char* cFname)
{
	if (!pImageData) return FALSE;
	if (strlen(cFname) < 5) return FALSE;
	FILE* f = NULL;
	f = fopen(cFname, "w+b");
	if (f == NULL) return FALSE;
	SaveFileHeader(f);
	SaveInfoHeader(f);
	BOOL res = FALSE;
	if (iYRGBnum == 1)
		res = Save8BMPData(f);
	else if (iYRGBnum == 3)
		res = Save24BMPData(f);
	fclose(f);
	return res;
}

//void get_data(FILE* f) {
//	for(int i=0;i<)
//}