
#include "stdafx.h"
#include "Login.h"



LoginWindow::LoginWindow()
{
}

LoginWindow::~LoginWindow()
{
}

PCWSTR LoginWindow::ClassName() const
{
	return L"Hello I'm Login Class Name";
}

PCWSTR LoginWindow::WindowName() const
{
	return L"Hello I'm Login Window Name";
}

LRESULT LoginWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	INT numTrans;
	RECT rt;

	switch (uMsg)
	{
#if 0
	case WM_COMMAND:
		// wmId = LOWORD(wParam);
		// wmEvent = HIWORD(wParam);
		break;
	case WM_PAINT:

		hdc = BeginPaint(this->hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		GetClientRect(this->hWnd, &rt);
		numTrans = (INT)GetWindowLong(this->hWnd, 0);  //获取子窗口中的数字
		wchar_t str[10];
		_itow_s(numTrans, str, 10);  //将原本int类型转为char 留作显示
		TextOut(hdc, 30, 30, str, 1);
		EndPaint(this->hWnd, &ps);
#endif
		break;
	case WM_CREATE:
		//CreateWindow();
		break;
	case WM_LBUTTONDOWN:
		// numTrans = (INT)GetWindowLong(this->hWnd, 0);
		// SendMessage(GetParent(hWnd), WM_CHANGE_NUM, (WPARAM)0, (LPARAM)numTrans);
		// DestroyWindow(this->hWnd);
		break;
	case WM_DESTROY:
		DestroyWindow(this->hWnd);
		break;
	default:
		return DefWindowProc(this->hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

#if USE_SELF_CREATE
BOOL LoginWindow::Create(DWORD dwStyle, DWORD dwExStyle,
	int x, int y, int nWidth, int nHeight,
	HWND hWndParent, HMENU hMenu)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	/* Main Window Class */
	wcex.style = /*CS_HREDRAW | CS_VREDRAW | */CS_DBLCLKS;
	wcex.lpfnWndProc = BaseWindow::WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 5;
	wcex.hInstance = this->hInstance;
	wcex.hIcon = LoadIcon(this->hInstance, MAKEINTRESOURCE(IDI_MEANINGLESSSOFTWARE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = this->ClassName();
	wcex.hIconSm = LoadIcon(this->hInstance, MAKEINTRESOURCE(IDI_SMALL));
	if (RegisterClassExW(&wcex) == 0)
	{
		return FALSE;
	}

	this->hWnd = CreateWindowEx(
		dwExStyle, ClassName(), WindowName(), dwStyle, x, y,
		nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
	);

	return (this->hWnd ? TRUE : FALSE);
}
#endif