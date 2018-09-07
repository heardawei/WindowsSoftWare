#pragma once

#ifndef UNICODE
#define UNICODE
#endif /* UNICODE */

#ifndef _UNICODE
#define _UNICODE
#endif /* _UNICODE */

#include <windows.h>


template <class T>
class BaseWindow
{
public:
	BaseWindow(HINSTANCE hInstance = NULL);
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual BOOL Create(DWORD dwStyle = WS_OVERLAPPEDWINDOW,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0, HMENU hMenu = 0);
	HWND Window() const;

protected:

	virtual PCWSTR ClassName() const = 0;
	virtual PCWSTR WindowName() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND hWnd;
	HINSTANCE hInstance;
};

/* ------------------------------ Template Implement ------------------------------ */

template <class T>
LRESULT BaseWindow<T>::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	T *pThis = NULL;

	if (uMsg == WM_NCCREATE)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		pThis = (T *)pCreate->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

		pThis->hWnd = hwnd;
	}
	else
	{
		pThis = (T *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	}
	if (pThis)
	{
		return pThis->HandleMessage(uMsg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

template <class T>
BaseWindow<T>::BaseWindow(HINSTANCE hInstance) :
	hWnd(NULL), hInstance(hInstance ? hInstance : GetModuleHandle(NULL))
{
}

template <class T>
BOOL BaseWindow<T>::Create(DWORD dwStyle, DWORD dwExStyle,
	int x, int y, int nWidth, int nHeight,
	HWND hWndParent, HMENU hMenu)
{
	WNDCLASS wc = { 0 };

	wc.style = CS_DBLCLKS;		/* enable double clicks msg */
	wc.lpfnWndProc = T::WindowProc;
	wc.hInstance = this->hInstance;
	wc.lpszClassName = ClassName();

	RegisterClass(&wc);

	hWnd = CreateWindowEx(
		dwExStyle, ClassName(), WindowName(), dwStyle, x, y,
		nWidth, nHeight, hWndParent, hMenu, this->hInstance, this
	);

	return (hWnd ? TRUE : FALSE);
}

template <class T>
HWND BaseWindow<T>::Window() const
{
	return hWnd;
}