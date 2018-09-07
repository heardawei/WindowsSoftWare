#ifndef UNICODE
#define UNICODE
#endif /* UNICODE */

#ifndef _UNICODE
#define _UNICODE
#endif /* _UNICODE */

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>

#include <cmath>

#include <wchar.h>
#include <windows.h>
#include <windowsx.h> /* GET_X_LPARAM GET_Y_LPARAM*/
#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include "basewin.h"



class MainWindow : public BaseWindow<MainWindow>
{


public:

	MainWindow()
	{
	}

	PCWSTR  ClassName() const
	{
		return L"Circle Window Class";
	}
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	wchar_t msg[1024];
	switch (uMsg)
	{
	case WM_SYSKEYDOWN:
		swprintf_s(msg, L"WM_SYSKEYDOWN: 0x%x\n", wParam);
		OutputDebugString(msg);
		break;

	case WM_SYSCHAR:
		swprintf_s(msg, L"WM_SYSCHAR: '%c'\n", (wchar_t)wParam);
		OutputDebugString(msg);
		break;

	case WM_SYSKEYUP:
		swprintf_s(msg, L"WM_SYSKEYUP: 0x%x\n", wParam);
		OutputDebugString(msg);
		break;

	case WM_KEYDOWN:
		swprintf_s(msg, L"WM_KEYDOWN: 0x%x\n", wParam);
		OutputDebugString(msg);
		break;

	case WM_KEYUP:
		swprintf_s(msg, L"WM_KEYUP: 0x%x\n", wParam);
		OutputDebugString(msg);
		break;

	case WM_CHAR:
		swprintf_s(msg, L"WM_CHAR: '%c'\n", (wchar_t)wParam);
		OutputDebugString(msg);
		break;

		/* Handle other messages (not shown) */
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
	MainWindow win;

	if (!win.Create(L"Circle", WS_OVERLAPPEDWINDOW))
	{
		return 0;
	}

	ShowWindow(win.Window(), nCmdShow);

	// Run the message loop.

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}