#include<iostream>
#include<fstream>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<iomanip>
#include<string>
#include<Windows.h>
#include<io.h>
#include<stdlib.h>
#pragma warning(disable : 4996)
using namespace std;

//***解决打开文件不同的问题***
static int male_num = 1;
static int female_num = 1;
/*设置文件存放路径*/
string sRoot = "";
string sSuffix_wav = "data\\*.wav";
string sSuffix_txt = "data\\*.TextGrid.txt";
string sPathLast_One = sRoot + sSuffix_wav;
string sPathLast_Two = sRoot + sSuffix_txt;
struct file_name_list
{
	char filename[260];  //.wav  
	char fileName[260];  //.TextGrid
}file_name_list[1000];
//int number = 0;

string ReadLine(char *filename_txt, int line);
int CountFileLines(char *filename_txt);
int get_wavs();   //获取wav 
int get_txts();   //获取txt
//获取 *.wav 
int get_wavs()
{
	int number = 0;
	long Handle;
	//long HandleNext;  /* 当搜索文件是 *.* 时打开 */
	struct _finddata_t File;
	string sPathLast_One = sRoot + sSuffix_wav;
	printf("sPathLast_One:%s\n", sPathLast_One.c_str());
	Handle = _findfirst(sPathLast_One.c_str(), &File);
	if (Handle == -1) {
		printf("Can't find any files!\n"); return -1;
	}
	else {
		//printf("Firstfile:%s\n", File.name);
	}
	//HandleNext = _findnext(Handle, &File);
	while (!_findnext(Handle, &File))
	{
		for (int i = 0; i < 260; i = i++) {
			file_name_list[number].filename[i] = File.name[i];
		}
		printf("%s\n", File.name);
		number = number + 1;
	}
	printf("wav_number:%d\n", number);
	_findclose(Handle); return number;
}
//获取 *.TeatGrid.txt 
int get_txts()
{
	int number = 0;
	long Handle;
	//long HandleNext;
	struct _finddata_t File;
	string sPathLast_Two = sRoot + sSuffix_txt;
	printf("sPathLast_Two:%s\n", sPathLast_Two.c_str());
	Handle = _findfirst(sPathLast_Two.c_str(), &File);
	if (Handle == -1) {
		printf("Can't find any files!\n"); return -1;
	}
	else {
		//printf("Firstfile:%s\n", File.name);
	}
	//HandleNext = _findnext(Handle, &File);
	while (!_findnext(Handle, &File))
	{
		for (int i = 0; i < 260; i++) {
			file_name_list[number].fileName[i] = File.name[i];
		}
		number = number + 1;
		printf("%s\n", File.name);
	}
	printf("txt_number:%d\n", number);
	_findclose(Handle); return number;
}
//Read line
string ReadLine(char *filename_txt, int line)
{
	string str;
	int i = 0;
	fstream file;
	file.open(filename_txt);
	while (getline(file, str) && i < line - 1)
	{
		i++;
	}
	file.close();
	printf("Readline:%s\n", str.c_str()); return str;
}
//find string
string FindString(string str)
{
	//double number = 0;
	string str1;
	size_t position;
	position = str.find("=");
	if (position != string::npos)
	{
		str1 = str.substr(position + 1);
		printf("FindString:%s\n", str1.c_str());
	}
	else
		cout << "not found!" << endl;
	return str1;
}
//获取双引号里面字符串
string StringToChar(string str)
{
	string dest = "";
	int pos_1 = str.find("\"");
	int pos_2 = str.find_last_of("\"");
	dest = str.substr(pos_1 + 1, pos_2 - pos_1 - 1);
	printf("buf = %s\n", dest.c_str());
	return dest;
}
#if 1
//find number
double FindNumberInString(string str)
{
	size_t position;
	string str2;
	double num = 0;
	char ch[64] = { 0 };
	position = str.find("=");
	if (position != string::npos)
	{
		str2 = str.substr(position + 1);
		num = atof(str2.c_str());
		cout << setprecision(17) << "FindNumberInString:" << num << endl;
	}
	else
		cout << "not found!" << endl;
	//	cout<<"final_num:"<<num<<endl;
	return num;
}
#endif
//保存为新的wav
void Save_to_NewWav(FILE *fp, int bytepermin, double time, int data_flag, int datasize, unsigned char ch[], string Sex)
{
	//cout << "--------------------Sex:" << Sex << endl;
	cout << "******************start Wav write***************" << endl;
	if (Sex == "女1")
	{
		cout << "------------------女1----------------------" << endl;
		int newfile_datasize = bytepermin * time;   /*确定截取 wav 的大小*/
		printf("\nnewfile_datasize:%d\n", newfile_datasize);
		int newfile_size = newfile_datasize + data_flag;
		ch[data_flag + 7] = newfile_datasize / (16 * 16 * 16 * 16 * 16 * 16);
		ch[data_flag + 6] = (newfile_datasize - ch[data_flag + 7] * (16 * 16 * 16 * 16 * 16 * 16)) / (16 * 16 * 16 * 16);
		ch[data_flag + 5] = (newfile_datasize - ch[data_flag + 7] * (16 * 16 * 16 * 16 * 16 * 16) - ch[data_flag + 6] * (16 * 16 * 16 * 16)) / (16 * 16);
		ch[data_flag + 4] = newfile_datasize - ch[data_flag + 7] * (16 * 16 * 16 * 16 * 16 * 16) - ch[data_flag + 6] * (16 * 16 * 16 * 16) - ch[data_flag + 5] * (16 * 16);

		ch[7] = newfile_size / (16 * 16 * 16 * 16 * 16 * 16);
		ch[6] = (newfile_size - ch[7] * (16 * 16 * 16 * 16 * 16 * 16)) / (16 * 16 * 16 * 16);
		ch[5] = (newfile_size - ch[7] * (16 * 16 * 16 * 16 * 16 * 16) - ch[6] * (16 * 16 * 16 * 16)) / (16 * 16);
		ch[4] = newfile_size - ch[7] * (16 * 16 * 16 * 16 * 16 * 16) - ch[6] * (16 * 16 * 16 * 16) - ch[5] * (16 * 16);
		for (int i = 0; i < data_flag + 8; i++)
		{
			if (i % 16 == 0)      //每行输出16个字符对应的十六进制数
				printf("\n");
			if (ch[i] < 16)       //对小于16的数，在前面加0，使其用8bit显示出来
				printf("0%x ", ch[i]);
			else
				printf("%x ", ch[i]);
		}
		//此时ch[i]中存放的是新文件的文件头信息。
		FILE *fp1;
		printf("\n");
		int m = data_flag + 8;
		char female_path[64] = { 0 }; //新的 wav 名
		sprintf(female_path, "./female/female_%d.wav", female_num);
		female_num++;
		fp1 = fopen(female_path, "wb");//以写的方式打开文件  
		for (int j = 0; j < data_flag + 8; j++)  //将特定头文件写入新文件 
		{
			fputc(ch[j], fp1);
		}
		for (int t = 0; t < newfile_datasize; t++)//将新的data数据写入新文件
		{
			if (m < 100)
			{
				fputc(ch[m], fp1); m = m + 1;
			}
			else
			{
				fputc(fgetc(fp), fp1);
			}
		}

		printf("newFemalewav:%s\n", female_path);
		fflush(fp1);
		fclose(fp1);
	}
	else if (Sex == "男1")
	{
		cout << "--------------------------男1-----------------------" << endl;
		int newfile_datasize = bytepermin * time;   /*确定窃取 wav 的大小*/
		printf("\nnewfile_datasize:%d\n", newfile_datasize);
		int newfile_size = newfile_datasize + data_flag;
		ch[data_flag + 7] = newfile_datasize / (16 * 16 * 16 * 16 * 16 * 16);
		ch[data_flag + 6] = (newfile_datasize - ch[data_flag + 7] * (16 * 16 * 16 * 16 * 16 * 16)) / (16 * 16 * 16 * 16);
		ch[data_flag + 5] = (newfile_datasize - ch[data_flag + 7] * (16 * 16 * 16 * 16 * 16 * 16) - ch[data_flag + 6] * (16 * 16 * 16 * 16)) / (16 * 16);
		ch[data_flag + 4] = newfile_datasize - ch[data_flag + 7] * (16 * 16 * 16 * 16 * 16 * 16) - ch[data_flag + 6] * (16 * 16 * 16 * 16) - ch[data_flag + 5] * (16 * 16);

		ch[7] = newfile_size / (16 * 16 * 16 * 16 * 16 * 16);
		ch[6] = (newfile_size - ch[7] * (16 * 16 * 16 * 16 * 16 * 16)) / (16 * 16 * 16 * 16);
		ch[5] = (newfile_size - ch[7] * (16 * 16 * 16 * 16 * 16 * 16) - ch[6] * (16 * 16 * 16 * 16)) / (16 * 16);
		ch[4] = newfile_size - ch[7] * (16 * 16 * 16 * 16 * 16 * 16) - ch[6] * (16 * 16 * 16 * 16) - ch[5] * (16 * 16);
		for (int i = 0; i < data_flag + 8; i++)
		{
			if (i % 16 == 0)      //每行输出16个字符对应的十六进制数
				printf("\n");
			if (ch[i] < 16)       //对小于16的数，在前面加0，使其用8bit显示出来
				printf("0%x ", ch[i]);
			else
				printf("%x ", ch[i]); 
		}
		//此时ch[i]中存放的是新文件的文件头信息。
		FILE *fp1;
		printf("\n");
		int m = data_flag + 8;
		char male_path[64] = { 0 };  //新的 wav 名
		sprintf(male_path, "./male/male_%d.wav", male_num);
		male_num++;

		fp1 = fopen(male_path, "wb");//以写的方式打开文件  
		for (int j = 0; j < data_flag + 8; j++)   //将特定头文件写入新文件 
		{
			fputc(ch[j], fp1);
		}
		for (int t = 0; t < newfile_datasize; t++)//将新的data数据写入新文件
		{
			if (m < 100)
			{
				fputc(ch[m], fp1); m = m + 1;
			}
			else
			{
				fputc(fgetc(fp), fp1);
			}
		}

		printf("newMalewav:%s\n", male_path);
		fflush(fp1);
		fclose(fp1);
	}
	else
	{
		cout << "Not match any files!" << endl; return;
		//system("pause"); exit(0);
	}
}

/*get file lines */
int CountFileLines(char *filename_txt)
{
	ifstream Readfile;
	string tmp;
	int lines = 0;
	Readfile.open(filename_txt, ios::in);
	if (Readfile.fail())
	{
		perror("open"); exit(0);
	}
	else
	{
		while (getline(Readfile, tmp, '\n')) {
			lines++;
		}
	}
	//cout<<"CountFileLines:"<<lines<<endl;
	Readfile.close();
	return lines;
}
#if 1
//The key point
void Wav_Cut(char filename[], char fileName[])
{
	ifstream ReadFile;
	char filename_wav[260];
	char filename_txt[260];
	sprintf(filename_wav, "data/%s", filename);
	printf("%s\n", filename_wav);
	sprintf(filename_txt, "data/%s", fileName);
	printf("%s\n", filename_txt);
	int n = 0;
	int n1 = 0;
	int filelines = 0;
	string tmp;
	int lenalready = 0;
	FILE *fp = fopen(filename_wav, "rb");    /*打开.wav*/
	//printf("%s\n", filename_wav);
	if (fp == NULL)
	{
		perror("open"); system("pause"); exit(0);
	}

	//---------------------------------- 获取原 wav 信息 ----------------------------------------//
	unsigned char ch[100];   //存储wav 的头信息
	for (int i = 0; i < 100; i++)
	{
		ch[i] = fgetc(fp);   //每次读取一个字符，存在数组ch中
		if (i % 16 == 0)     //每行输出16个字符对应的十六进制数
			printf("\n");
		if (ch[i] < 16)      //对小于16的数，在前面加0，使其用8bit显示出来
			printf("0%x ", ch[i]);
		else
			printf("%x ", ch[i]);
	}
	int data_flag = 0;
	for (int i = 32; i < 100; i++) //find data_flag
	{
		if (ch[i] == 't'&&ch[i - 1] == 'a'&&ch[i - 2] == 'd'&&ch[i + 1] == 'a')
		{
			data_flag = i - 2;
			break;
		}
	}
	printf("data_flag:%d\n", data_flag);
	int bytepermin = *((int*)&ch[28]);//ch[31]*(16*16*16*16*16*16)+ch[30]*(16*16*16*16)+ch[29]*(16*16)+ch[28];   //每秒字节数 
	printf("\nbytepermin:%d\n", bytepermin);
	int datasize = *((int*)&ch[data_flag + 4]);//ch[data_falg+7]*(16*16*16*16*16*16)+ch[data_falg+6]*(16*16*16*16)+ch[data_falg+5]*(16*16)+ch[data_falg+4];//data长度 
	printf("\ndatasize:%d\n", datasize);
	ReadFile.open(filename_txt, ios::in);   //打开.txt, ios::in 表示以只读的方式读取文件  
	printf("%s\n", filename_txt);
	if (ReadFile.fail())    //文件打开失败:返回0    
	{
		perror("open");	system("pause"); exit(0);
	}
	else
	{
		while (getline(ReadFile, tmp, '\n')) // 定位开始循环的行        
		{
			n++; n1 = n;
			if (n1 == 15)
				break;
		}
		filelines = CountFileLines(filename_txt);
		for (n1; n1 <= filelines; n1++)
		{
			string str4 = ReadLine(filename_txt, n1);
			string NoneStr = FindString(str4);
			n1 = n1 + 1;

			/* string  to  double number */
			string str = ReadLine(filename_txt, n1);
			double Xmin = FindNumberInString(str);
			//cout<<"First_num:"<<Xmin<<endl;
			int pos_1 = (int)(Xmin*bytepermin);
			cout << "pos_1:" << pos_1 << endl;
			n1 = n1 + 1;

			string str1 = ReadLine(filename_txt, n1);
			double Xmax = FindNumberInString(str1);
			int pos_2 = (int)(Xmax*bytepermin);
			cout << "pos_2:" << pos_2 << endl;
			n1 = n1 + 1;

			double time = Xmax - Xmin;  /*获得wav片段时间*/

			string str2 = ReadLine(filename_txt, n1);
			string Sex = StringToChar(str2);
			printf("Sex = %s\n", Sex.c_str());

			/* Save to New  **.Wav */
			Save_to_NewWav(fp, bytepermin, time, data_flag, datasize, ch, Sex);
			fseek(fp, pos_2, SEEK_SET);   /*重新定位 wav 的截取开始点*/
			//int CurrentLen = ftell(fp);
			//printf("CurrentLen:%d\n", CurrentLen);
		}
		ReadFile.close();
	}
	cout << "filelines ----------------------------- " << filelines << endl;
}
#endif

int main()
{
	string str;
	int number_tmp = 0;
	int number_wav = 0;
	int number_txt = 0;
	number_wav = get_wavs();
	number_txt = get_txts();
	if (number_txt == number_wav) {//判断wav音频与txt文件是否相互对应 
		number_tmp = number_txt;
	}
	else {
		printf("文件可能缺少对应项！\n"); exit(0);
	}
	//printf("number_tmp:%d\n", number_tmp);
	//将wav读到内存???need to replace the codes in  CountFileLines(...);
	for (int i = 0; i < number_tmp; i++)
	{
		printf("%s\n%s\n", file_name_list[i].filename, file_name_list[i].fileName);
		Wav_Cut(file_name_list[i].filename, file_name_list[i].fileName); //主要函数
	}
	system("pause");
	return 0;
}





