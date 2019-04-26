#include <cstdio>
#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "stringEater.h"
#include "wx.h"
using namespace std;

#define string_equals(a,b) (strcmp((a),(b))==0)
#define MAXX 448
#define MAXY 448
#define MAXZ 128
#define FORMAT_MATRIX 0
#define FORMAT_CSV 1

int outputType = 0;
bool graph[MAXX][MAXY][MAXZ];

ifstream input;
ofstream output;

void usage() {
	printf("关于程序\n");
	printf("输入文件：TubeTK平台提供的血管标注文件（.tre）\n");
	printf("输出文件：三维矩阵（448x448x128）二进制文件，存储方式如下\n");
	printf("matrix:\nfor (int z = 0; z < 128; z++)\n\tfor (int y = 0; y < 448; y++)\n\t\tfor (int x = 0; x < 448; x++)\n\t\t\toutput << graph[x][y][z]; \n");
	printf("csv:\nfor (int i = 0; i < n; i++)\n\toutput << x << \",\" << y << \",\" << z << endl;\n");
	printf("参数列表：\n-input [PATH]  输入文件地址\n");
	printf("-output [PATH]  输出文件地址\n");
	printf("-format [matrix|csv]  输出格式：顺序输出整个矩阵|在csv文件中输出非0元素位置\n");
}

void checkArgs(int argc, char** argv) {
	bool fail = false;
	for (int i = 1; i < argc; i += 2) {
		if (string_equals(argv[i], "-input"))
			input.open(argv[i + 1]);
		else if (string_equals(argv[i], "-output"))
			output.open(argv[i + 1]);
		else if (string_equals(argv[i], "-format")) {
			if (string_equals(argv[i + 1], "csv"))
				outputType = FORMAT_CSV;
			else if (string_equals(argv[i + 1], "matrix"))
				outputType = FORMAT_MATRIX;
			else {
				fail = true;
				break;
			}
		}
		else {
			fail = true;
			break;
		}
	}

	if (fail || !input.is_open() || !output.is_open()) {
		printf("参数有误\n");
		usage();
		exit(0);
	}
}

bool check(Vector3 v1, Vector3 v2, Vector3 target, double r) {
	return DistanceToSegment(target, v1, v2) < r;
}

void draw(Vector3 v1, Vector3 v2, int x, int y, int z, double r) {
	if (x < 0 || x >= MAXX || y < 0 || y >= MAXY || z < 0 || z >= MAXZ || graph[x][y][z]) return;
	if (check(v1, v2, Vector3(x, y, z), r)) {
		graph[x][y][z] = true;
		draw(v1, v2, x - 1, y, z, r);
		draw(v1, v2, x - 1, y, z, r);
		draw(v1, v2, x, y - 1, z, r);
		draw(v1, v2, x, y + 1, z, r);
		draw(v1, v2, x, y, z - 1, r);
		draw(v1, v2, x, y, z + 1, r);
	}
	else return;
}

char getter() {
	if (input.eof()) return 0;
	else return input.get();
}

void ignoreInt(int t) {
	int z;
	for (int i = 0; i < t; i++) {
		input >> z;
	}
}

void trans() {
	int t = 0;
	stringEater neater("NPoints = ", getter), peater("Points = ", getter);
	while (1) {
		neater.clearCache();
		int n;
		if (neater.jump()) {
			input >> n;
		}
		else return;
		peater.clearCache();
		if (!peater.jump()) return;
		double nowx, nowy, nowz, nowr;
		input >> nowx >> nowy >> nowz >> nowr;
		ignoreInt(21);
		printf("\n正在转换第%d组%d个结点 ", ++t, n);
		for (int i = 2; i <= n; i++) {
			double x, y, z, r;
			input >> x >> y >> z >> r;
			ignoreInt(21);
			Vector3 v1(nowx, nowy, nowz), v2(x, y, z);
			draw(v1, v2, x, y, z, nowr);
			nowx = x;
			nowy = y;
			nowz = z;
			nowr = r;
			if (i % 100 == 0)printf(".");
		}
	}
}

char mem[MAXX * MAXY * MAXZ];
int top;

void print() {
	printf("\n正在输出");
	if (outputType == FORMAT_MATRIX) {
		top = 0;
		int ans = 0;
		for (int z = 0; z < MAXZ; z++)
			for (int y = 0; y < MAXY; y++)
				for (int x = 0; x < MAXX; x++)
					ans += ((mem[top++] = graph[x][y][z] ? '1' : '0') == '1');
		output << mem;
	}
	else {
		for (int z = 0; z < MAXZ; z++)
			for (int y = 0; y < MAXY; y++)
				for (int x = 0; x < MAXX; x++)
					if (graph[x][y][z])
						output << x << "," << y << "," << z << endl;
	}
	printf("\n输出完毕\n");
}

int main(int argc, char** argv) {

	checkArgs(argc, argv);
	trans();
	print();

	input.close();
	output.close();

	return 0;
}