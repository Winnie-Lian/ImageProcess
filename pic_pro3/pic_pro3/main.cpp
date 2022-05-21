#include "hxlbmpfile.h"
#include"arithmetic_coding.h"
#include <iostream>


int main() {
	hxlbmpfile b1;
	b1.LoadBMPFile("./b8gray.bmp");
	//long count=0;
	//for (int i = 0; i < b1.iImageh; i++) {
	//	for (int j = 0; j < b1.iImagew; j++) {
	//		printf("%d\n",*b1.pDataAt(i, 0));
	//		count++;
	//	}
	//}
	//printf("%d", count);
	arithmetic_coding bmp_compress;
	bmp_compress.gen_freq_table(b1);
	bmp_compress.Encoding(b1);
}