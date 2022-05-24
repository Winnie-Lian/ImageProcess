#pragma once
#include"hxlbmpfile.h"
#include <stdio.h>
#include <math.h>
#include <Windows.h>
using namespace std;

#define LENTH 256

class arithmetic_coding
{
	
	unsigned int high=0xFFFFFFFFU;		// ÿһ�α������������ޣ���ʼ��Ϊ1
	unsigned int low=0;		// ÿһ�α������������ޣ���ʼ��Ϊ0
	double freq_table[LENTH];		// �洢��������Ƶ��
	double range_table[LENTH];		// �洢����ʱ��������
	int pending_bits=0;		// ׷�ӵ���ֵ
public:
	void arrithmetic_coding();
	void single_encoding(int data);		// ��ÿ�����ؽ��б���
	void Encoding(hxlbmpfile bmp);		// ������������ֵ�����ͼƬ����
	void gen_freq_table(hxlbmpfile bmp);	// ��ʼ��freq_table
	void set_high(unsigned int);		// ���ã����£���������
	void set_low(unsigned int);		// ���ã����£� ��������
	void set_range_table(int range);		// ���ã����£�range_table
	void output_bit_plus_pending(int);		// �����������ֵ
	int get_pendng_bits();		// ׷�Ӷ�������ֵ
	void disp_freq_table();
	void disp_range_table();




};

