﻿// Graph.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <cmath>
#pragma execution_character_set("utf-8")

struct Question
{
	double dimentionless_time;//無次元時間
	double dimentionless_distance;//無次元距離(今回のx軸)
};

class Answer
{
private:
	Question q;
public:
	Answer(Question q);
	~Answer();
	void change_x(double r2);
	double get_x();
	void change_tau(double dimentionless_time);
	double get_tau();
	double printAnswer();
};

Answer::Answer(Question q)
{
	this->q = q;
}

Answer::~Answer()
{
}

void Answer::change_x(double x)
{
	this->q.dimentionless_distance = x;
}

double Answer::get_x()
{
	return this->q.dimentionless_distance;
}

void Answer::change_tau(double dimentionless_time)
{
	this->q.dimentionless_time = dimentionless_time;
}

double Answer::get_tau()
{
	return this->q.dimentionless_time;
}

double Answer::printAnswer()
{
	const double pi = 3.14159265359;

	double dimentionless_heat = 0;
	unsigned int n;
	unsigned int oddN;//(2n-1)のこと
	const int limit = 100;

	for (n = 1; n < limit; n++)
	{
		oddN = (2 * n - 1);

		//pow(x1,x2) = x1をx2で累乗したものを戻り値とする。
		dimentionless_heat += (1 / oddN) * exp(-pow(oddN, 2) * pow(pi, 2) * this->q.dimentionless_time) * sin(oddN * pi * this->q.dimentionless_distance);
	}
	dimentionless_heat *= 4 / pi;
	return dimentionless_heat;
}

class printFileCreator
{
private:
	FILE *fp;
	bool error = false;
public:
	printFileCreator(const char *Filename);
	~printFileCreator();
	bool printFileMain(Answer a);
};

printFileCreator::printFileCreator(const char *Filename)
{
	try {
		if (Filename == NULL) {
			wchar_t *e;
			char *message = "ファイル名が無記入です";
			mbstowcs(e, message, sizeof(message));//char->wchar_t
			throw *e;
		}
		fopen_s(&fp, Filename, "w");
		if (fp == nullptr) {
			wchar_t *e;
			char *message = "ファイルが開けません";
			mbstowcs(e, message, sizeof(message));//char->wchar_t
			throw *e;
		}
	}
	catch (wchar_t *e)
	{
		std::cout << *e << std::endl;
		error = true;
	}
}

printFileCreator::~printFileCreator()
{
	//UnInitialized
	fclose(fp);
}

bool printFileCreator::printFileMain(Answer a)
{
	if (error)
	{
		return false;
	}

	//計算をしてtest.batに代入

	//現在のx軸
	double current_x = a.get_x();
	//現在の答え
	double current_Answer = 0;
	//一回のループでx軸の値がいくつ増えるか
	const double loop_up_x = 0.01;

	fprintf(fp, "# x/b θ/θ0");//ラベルを記入
	while (a.get_x() <= 1)
	{
		fprintf(fp, "\n");//改行
		a.change_x(current_x);//x軸の値をオブジェクトに代入
		current_Answer = a.printAnswer();//オブジェクトにて計算された答えをもらう
		fprintf(fp, "%lf %lf", current_x, current_Answer);//値をファイルに書き込む
		current_x += loop_up_x;//x軸の値を増やす
	}
	return true;
}

class batFileCreator
{
private:
	FILE *fp;
	bool error = false;
public:
	batFileCreator(const char *Filename);
	~batFileCreator();
	bool createbatFile(const char *printFile);

};

batFileCreator::batFileCreator(const char *Filename)
{
	try {
		if (Filename == NULL) {
			wchar_t *e;
			char *message = "ファイル名が無記入です";
			mbstowcs(e, message, sizeof(message));//char->wchar_t
			throw *e;
		}
		fopen_s(&fp, Filename, "w");
		if (fp == nullptr) {
			wchar_t *e;
			char *message = "ファイルが開けません";
			mbstowcs(e, message, sizeof(message));//char->wchar_t
			throw *e;
		}
	}
	catch (wchar_t *e)
	{
		std::cout << *e << std::endl;
		error = true;
	}
}

batFileCreator::~batFileCreator()
{
	//UnInitialized
	fclose(fp);
}

bool batFileCreator::createbatFile(const char *printFile)
{
	if (error || printFile == NULL)
	{
		return false;
	}
	fprintf(fp, "set encoding utf8\n");//UTF-8に設定
	fprintf(fp, "set xrange[0:1]\n");//x軸の範囲を指定
	fprintf(fp, "set xl \"無次元距離 x/b\"\n");//x軸に名前を付ける
	fprintf(fp, "set yl \"無次元温度 θ/θ0\"\n");//y軸に名前を付ける
	fprintf(fp, "set grid\n");//グリッド線を付ける
	fprintf(fp, "plot");//描画
	fprintf(fp, printFile);
	fprintf(fp, " with lp\n");
	return true;
}


int main()
{

	bool error;
	const char *Filename0005 = "test0005.bat";
	const char *Filename001 = "test001.bat";
	const char *Filename005 = "test005.bat";
	const char *Filename01 = "test01.bat";
	const char *Filename05 = "test05.bat";
	//オブジェクトをインスタンスして値を代入
	Question *q = new Question{ 0.005, 0 };
	Answer a(*q);
	delete q;

	//τ=0.005
	printFileCreator *printFile0005 = new printFileCreator(Filename0005);
	error = printFile0005->printFileMain(a);
	delete printFile0005;

	if (!error)
	{
		return -1;
	}

	//τ=0.01
	a.change_tau(0.01);
	printFileCreator *printFile001 = new printFileCreator(Filename001);
	error = printFile001->printFileMain(a);
	delete printFile001;

	if (!error)
	{
		return -1;
	}

	//τ=0.05
	a.change_tau(0.05);
	printFileCreator *printFile005 = new printFileCreator(Filename005);
	error = printFile005->printFileMain(a);
	delete printFile005;

	if (!error)
	{
		return -1;
	}

	//τ=0.1
	a.change_tau(0.1);
	printFileCreator *printFile01 = new printFileCreator(Filename01);
	error = printFile01->printFileMain(a);
	delete printFile01;

	if (!error)
	{
		return -1;
	}

	//τ=0.5
	a.change_tau(0.5);
	printFileCreator *printFile05 = new printFileCreator(Filename05);
	error = printFile05->printFileMain(a);
	delete printFile05;

	if (!error)
	{
		return -1;
	}

	char graphFile[100];
	sprintf(graphFile, "\"%s\" title \"τ=0.005\", \"%s\" title \"τ=0.01\", \"%s\" title \"τ=0.05\", \"%s\" title \"τ=0.1\", \"%s\" title \"τ=0.5\"", Filename0005, Filename001, Filename005, Filename01, Filename05);
	//sprintf(graphFile, "%s", Filename0005);

	batFileCreator *batFile = new batFileCreator("command.bat");
	error = batFile->createbatFile(graphFile);
	delete batFile;

	if (!error)
	{
		return -1;
	}

	//コマンド用batをwgnuplotで開く
	system("C:\\\"Program Files (x86)\"\\gnuplot\\bin\\wgnuplot.exe -persist \"command.bat\"");

	return 0;
}
