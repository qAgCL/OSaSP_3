
#include "pch.h"
#include <iostream>
#include <string>
#include "InjectFunc.h"
#include <Windows.h> 
#include <math.h>
#include <time.h>
#include <tchar.h>
#include <fstream>
#include <string>
using namespace std;
const int MainWindowHeight = 500;
const int MainWindowWidth = 500;
HWND hProgName, hOldString, hNewString,hInjectButton;
const HBRUSH WindowBackground = CreateSolidBrush(RGB(255, 255, 255));
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		MessageBox(NULL, "Test", "Test", MB_OK);
		WNDCLASSEX WinClass;
		const char* ClassName = "MainClass";
		WinClass.cbSize = sizeof(WinClass);
		WinClass.lpfnWndProc = (WNDPROC)WndProc;
		WinClass.style = 0;
		WinClass.cbClsExtra = 0;
		WinClass.cbWndExtra = 0;
		WinClass.hInstance = NULL;
		WinClass.hIcon = LoadIcon(NULL, IDI_ERROR);
		WinClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WinClass.hbrBackground = WindowBackground;
		WinClass.lpszMenuName = NULL;
		WinClass.lpszClassName = ClassName;
		WinClass.hIconSm = NULL;
		if (!RegisterClassEx(&WinClass)) {
			MessageBox(NULL,
				"Не получилось зарегистрировать класс!",
				"Ошибка", MB_OK);
			return NULL;
		}
		const char* WindowName = "MainWindow";
		HWND MainWindow = CreateWindowEx(0, ClassName, WindowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, MainWindowWidth, MainWindowHeight, NULL, NULL, NULL, NULL);
		if (!MainWindow) {
			MessageBox(NULL,
				"Не получилось создать окно",
				"Ошибка", MB_OK);
			return NULL;
		}
		ShowWindow(MainWindow, SW_RESTORE);
		UpdateWindow(MainWindow);
		MSG RecMessage;
		while (GetMessage(&RecMessage, NULL, 0, 0)) {
			TranslateMessage(&RecMessage);
			DispatchMessage(&RecMessage);
		}
		DeleteObject(MainWindow);
		return RecMessage.wParam;

	}
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInst;
	PAINTSTRUCT ps;
	HDC hdc;
	switch (uMsg) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps); // начало перерисовки
		TextOut(hdc, 50, 50, "Program name", 13); // вывод текстовых сообщений
		TextOut(hdc, 50, 150, "Old string", 13); // вывод текстовых сообщений
		TextOut(hdc, 50, 250, "New string", 13); // вывод текстовых сообщений
		EndPaint(hWnd, &ps); // конец перерисовки
		break;
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		hProgName = CreateWindow("edit", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 50, 100, 250, 20,
			hWnd, 0, hInst, NULL);
		hOldString = CreateWindow("edit", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 50, 200, 250, 20,
			hWnd, 0, hInst, NULL);
		hNewString = CreateWindow("edit", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 50, 300, 250, 20,
			hWnd, 0, hInst, NULL);
		hInjectButton = CreateWindow("button", "Inject",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			50, 350, 120, 30, hWnd, 0, hInst, NULL);
		break;
	case WM_COMMAND:
		if (lParam == (LPARAM)hInjectButton) {
			char BufStr[200] = { '\0' };
			GetWindowText(hProgName, BufStr, 200);
			char *ProgramName = new char[strlen(BufStr)+1];
			strcpy(ProgramName, BufStr);

			ZeroMemory(BufStr, 200); 
			GetWindowText(hOldString, BufStr, 200);
			char *OldString = new char[strlen(BufStr) + 1];
			strcpy(OldString, BufStr);

			ZeroMemory(BufStr, 200);
			GetWindowText(hNewString, BufStr, 200);
			char *NewString = new char[strlen(BufStr) + 1];
			strcpy(NewString, BufStr);
			
			find_change_string(ProgramName, OldString, NewString);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(NULL);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return NULL;
}