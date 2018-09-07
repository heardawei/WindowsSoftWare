
#include "stdafx.h"
#include "Resource.h"
#include "MainWindow.h"
#include "Login.h"

static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

MainWindow::MainWindow(HINSTANCE hInstance) :
	BaseWindow(hInstance)
{
if (LoadStringW(hInstance, IDS_APP_TITLE, this->szTitle, sizeof(this->szTitle)) == 0)
{
	/* TODO: handle error */
}
if (LoadStringW(hInstance, IDC_MEANINGLESSSOFTWARE, this->szWindowClass, sizeof(this->szWindowClass)) == 0)
{
	/* TODO: handle error */
}
}

MainWindow::~MainWindow()
{
	if (this->pLoginWin)
	{
		delete this->pLoginWin;
	}
}

BOOL MainWindow::Create(DWORD dwStyle, DWORD dwExStyle,
	int x, int y, int nWidth, int nHeight,
	HWND hWndParent, HMENU hMenu)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	/* Main Window Class */
	wcex.style = /*CS_HREDRAW | CS_VREDRAW | */CS_DBLCLKS;
	wcex.lpfnWndProc = BaseWindow::WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = this->hInstance;
	wcex.hIcon = LoadIcon(this->hInstance, MAKEINTRESOURCE(IDI_MEANINGLESSSOFTWARE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MEANINGLESSSOFTWARE);
	wcex.lpszClassName = szWindowClass;
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

PCWSTR MainWindow::ClassName() const
{
	return this->szWindowClass;
}

PCWSTR MainWindow::WindowName() const
{
	return this->szTitle;
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		return this->OnCreate(uMsg, wParam, lParam);
	case WM_COMMAND:
		return this->OnCommand(uMsg, wParam, lParam);
	case WM_PAINT:
		return this->OnPaint(uMsg, wParam, lParam);
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(this->hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

LRESULT MainWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/* Create Button */
	this->hWndLoginButton = CreateWindow(L"Button", L"使用QQ登录", WS_VISIBLE | WS_TABSTOP | WS_CHILD | BS_PUSHBUTTON,
		35, 10, 160, 60, this->hWnd, (HMENU)IDC_CREATE_LOGIN_WINDOW, this->hInstance, NULL);

	return 0;
	//	return DefWindowProc(this->hWnd, uMsg, wParam, lParam);
}

BOOL MainWindow::CreateLoginWindow()
{
	if (this->pLoginWin)
	{
		MessageBox(this->hWnd, L"Login Window has been created, please log in!", L"提醒", MB_OK);
		return TRUE;
	}

	if ((this->pLoginWin = new LoginWindow) == NULL)
	{
		/* TODO: handle error */
		MessageBox(this->hWnd, L"Login Window has been created, please log in!", L"提醒", MB_OK);
		return FALSE;
	}

	BOOL boolRet = FALSE;
	RECT rect;

	boolRet = GetClientRect(this->hWnd, &rect);
	if (boolRet == FALSE)
	{
		/* TODO: handle error */
		MessageBox(this->hWnd, L"GetClientRect failed!", L"提醒", MB_OK);
		return FALSE;
	}

	/* Create Login-Window */
	boolRet = this->pLoginWin->Create(WS_CHILD | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0,
		
		(rect.right - rect.left) / 4, 0, (rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2,
		this->hWnd, 0);
	if (boolRet == FALSE)
	{
		/* TODO: handle error */
		MessageBox(this->hWnd, L"Create failed!", L"提醒", MB_OK);
		return FALSE;
	}

	/* Show Login-Window */
	ShowWindow(this->pLoginWin->Window(), SW_SHOWNORMAL);
	return TRUE;
}

LRESULT MainWindow::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
	HWND selfHwnd = (HWND)lParam;
	// 分析菜单选择: 
	switch (wmId)
	{
	case IDC_CREATE_LOGIN_WINDOW:
		if (this->CreateLoginWindow())
		{
			/* disable login-button util the login-window close */
			SendMessage(selfHwnd, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"正在登录");
			EnableWindow(selfHwnd, FALSE);
		}
		break;
	case IDM_ABOUT:
		DialogBox(this->hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), this->hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(this->hWnd);
		break;
	default:
		return DefWindowProc(this->hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

LRESULT MainWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(this->hWnd, &ps);
	// TextOut(hdc, 10, 10, L"Hello I'm Textout()", wcslen(L"Hello I'm Textout()"));
	// TODO: 在此处添加使用 hdc 的任何绘图代码...
	EndPaint(this->hWnd, &ps);
	return 0;
}

static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}