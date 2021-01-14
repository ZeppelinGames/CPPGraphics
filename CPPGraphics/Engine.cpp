#include <Windows.h>
#include <math.h>
#include <iostream>
#include <string>
#include <ctime>
#include "Bounds.h"
#include "Mathf.h"
#include "Vector2.h"

#define _USE_MATH_DEFINES

bool running = true;

int halfWidth;
int halfHeight;

int fps=0;

struct Render_State {
	int width, height;
	void* memory;

	BITMAPINFO bitmap_info;
};

Render_State render_state;

#include "renderer.cpp"

LRESULT CALLBACK Window_Callback(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	LRESULT result = 0;

	switch (uMsg)
	{
		case(WM_CLOSE):
		case (WM_DESTROY): {
			running = false;
		} break;

		case WM_SIZE: {
			RECT rect;
			GetClientRect(hwnd, &rect);
			render_state.width = rect.right - rect.left;
			render_state.height = rect.bottom - rect.top;

			halfWidth = render_state.width / 2;
			halfHeight = render_state.height / 2;

			int bufferSize = render_state.width * render_state.height * sizeof(unsigned int);

			if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
			render_state.memory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
			render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
			render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
			render_state.bitmap_info.bmiHeader.biPlanes = 1;
			render_state.bitmap_info.bmiHeader.biBitCount = 32;
			render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;

		} break;
			
		default: {
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	return result;
}

std::wstring StringToWString(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

double clockToMilliseconds(clock_t ticks) {
	// units/(units/time) => time (seconds) * 1000 = milliseconds
	return (ticks / (double)CLOCKS_PER_SEC) * 1000.0;
}

clock_t deltaTime = 0;
unsigned int frames = 0;
double  frameRate = 30;
double  averageFrameTimeMilliseconds = 33.333;
std::wstring conv = L"0";


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	//Create Window Class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = L"Main Window Class";
	window_class.lpfnWndProc = Window_Callback;

	//Register Class
	RegisterClass(&window_class);

	//Create window
	HWND window = CreateWindow(window_class.lpszClassName, L"Main Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	//Get monitor
	MONITORINFO monitor_info;
	monitor_info.cbSize = sizeof(monitor_info);
	GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST),
		&monitor_info);
	//Get monitor size
	RECT window_rect(monitor_info.rcMonitor);

	float angle = 0;

	while (running) {
		clock_t beginFrame = clock();

		//Input
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		//Simulate
		ClearScreen(0x000000);

		//RenderBackground();
		
		DrawRect(Vector2(10, 200), Vector2(100, 100), 0xFFFFFF);

		DrawRotatedRect(Vector2(700, 25), Vector2(100, 100), angle, 0xFFFFFF);
		angle += 1.0f;

		//DrawLine(Vector2(200, 300), Vector2(200, 600), 0xFFFFFF);
		//DrawLine(Vector2(200, 600), Vector2(600, 600), 0xFFFFFF);
		//DrawLine(Vector2(-100, 600), Vector2(900, 200), 0xFFFFFF);

		Point(Vector2(halfWidth, halfHeight), 0xFFFFFF);

		DrawThickLine(Vector2(1000, 500), Vector2(800, 700), 100, 0xFFFFFF);

		//DrawCircle(Vector2(800, 800), 300, 0xFFFFFF);
		//DrawCircle(Vector2(900, 600), 100, 5, 0xFFFFFF);
		//DrawCircle(Vector2(700, 600), 100, true, 0xFFFFFFF); 

		//Render 
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		clock_t endFrame = clock();
		deltaTime += endFrame - beginFrame;
		frames++;

		if (clockToMilliseconds(deltaTime) > 1000.0) { //every second
			frameRate = (double)frames * 0.5 + frameRate * 0.5; //more stable
			frames = 0;
			deltaTime -= CLOCKS_PER_SEC;
			averageFrameTimeMilliseconds = 1000.0 / (frameRate == 0 ? 0.001 : frameRate);
		}

		//Draw FPS on screen
		RECT rect = { 20,20, 200, 200 };
		conv = StringToWString(std::to_string((int)frameRate)); //Convert string to WString
		LPCWSTR text = conv.c_str(); //Convert WString to LPCWSTR
		DrawText(hdc, text, -1, &rect, DT_LEFT); //Draw Text
	}
};
