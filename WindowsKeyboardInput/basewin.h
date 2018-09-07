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
	BaseWindow();

	BOOL Create(PCWSTR lpWindowName, DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0, HMENU hMenu = 0);
	HWND Window() const;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:

	virtual PCWSTR  ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND m_hwnd;
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

		pThis->m_hwnd = hwnd;
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
BaseWindow<T>::BaseWindow() :
	m_hwnd(NULL)
{

}

template <class T>
BOOL BaseWindow<T>::Create(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle,
	int x, int y, int nWidth, int nHeight,
	HWND hWndParent, HMENU hMenu)
{
	WNDCLASS wc = { 0 };

	wc.style = CS_DBLCLKS;		/* enable double clicks msg */
	wc.lpfnWndProc = T::WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = ClassName();

	RegisterClass(&wc);

	m_hwnd = CreateWindowEx(
		dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
		nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
	);

	return (m_hwnd ? TRUE : FALSE);
}

template <class T>
HWND BaseWindow<T>::Window() const
{
	return m_hwnd;
}