#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <Windows.h>
#include <iostream>

using namespace cv;
using namespace std;

void SetWindow(int Width, int Height)
{
	_COORD coord;
	coord.X = Width;
	coord.Y = Height;
	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = Height - 1;
	Rect.Right = Width - 1;
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(Handle, coord);
	SetConsoleWindowInfo(Handle, TRUE, &Rect);
	string str = "mode con cols=" + to_string(Width) + " lines=" + to_string(Height);
	system(str.c_str());
	
}

int main(void)
{
	int nwidth = 170; // При необходимости можно изменить размер
	float aspect = 11.0 / 24.0;
    string path = "C:\\Users\\rchek\\OneDrive\\Desktop\\2.jpg"; 
	// Вставьте сюда путь к фото
    Mat image = imread(path, IMREAD_COLOR);

	Size sz = image.size();
	int im_width = sz.width, im_height = sz.height;
	float imasp = (float)im_width / nwidth;
	int nheight = round(im_height * aspect / imasp);
	
	resize(image, image, { nwidth, nheight }, 0, 0, INTER_NEAREST);
	SetWindow(nwidth, nheight);

	wchar_t* screen = new wchar_t[nwidth * nheight+1];
	for (int i = 0; i < nwidth * nheight + 1; i++) screen[i] = ' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	const char* gradient = " .:!/r(lZ4H9W8$@";

	for (int x = 0; x < image.rows; x++) {
		for (int y = 0; y < image.cols; y++) {
			Vec3b pixel = image.at<Vec3b>(x, y);
			int color = round((pixel[0] + pixel[1] + pixel[2]) / 3.0);
			screen[x * nwidth + y] = gradient[color / 16];
		}
	}

	while (1) WriteConsoleOutputCharacter(hConsole, screen, nwidth * nheight, { 0, 0 }, &dwBytesWritten);
 
    return 0;
}