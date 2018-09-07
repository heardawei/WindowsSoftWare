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
#include <WinUser.h>

#pragma comment(lib, "d2d1")

#include "basewin.h"
#include "resource.h"


class MainWindow : public BaseWindow<MainWindow>
{


public:
	int mode;
	MainWindow()
	{
	}
	void SetMode(int mode)
	{
		/* TODO: */
		this->mode = mode;
	}

	PCWSTR  ClassName() const
	{
		return L"Window Class adfasdfsdfasd";
	}
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	wchar_t msg[1024];
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_DRAW_MODE:
			this->SetMode(ID_DRAW_MODE);
			break;

		case ID_SELECT_MODE:
			this->SetMode(ID_SELECT_MODE);
			break;

		case ID_TOGGLE_MODE:
			if (this->mode == ID_DRAW_MODE)
			{
				this->SetMode(ID_SELECT_MODE);
			}
			else
			{
				this->SetMode(ID_DRAW_MODE);
			}
			break;
		}
		return 0;

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
	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCEL1));
	if (hAccel == NULL)
	{
		OutputDebugString(L"LoadAccelerators failed\n");
		return 0;
	}

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

	DestroyAcceleratorTable(hAccel);
	return 0;
}