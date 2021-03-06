//MeaningLessSoftware.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "MainWindow.h"
#include "MeaningLessSoftware.h"

#include <Windows.h>
#include <WinUser.h>







int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MainWindow mWin(hInstance);

	if (mWin.Create(WS_OVERLAPPEDWINDOW) == FALSE)
	{
		/* TODO: handle error */
		return 0;
	}

	ShowWindow(mWin.Window(), nCmdShow);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MEANINGLESSSOFTWARE));

	MSG msg;

	// 主消息循环: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}
