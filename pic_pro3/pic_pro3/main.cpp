#include "hxlbmpfile.h"
#include"arithmetic_coding.h"
#include <iostream>


int main() {
	hxlbmpfile b1;
	b1.LoadBMPFile("./b8gray.bmp");

	// �Ե����bmp�ļ�������������
	// ����һ��arithmetic_coding���͵ı���
	arithmetic_coding bmp_compress;
	// �����ñ������ݵ�Ƶ�ʱ�data-frequency
	bmp_compress.gen_freq_table(b1);
	// ��������
	bmp_compress.Encoding(b1);
}