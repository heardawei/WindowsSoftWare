#pragma once


#include "BaseWindow.h"
#include "Login.h"
#include "MeaningLessSoftware.h"

class MainWindow : public BaseWindow<MainWindow>
{
public:
	MainWindow(HINSTANCE hInstance);
	~MainWindow();

	virtual BOOL Create(DWORD dwStyle = SW_SHOWNORMAL,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0, HMENU hMenu = 0);
	virtual PCWSTR ClassName() const;
	virtual PCWSTR WindowName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL CreateLoginWindow();

	WCHAR szTitle[MAX_PRESENT_STRING];
	WCHAR szWindowClass[MAX_PRESENT_STRING];

	HWND hWndLoginButton;
	LoginWindow *pLoginWin;
};