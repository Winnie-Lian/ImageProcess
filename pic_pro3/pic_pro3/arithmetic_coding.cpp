#include "arithmetic_coding.h"
#include<iostream>
#define LENTH 256

int arithmetic_coding::arrithmetic_coding() {
	return 0;
}

// 仅存储range上限，每个字段的range逻辑上严格左闭右开
void arithmetic_coding::set_range_table(int range) {
	for (int i = 0; i < LENTH; i++) {
		if (freq_table[i] != -1) {
			if (i == 0) {
				range_table[i] = low + range * freq_table[i];
			}
			else {
				range_table[i] = range_table[i - 1] + range * freq_table[i];
			}
		}
		else continue;
	}
}

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
		//if (i == 0) {
		//	freq_table[i] = count_table[i] / count;
		//}
		//else
		if (count_table[i] == 0) {
			freq_table[i] = -1;
		}
		else freq_table[i] = count_table[i] / count ;
	}


}

void arithmetic_coding::set_low(unsigned int i) {
	low = i;
}

void arithmetic_coding::set_high(unsigned int i) {
	high = i;
}

void arithmetic_coding::single_encoding(int data) {
	int range = high - low + 1;
	set_range_table(range);
	
	set_low(range_table[0] - range * freq_table[0]);

}

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
				//else if (low >= 0x40000000 && high < 0xC0000000U) {
				//	pending_bits++;
				//	low <<= 1;
				//	low &= 0x7FFFFFFF;
				//	high <<= 1;
				//	high |= 0x80000001;
				//}
				else {
					break;
				}
			}
		}
	}

}


void arithmetic_coding::output_bit_plus_pending(int output)
{
	FILE* f = fopen("./output.txt", "a");
	fprintf(f, "%d", output);
}

int arithmetic_coding::get_pendng_bits() {
	return pending_bits;
}