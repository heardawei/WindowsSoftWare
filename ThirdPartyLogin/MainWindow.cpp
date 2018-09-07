
#include "stdafx.h"
#include "Resource.h"
#include "MainWindow.h"
#include "ThirdPartyLogin.h"


MainWindow::MainWindow(HINSTANCE hInstance)
{
	this->hInstance = hInstance;
	if (LoadStringW(hInstance, IDS_MainWindow_TITLE, this->szTitle, sizeof(this->szTitle)) == 0)
	{
		/* TODO: handle error */
	}
	if (LoadStringW(hInstance, IDC_THIRDPARTYLOGIN, this->szWindowClass, sizeof(this->szWindowClass)) == 0)
	{
		/* TODO: handle error */
	}
}

MainWindow::~MainWindow()
{
}

