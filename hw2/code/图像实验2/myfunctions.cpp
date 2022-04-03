#include"hxlbmpfile.h"
#include"myfunctions.h"
using namespace std;


//���������ı����ɺ���
template<typename T>
bool write_array_in_txt(T &A, const char *filename)//���������txt�ļ���,filename���ļ�����AΪ������
{
	ofstream outf;
	outf.open(filename, ios::out);
	if (!outf.is_open())//�򲻿��ļ�
	{
		cout << "the file can not open" << endl;
		outf.close();
		return 0;
	}

	for (int i = 0; i < (sizeof(A) / sizeof(A[0])); i++)
	{
		outf << A[i] << endl;
	}
	outf.close();
	return 1;
}

template<typename T>

bool write_array_in_txt(string H, const char* cFilename)	//����ַ�������������
{
	ofstream outfile;
	outfile.open(cFilename, ios::out);
	if (!outfile.is_open())
	{
		cout << "Open file failure" << endl;
		outfile.close();
		return false;
	}

	for (int i = 0; H[i] != '\0'; i++)	//��������д��txt
	{
		outfile << H[i];
	}
	outfile.close();
	return true;
}


//ͳ��ͼ��ֱ��ͼ�ĺ���
float* gray_histogram(HXLBMPFILE *bmp,const char* filename)
{
	
	float A[256] = { 0 };//��ʼ��һ���������ڴ洢�Ҷ�
	float* pichistogram = (float*)malloc(256 * sizeof(float));//��txt��һ������
	int sum = bmp->iImageh*bmp->iImagew;//���ص����
	/*for (int k = 0; k < 256; k++)
	{
		A[k] = 0;
		for (int i = 0; i < pic.iImageh; i++)
			for (int j = 0; j < pic.iImagew; j++)
			{
				if (pic.pDataAt(i)[j] == k)
					A[k] += 1.0;
			}
	}//ͳ�ƻҶȽף�pDataAt��int h��������λ����i����Ԫ��λ��
	*/
	for(int i=0;i<bmp->iImageh;i++)
		for (int j = 0; j < bmp->iImagew; j++)
		{
			A[bmp->pDataAt(i)[j]] +=1.0;
		}//��һ��ͳ�ƻҶȽ׵�д��
	

	for (int i = 0; i < 256; i++)
	{
		A[i] /= (float)sum;
		pichistogram[i] = A[i];
	}

	if (!write_array_in_txt(A, filename))//ͼ���ֱ��ͼд��txt�ļ�
	{
		cout << "can not out put gray histogram" << endl;
		exit(0);
	}

	return pichistogram;
}

//����⻯��ͼ�񣬴�ȡ���⻯�任����
float* hist_equalize_outpic(HXLBMPFILE &pic, HXLBMPFILE &pic2, float *picturehg,const char *filename)
{
	//picturebg�����Ѿ���¼��ԭͼ��ĸ��ҽ׸����ˣ���ʱ��ֱ�ӵ�������һ����������
	float *outcome = (float*)malloc(256 * sizeof(float));
	float A[256] = { 0 };
	float equal[256] = { 0 };
	float origiondata[256] = { 0 };
	float total = pic.iImageh*pic.iImagew;
	for (int i = 0; i < 256; i++)
	{
		outcome[i] = equal[i];
	}//��ʼ��outcome

	for (int i = 0; i < 256; i++)
	{
		origiondata[i] = picturehg[i] * total;
	}//ԭͼ��Ҷȼ�����

	for (int i = 0; i < 256; i++)
		for (int j = 0; j <= i; j++)
		{
			A[i] += picturehg[i];
		}//��ԭͼ�ĻҶ��ۼƷֲ�Ƶ��


	for (int i = 0; i < 256; i++)
	{
		A[i] = int(255 * picturehg[i] + 0.5);//����tk=int((L-1)*tk+0.5)��tk��չ��Χ
		outcome[i] = A[i];
	}
	//A[i]���Ǿ��⻯�仯����
	if (!write_array_in_txt(outcome, "./output/Tse.txt"))
	{
		cout << "can not out put the Tse" << endl;
		exit(0);
	}

	for (int i = 0; i < pic.iImageh; i++)
		for (int j = 0; j < pic.iImagew; j++)
		{
			pic2.pDataAt(i)[j] = A[pic.pDataAt(i)[j]];	//���µ�ֵ��������ͼ��
		}

	pic2.SaveBMPFile(filename);
	return outcome;
}

//��ͼ����⻯����
float* hist_equalize_histarray(HXLBMPFILE &pic, float *picturehg)
{
	//picturebg�����Ѿ���¼��ԭͼ��ĸ��ҽ׸����ˣ���ʱ��ֱ�ӵ�������һ����������
	float *outcome = (float*)malloc(256 * sizeof(float));
	float A[256] = { 0 };
	float equal[256] = { 0 };
	float origiondata[256] = { 0 };
	float total = pic.iImageh*pic.iImagew;
	for (int i = 0; i < 256; i++)
	{
		outcome[i] = equal[i];
	}//��ʼ��outcome

	for (int i = 0; i < 256; i++)
	{
		origiondata[i] = picturehg[i] * total;
	}//ԭͼ��Ҷȼ�����

	for (int i = 0; i < 256; i++)
		for (int j = 0; j <= i; j++)
		{
			A[i] += picturehg[i];
			outcome[i] = A[i];
		}//��ԭͼ�ĻҶ��ۼƷֲ�Ƶ��

	return outcome;
}

//��Ŀ��ͼ��ľ��⻯�任����Tde
float* hist_equalize_outfunc(HXLBMPFILE &pic, float *picturehg, const char *filename)
{
	//picturebg�����Ѿ���¼��ԭͼ��ĸ��ҽ׸����ˣ���ʱ��ֱ�ӵ�������һ����������
	float *outcome = (float*)malloc(256 * sizeof(float));
	float A[256] = { 0 };
	float equal[256] = { 0 };
	float origiondata[256] = { 0 };
	float total = pic.iImageh*pic.iImagew;
	for (int i = 0; i < 256; i++)
	{
		outcome[i] = equal[i];
	}//��ʼ��outcome

	for (int i = 0; i < 256; i++)
	{
		origiondata[i] = picturehg[i] * total;
	}//ԭͼ��Ҷȼ�����

	for (int i = 0; i < 256; i++)
		for (int j = 0; j <= i; j++)
		{
			A[i] += picturehg[i];
		}//��ԭͼ�ĻҶ��ۼƷֲ�Ƶ��


	for (int i = 0; i < 256; i++)
	{
		A[i] = int(255 * picturehg[i] + 0.5);//����tk=int((L-1)*tk+0.5)��tk��չ��Χ
		outcome[i] = A[i];
	}
	//A[i]���Ǿ��⻯�仯����
	if (!write_array_in_txt(outcome, filename))
	{
		cout << "can not out put the Tde" << endl;
		exit(0);
	}
	return outcome;
}

//��ȡĿ��任����
float* get_Id()
{
	float a = 6;//��Ա�����
	float x[256];
	float y[256];
	float *outcome = (float*)malloc(256 * sizeof(float));
	for (int i = 0; i < 256; i++)
	{
		x[i] = i * (float)(1 / 256);//�������ŵ�˼�뽫�仮��Ϊ256��
	}
	for (int i = 0; i < 256; i++)
	{
		y[i] = 6 * (0.25 - pow((x[i] - 0.5), 2));
		y[i] /= 256;
		outcome[i] = y[i];
	}

	/*if (!write_array_in_txt(outcome, "C:/Users/��Ӣ��/source/repos/ͼ��2/ͼ��2/Id_change_function.txt"))
	{
		cout << "can not out put the Id_change_function" << endl;
		exit(0);
	}//����Ŀ��任����*/

	return outcome;
}

//���Ŀ��ͼ��(ͼ��仯����)
float* out_put_targetpic(HXLBMPFILE pic, HXLBMPFILE pic2, float *change)
{
	change = get_Id();
	
	for(int i=0;i<pic.iImageh;i++)
		for (int j = 0; j < pic.iImagew; j++)
		{
			pic2.pDataAt(i)[j] = change[pic.pDataAt(i)[j]];//������ı任����
		}

	pic2.SaveBMPFile("./output/Id.bmp");
	return 0;
}

//ֱ��ͼ�涨������ȡ�涨����ǿ�仯����
int* HistSpecificate(HXLBMPFILE& src,float *yuantu,float *target,const char* cFilename)	//ֱ��ͼ�涨��
{
	int i, j, index = 0;
	float* SQH_tk = hist_equalize_histarray(src,yuantu);	//��ȡԭͼ��ľ��⻯tk����
	float* EQH_tk = hist_equalize_histarray(src,target);//��ȡĿ��ͼ��ľ��⻯tk����
	int T[256];	//�ҽ׶�Ӧ��ϵ����
	int* result = (int*)malloc(256 * sizeof(int));
	float min, temp;
	for (i = 0; i < 256; i++)
	{
		min = fabs(SQH_tk[i] - EQH_tk[0]);	//������Сֵ
		for (j = index; j < 256; j++)	//�����������鶼�ǵ����ģ���������ÿ�δ���һ�εĽ����������
		{
			temp = fabs(SQH_tk[i] - EQH_tk[j]);	//���������ҵ���Сֵ
			if (temp < min)
			{
				min = temp;
				index = j;	//�ҵ���Сֵ���±�
			}
			if (SQH_tk[i] < EQH_tk[j])
				break;
		}
		T[i] = index;	//����ӳ����
		result[i] = index;
	}
	if (!write_array_in_txt(T, "./output/Td.txt"))
	{
		printf("error!can not out put Td\n");
		exit(0);
	}//��ȡ�涨����ǿ�ı任����Td
	return result;
}