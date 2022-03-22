#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include "MyBmp.h"

using namespace std;

class Resampling {
private:
	double Pi = 3.141592653589793238463;
	int width;
	int height; 
	int new_width;
	int new_height;
	RGB** original;
	RGB** result;
public:
	Resampling(double n, double m) {
		if (n == 0 || m == 0 || (n - 0.0019) <= 0 || (m - 0.0019 <= 0)) {
			cout << "Incorrect parameters";
			return;
		}
		BITMAPFILEHEADER* bfh = new BITMAPFILEHEADER;
		BITMAPINFOHEADER* bih = new BITMAPINFOHEADER;
		FILE* file = fopen("Lena.bmp", "rb");
		fread(bfh, sizeof(*bfh) - 2, 1, file);
		fread(bih, sizeof(*bih), 1, file);
		width = bih->biWidth;
		height = bih->biHeight;
		new_height = height * n;
		new_width = width * m;		
		original = create(height, width);
		int step = 4 - (width * 3) % 4;
		for (int i = 0; i < height; i++)
		{
			fread(original[i], sizeof(RGB), width, file);
			if (step != 4) {
				fseek(file, step, SEEK_CUR);
			}
		}
		fclose(file);
		result = create(new_height, new_width);
		oversampling(n, m);

		FILE* file_res;
		file_res = fopen("Result.bmp", "wb");
		bih->biHeight = new_height;
		while (new_width % 4 != 0) {
			new_width++;
		}
		bih->biWidth = new_width;
		fwrite(bfh, sizeof(*bfh) - 2, 1, file_res);
		fwrite(bih, sizeof(*bih), 1, file_res);
		int step2 = 4 - ((width / 2) * 3) % 4;
		char buf = 0;
		for (int i = 0; i < new_height; i++)
		{
			fwrite((result[i]), sizeof(RGB), new_width, file_res);
			if (step2 != 4)
			{
				fwrite(&buf, 1, step2, file_res);
			}
		}
		fclose(file_res);
	}

	RGB** create(int h, int w) {
		RGB** rgb_table = new RGB * [h];
		for (int i = 0; i < h; i++)
			rgb_table[i] = new RGB[w];
		return rgb_table;
	}

	void oversampling(double n, double m) {
		double** tmpred;
		double** tmpblue;
		double** tmpgreen;
		tmpred = new double* [height];
		tmpblue = new double* [height];
		tmpgreen = new double* [height];
		for (int i = 0; i < height; i++)
		{
			tmpred[i] = new double[new_width];
			tmpblue[i] = new double[new_width];
			tmpgreen[i] = new double[new_width];
		}
		double red = 0;
		double blue = 0;
		double green = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 1; j <= new_width; j++) {
				for (int h = 1; h <= width; h++) {
					blue += ((double)original[i][h - 1].B * sinc(Pi*(j / m - h)));
					red += ((double)original[i][h - 1].R * sinc(Pi*(j / m - h)));
					green += ((double)original[i][h - 1].G * sinc(Pi*(j / m - h)));
				}
				tmpblue[i][j - 1] = blue;
				tmpgreen[i][j - 1] = green;
				tmpred[i][j - 1] = red;
				blue = 0;
				green = 0;
				red = 0;
			}
		}
		blue = 0;
		green = 0;
		red = 0;
		for (int i = 1; i <= new_height; i++) {
			for (int j = 0; j < new_width; j++) {
				for (int h = 1; h <= height; h++) {
					blue += (tmpblue[h - 1][j] * sinc(Pi*(i / n - h)));
					red += (tmpred[h - 1][j] * sinc(Pi* (i / n - h)));
					green += (tmpgreen[h - 1][j] * sinc(Pi *(i / n - h)));
				}
				result[i - 1][j].B = (unsigned char) blue;
				result[i - 1][j].G = (unsigned char)green;
				result[i - 1][j].R = (unsigned char)red;
				blue = 0;
				green = 0;
				red = 0;
			}
		}
		for (int i = 0; i < height; i++)
		{
			delete tmpred[i];
			delete tmpblue[i];
			delete tmpgreen[i];
		}
		delete tmpred;
		delete tmpblue;
		delete tmpgreen;
	}

	double sinc(double arg) {
		if (arg != 0)
			return (sin(arg) / arg);
		return 1;
	}
};