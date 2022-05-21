#pragma once
#include"hxlbmpfile.h"
#include <stdio.h>
#include <math.h>
#include <Windows.h>
using namespace std;

#define LENTH 256

class arithmetic_coding
{
	unsigned int high=0xFFFFFFFFU;
	unsigned int low=0;
	double freq_table[LENTH];
	double range_table[LENTH];
	int pending_bits=0;
public:
	int arrithmetic_coding();
	void single_encoding(int data);
	void Encoding(hxlbmpfile bmp);
	void gen_freq_table(hxlbmpfile bmp);
	void set_high(unsigned int);
	void set_low(unsigned int);
	void set_range_table(int range);
	void output_bit_plus_pending(int);
	int get_pendng_bits();




};

