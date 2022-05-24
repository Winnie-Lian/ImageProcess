#include "arithmetic_coding.h"
#include<iostream>
#define LENTH 256	//ʵ�����Ϊ8bit�Ҷ�ͼ�񣬼�¼freq_table�ȳ��Ⱦ�Ϊ2^8=256

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
	����range_table���ԣ����洢range����
	ÿ���ֶε�range�߼����ϸ�����ҿ�
	ÿ�α�������range_table
*/
void arithmetic_coding::set_range_table(int range) {
	// printf("set_range_table called");
	int flag = 0;	// ��¼��һ�����ڵĻҶ�ֵ
	
	// ���freq_table�м�¼�ûҶ�ֵƵ��Ϊ-1�������ڸûҶ�ֵ�����ص㣬�����
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
	��ʼ��Ƶ�ʱ�
	������������ֵ���洢��count_table
	����Ƶ�ʴ洢��˽�ж���freq_table
*/
void arithmetic_coding::gen_freq_table(hxlbmpfile bmp) {
	int count=0;
	double* count_table ;
	count_table = new double [LENTH];

	for (int i = 0; i < LENTH; i++) {
		count_table[i] = 0;
	}

	// ����data
	for (int i = 0; i < bmp.iImageh; i++) {
		for (int j = 0; j < bmp.iImagew; j++) {
			// printf("%0x\n", *bmp.pDataAt(i, 0));
			count_table[*bmp.pDataAt(i, 0)]++;
			count++;
		}
	}

	for (int i = 0; i < 256; i++) {
		// ���û�иûҶ�ֵ��Ϊ-1
		if (count_table[i] == 0) {
			freq_table[i] = -1;
		}
		else freq_table[i] = count_table[i] / count ;
	}

	//disp_freq_table();

}

// ���ã����£���������
void arithmetic_coding::set_low(unsigned int i) {
	low = i;
}


// ���ã����£���������
void arithmetic_coding::set_high(unsigned int i) {
	high = i;
}

/*
�Ե�һ����ֵ����
ͨ������õ�����󣬸���������
*/
void arithmetic_coding::single_encoding(int data) {
	printf("single_encoding called\n");
	int range = high - low + 1;
	set_range_table(range);
	
	set_low(range_table[0] - range * freq_table[0]);
	printf("high:%f    low:%f\n\n", high,low);
}

/*
	��������ֵ����ɶ�ͼ��ı���
	����ÿ������ֵ������single_encoding

	����С����������Ľ����
		ʹ�ø������Ķ����Ʊ�ʾ
		�����Ϊint���ͣ����Ҳֻ��ʹ��32λ�������Դ�������Ŀ���
		��˶�ÿ�α������м�飬�����������޸�λ����ͬ���ؼ��ɽ���λ��ֵ�������
		��������ͬλ��
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


