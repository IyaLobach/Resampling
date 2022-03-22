#include <iostream>

struct BITMAPFILEHEADER {
	short bfType;
	int bfSize;
	short bfReserved1;
	short bfOffBits;;
	int bfReserved2;
};

struct BITMAPINFOHEADER {
	int biSize;
	int biWidth;
	int biHeight;
	short int biPlanes;
	short int biBitCount;
	int biCompression;
	int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	int biClrUsed;
	int biClrImportant;
};

struct RGB {
	unsigned char B;
	unsigned char G;
	unsigned char R;
};