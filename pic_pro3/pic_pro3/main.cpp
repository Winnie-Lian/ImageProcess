#include "hxlbmpfile.h"
#include"arithmetic_coding.h"
#include <iostream>


int main() {
	hxlbmpfile b1;
	b1.LoadBMPFile("./b8gray.bmp");

	// 对导入的bmp文件进行算术编码
	// 创建一个arithmetic_coding类型的变量
	arithmetic_coding bmp_compress;
	// 构建该变量数据的频率表：data-frequency
	bmp_compress.gen_freq_table(b1);
	// 算术编码
	bmp_compress.Encoding(b1);
}