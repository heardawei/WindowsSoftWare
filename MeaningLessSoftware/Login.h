#pragma once

#include "BaseWindow.h"

#define USE_SELF_CREATE 0

class LoginWindow : public BaseWindow<LoginWindow>
{
public:
	LoginWindow();
	~LoginWindow();
#if USE_SELF_CREATE
	virtual BOOL Create(DWORD dwStyle = SW_SHOWNORMAL,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0, HMENU hMenu = 0);
#endif
	virtual PCWSTR ClassName() const;
	virtual PCWSTR WindowName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE hInstance;
	HWND hWndPushButton;
};
