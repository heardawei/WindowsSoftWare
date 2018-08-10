//#ifndef UNICODE
//#define UNICODE
//#endif 
//
//#include <windows.h>
//#include <new>
//#include <cassert>
//
//class MyClass
//{
//public:
//private:
//};
//
//inline MyClass* GetAppState(HWND hwnd)
//{
//	LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
//	MyClass *pState = reinterpret_cast<MyClass*>(ptr);
//	return pState;
//}
//LRESULT CALLBACK main_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	MyClass *pState;
//	if (uMsg == WM_CREATE)
//	{
//		CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
//		pState = reinterpret_cast<MyClass *>(pCreate->lpCreateParams);
//		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pState);
//	}
//	else
//	{
//		pState = GetAppState(hwnd);
//	}
//
//	switch (uMsg)
//	{
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return 0;
//	case WM_CLOSE:
//		if (MessageBox(hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
//		{
//			DestroyWindow(hwnd);
//		}
//		return 0;
//
//	case WM_PAINT:
//	{
//		PAINTSTRUCT ps;
//		HDC hdc = BeginPaint(hwnd, &ps);
//		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
//
//
//		EndPaint(hwnd, &ps);
//	}
//	return 0;
//
//	}
//	return DefWindowProc(hwnd, uMsg, wParam, lParam);
//}
//
//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
//{
//	PCWCHAR main_class_name = L"Hello World Main Class";
//	PCWCHAR main_window_title = L"Hello World Main Window";
//
//	WNDCLASS cls = {};
//	cls.lpfnWndProc = main_window_proc;
//	cls.hInstance = hInstance;
//	cls.lpszClassName = main_class_name;
//
//	RegisterClass(&cls);
//
//	HWND hwnd = CreateWindowEx(0, main_class_name, main_window_title, 
//		WS_OVERLAPPEDWINDOW,
//		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
//		NULL, NULL, hInstance, new (std::nothrow) MyClass);
//
//	if (hwnd == NULL)
//	{
//		return 0;
//	}
//
//	ShowWindow(hwnd, nCmdShow);
//
//	MSG msg = {};
//	while (GetMessage(&msg, NULL, 0, 0))
//	{
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}
//
//	return 0;
//}


#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <new>
#include <cassert>

class BaseClass
{
public:
	BaseClass();
	static LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL Create(DWORD dwStyle, DWORD exStyle = 0, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int nWidth = CW_USEDEFAULT, int Height = CW_USEDEFAULT, HWND hWndParent = 0, HMENU hMenu = 0);
	HWND window();
protected:
	virtual LRESULT HandleMassage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual PCWSTR ClassName();
	virtual PCWSTR TitleName();
private:
	HWND hwnd;
};


BaseClass::BaseClass() :
	hwnd(NULL)
{

}

HWND BaseClass::window()
{
	return this->hwnd;
}

BOOL BaseClass::Create(DWORD dwStyle, DWORD exStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu)
{
	WNDCLASS cls = {};
	cls.lpfnWndProc = BaseClass::MainWindowProc;
	cls.hInstance = GetModuleHandle(NULL);
	cls.lpszClassName = this->ClassName();

	RegisterClass(&cls);

	HWND hwnd = CreateWindowEx(0, this->ClassName(), this->TitleName(),
		WS_OVERLAPPEDWINDOW,
		x, y, nWidth, nHeight,
		hWndParent, hMenu, GetModuleHandle(NULL), this);

	if (hwnd)
	{
		this->hwnd = hwnd;
		return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK BaseClass::MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BaseClass *thiz = NULL;
	if (uMsg == WM_CREATE)
	{
		CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
		thiz = reinterpret_cast<BaseClass *>(pCreate->lpCreateParams);
		thiz->hwnd = hwnd;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)thiz);
	}
	else
	{
		thiz = reinterpret_cast<BaseClass*> (GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (thiz)
	{
		return thiz->HandleMassage(uMsg, wParam, lParam);
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/* ----------------------------------- virtual implement ----------------------------------------- */

PCWSTR BaseClass::ClassName()
{
	return L"BaseClass";
}

PCWSTR BaseClass::TitleName()
{
	return L"BaseTitle";
}

LRESULT BaseClass::HandleMassage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		if (MessageBox(this->hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(this->hwnd);
		}
		return 0;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(this->hwnd, &ps);
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			EndPaint(this->hwnd, &ps);
		}
		return 0;
	}
	return DefWindowProc(this->hwnd, uMsg, wParam, lParam);
}
/* ----------------------------------- virtual implement end ----------------------------------------- */

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	PCWCHAR pClassName = L"Hello World Main Class";
	PCWCHAR pMainWindowTitle = L"Hello World Main Window";

	BaseClass win;
	if (!win.Create(WS_EX_OVERLAPPEDWINDOW))
	{
		return 0;
	}
	ShowWindow(win.window(), nCmdShow);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
