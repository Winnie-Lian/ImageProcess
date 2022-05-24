#pragma once
#include"hxlbmpfile.h"
#include <stdio.h>
#include <math.h>
#include <Windows.h>
using namespace std;

#define LENTH 256

class arithmetic_coding
{
	
	unsigned int high=0xFFFFFFFFU;		// 每一次编码后的区间上限，初始化为1
	unsigned int low=0;		// 每一次编码后的区间下限，初始化为0
	double freq_table[LENTH];		// 存储对象数据频率
	double range_table[LENTH];		// 存储编码时的区间表格
	int pending_bits=0;		// 追加的数值
public:
	void arrithmetic_coding();
	void single_encoding(int data);		// 对每个像素进行编码
	void Encoding(hxlbmpfile bmp);		// 遍历所有像素值，完成图片编码
	void gen_freq_table(hxlbmpfile bmp);	// 初始化freq_table
	void set_high(unsigned int);		// 设置（更新）区间上限
	void set_low(unsigned int);		// 设置（更新） 区间下限
	void set_range_table(int range);		// 设置（更新）range_table
	void output_bit_plus_pending(int);		// 输出二进制数值
	int get_pendng_bits();		// 追加二进制数值
	void disp_freq_table();
	void disp_range_table();




};

