#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
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
	int nwidth = 245; // При необходимости можно изменить размер
	float aspect = 11.0 / 24.0;
    string path = ""; 
	// Вставьте сюда путь к видео
	VideoCapture vid(path);

	int im_width = static_cast<int>(vid.get(CAP_PROP_FRAME_WIDTH));
	int im_height = static_cast<int>(vid.get(CAP_PROP_FRAME_HEIGHT));
	//int im_width = sz.width, im_height = sz.height;
	float imasp = (float)im_width / nwidth;
	int nheight = round(im_height * aspect / imasp);
	
	//resize(image, image, { nwidth, nheight }, 0, 0, INTER_NEAREST);
	SetWindow(nwidth, nheight);

	wchar_t* screen = new wchar_t[nwidth * nheight+1];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	const char* gradient = " .:!/r(lZ4H9W8$@";

	Mat frame;

	while (1)
	{
		vid >> frame;
		if (frame.empty()) break;
		resize(frame, frame, { nwidth, nheight }, 0, 0, INTER_NEAREST);
		for (int x = 0; x < frame.rows; x++) {
			for (int y = 0; y < frame.cols; y++) {
				Vec3b pixel = frame.at<Vec3b>(x, y);
				int color = round((pixel[0] + pixel[1] + pixel[2]) / 3.0);
				screen[x * nwidth + y] = gradient[color / 16];
			}
		}
		WriteConsoleOutputCharacter(hConsole, screen, nwidth * nheight, { 0, 0 }, &dwBytesWritten);
	}
 
    return 0;
}