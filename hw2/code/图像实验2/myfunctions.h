#pragma once
#include"hxlbmpfile.h"
#include"stdio.h"
#include"math.h"
#include"windows.h"
#include<iostream>
#include<fstream>
using namespace std;

template<typename T>
bool write_array_in_txt(T &A, const char *filename);
float*  gray_histogram(HXLBMPFILE *bmp, const char* filename);
float* hist_equalize_outpic(HXLBMPFILE &pic, HXLBMPFILE &pic2, float *picturehg,const char *filename);
float* hist_equalize_outfunc(HXLBMPFILE &pic, float *picturehg, const char *filename);
float* hist_equalize_histarray(HXLBMPFILE &pic, float *picturehg);
float* get_Id();
float* out_put_targetpic(HXLBMPFILE pic, HXLBMPFILE pic2, float *change);
int* HistSpecificate(HXLBMPFILE& src, float *yuantu, float *target, const char* cFilename);