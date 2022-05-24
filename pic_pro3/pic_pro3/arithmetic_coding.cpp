#include "arithmetic_coding.h"
#include<iostream>
#define LENTH 256	//实验对象为8bit灰度图像，记录freq_table等长度均为2^8=256

void arithmetic_coding::arrithmetic_coding() {}


void arithmetic_coding::disp_freq_table() {
	for (int i = 0; i < LENTH; i++) {
		printf("%f", freq_table[i]);
	}
}

void arithmetic_coding::disp_range_table() {
	for (int i = 0; i < LENTH; i++) {
		printf("%f", range_table[i]);
	}
}


/*
	对于range_table而言，仅存储range上限
	每个字段的range逻辑上严格左闭右开
	每次编码后更新range_table
*/
void arithmetic_coding::set_range_table(int range) {
	// printf("set_range_table called");
	int flag = 0;	// 记录上一个存在的灰度值
	
	// 如果freq_table中记录该灰度值频率为-1，即存在该灰度值的像素点，则忽略
	for (int i = 0; i < LENTH; i++) {
		if (freq_table[i] != -1) {
			if (i == 0) {
				range_table[i] = low + range * freq_table[i];
			}
			else {
				range_table[i] = range_table[flag] + range * freq_table[i];
			}
			flag = i;
		}
		else continue;
	}
	// disp_range_table();
}

/*
	初始化频率表
	遍历所有像素值，存储于count_table
	计算频率存储于私有对象freq_table
*/
void arithmetic_coding::gen_freq_table(hxlbmpfile bmp) {
	int count=0;
	double* count_table ;
	count_table = new double [LENTH];

	for (int i = 0; i < LENTH; i++) {
		count_table[i] = 0;
	}

	// 遍历data
	for (int i = 0; i < bmp.iImageh; i++) {
		for (int j = 0; j < bmp.iImagew; j++) {
			// printf("%0x\n", *bmp.pDataAt(i, 0));
			count_table[*bmp.pDataAt(i, 0)]++;
			count++;
		}
	}

	for (int i = 0; i < 256; i++) {
		// 如果没有该灰度值记为-1
		if (count_table[i] == 0) {
			freq_table[i] = -1;
		}
		else freq_table[i] = count_table[i] / count ;
	}

	//disp_freq_table();

}

// 设置（更新）区间下限
void arithmetic_coding::set_low(unsigned int i) {
	low = i;
}


// 设置（更新）区间上限
void arithmetic_coding::set_high(unsigned int i) {
	high = i;
}

/*
对单一像素值编码
通过运算得到区间后，更新区间表格
*/
void arithmetic_coding::single_encoding(int data) {
	printf("single_encoding called\n");
	int range = high - low + 1;
	set_range_table(range);
	
	set_low(range_table[0] - range * freq_table[0]);
	printf("high:%f    low:%f\n\n", high,low);
}

/*
	遍历像素值，完成对图像的编码
	对于每个像素值，调用single_encoding

	对于小数上溢问题的解决：
		使用浮点数的二进制表示
		因对象为int类型，因此也只能使用32位，所以仍存在溢出的可能
		因此对每次编码后进行检查，如区间上下限高位有相同比特即可将该位数值进行输出
		即左移相同位数
*/
void arithmetic_coding::Encoding(hxlbmpfile bmp) {
	for (int i = 0; i < bmp.iImageh; i++) {
		for (int j = 0; j < bmp.iImagew; j++) {
			// printf("%0x\n", *bmp.pDataAt(i, 0));
			single_encoding(*bmp.pDataAt(i, 0));
			for (; ; ) {
				if (high < 0x80000000U) {
					output_bit_plus_pending(0);
					low <<= 1;
					high <<= 1;
					high |= 1;
				}
				else if (low >= 0x80000000U) {
					output_bit_plus_pending(1);
					low <<= 1;
					high <<= 1;
					high |= 1;
				}
				else if (low >= 0x40000000 && high < 0xC0000000U) {
					pending_bits++;
					low <<= 1;
					low &= 0x7FFFFFFF;
					high <<= 1;
					high |= 0x80000001;
				}
				else {
					break;
				}
			}
		}
	}

}


void arithmetic_coding::output_bit_plus_pending(int output)
{
	printf("output called");
	FILE* f = fopen("./output.txt", "a");
	fprintf(f, "%d", output);
}

int arithmetic_coding::get_pendng_bits() {
	return pending_bits;
}


