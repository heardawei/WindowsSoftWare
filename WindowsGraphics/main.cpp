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
#pragma comment(lib, "d2d1")

#include "basewin.h"

template <class T>
void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

BOOL ellipseContainsPoint(const D2D1_ELLIPSE &ellipse, const D2D1_POINT_2F &point)
{
	if (pow(point.x - ellipse.point.x, 2) / pow(ellipse.radiusX, 2) + pow(point.y - ellipse.point.y, 2) / pow(ellipse.radiusY, 2) <= 1)
	{
		return TRUE;
	}
	return FALSE;
}

class DPIScale
{
public:
	static void Initialize(ID2D1Factory *pFactory)
	{
		if (pFactory)
		{
			FLOAT dpiX, dpiY;
			pFactory->GetDesktopDpi(&dpiX, &dpiY);
			DPIScale::scaleX = dpiX / 96.0f;
			DPIScale::scaleY = dpiY / 96.0f;
		}
	}

	template <typename T>
	static D2D1_POINT_2F PixelsToDips(T x, T y)
	{
		return D2D1::Point2F(static_cast<float>(x) / DPIScale::scaleX, static_cast<float>(y) / DPIScale::scaleY);
	}

public:
	/* NOTE: static member constant must be init out of this Class !!! */
	static FLOAT scaleX;
	static FLOAT scaleY;
};
FLOAT DPIScale::scaleX = 1;
FLOAT DPIScale::scaleY = 1;


class MainWindow : public BaseWindow<MainWindow>
{
	ID2D1Factory            *pFactory;
	ID2D1HwndRenderTarget   *pRenderTarget;
	ID2D1SolidColorBrush    *pBrush;
	D2D1_ELLIPSE            ellipse;
	std::list<D2D1_ELLIPSE> listDrawEllipse;
	D2D1_POINT_2F           ptLbuttonDown;

	void    Resize();
	void    CalculateLayout();
	HRESULT CreateGraphicsResources();
	void    DiscardGraphicsResources();
	void    OnPaint();
	void	OnLButtonDown(int pixelX, int pixelY, DWORD flags);
	void	OnMouseMove(int pixelX, int pixelY, DWORD flags);
	void	OnLButtonUp();
	void	OnRButtonDown(int pixelX, int pixelY);

public:

	MainWindow() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL)
	{
	}

	PCWSTR  ClassName() const
	{
		return L"Circle Window Class";
	}
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

/* Recalculate drawing layout when the size of the window changes. */

void MainWindow::CalculateLayout()
{
	if (this->pRenderTarget != NULL)
	{
		D2D1_SIZE_F size = this->pRenderTarget->GetSize();
		const float x = size.width / 2;
		const float y = size.height / 2;
		const float radius = min(x, y);
		// this->ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
		/*this->ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), x, y);*/
	}
}

HRESULT MainWindow::CreateGraphicsResources()
{
	HRESULT hr = S_OK;
	if (this->pRenderTarget == NULL)
	{
		RECT rc;
		GetClientRect(this->m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = this->pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(this->m_hwnd, size),
			&this->pRenderTarget);

		if (SUCCEEDED(hr))
		{
			const D2D1_COLOR_F white_color = D2D1::ColorF(1.0f, 1.0f, 1.0f);
			hr = this->pRenderTarget->CreateSolidColorBrush(white_color, &this->pBrush);

			if (SUCCEEDED(hr))
			{
				this->CalculateLayout();
			}
		}
	}
	return hr;
}

void MainWindow::DiscardGraphicsResources()
{
	SafeRelease(&this->pRenderTarget);
	SafeRelease(&this->pBrush);
}

void MainWindow::OnPaint()
{
	/* ensure device-dependent resource is valid before draw */
	HRESULT hr = this->CreateGraphicsResources();
	if (SUCCEEDED(hr))
	{
		PAINTSTRUCT ps;
		BeginPaint(m_hwnd, &ps);

		this->pRenderTarget->BeginDraw();

		D2D1_COLOR_F blue_color = D2D1::ColorF(D2D1::ColorF::SkyBlue);
		this->pRenderTarget->Clear(blue_color);
		for (std::list<D2D1_ELLIPSE>::const_iterator itr = this->listDrawEllipse.cbegin();
			itr != this->listDrawEllipse.cend(); ++itr)
		{
			this->pRenderTarget->DrawEllipse(&(*itr), this->pBrush, 1, 0);
			this->pRenderTarget->FillEllipse(*itr, this->pBrush);
		}
		this->pRenderTarget->DrawEllipse(this->ellipse, this->pBrush, 1, 0);
		this->pRenderTarget->FillEllipse(this->ellipse, this->pBrush);

#if 1
		char cont[256];
		snprintf(cont, sizeof(cont),
			"%s----------------------nEllipse:%d----------------------\r\n",
			__func__, this->listDrawEllipse.size());
		std::ofstream ofs(L"log.txt", std::ofstream::app);
		ofs.write(cont, strlen(cont));
		ofs.close();
#endif
		hr = this->pRenderTarget->EndDraw();
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
		{
			/* if a device becomes invalid, release device-dependent resource */
			this->DiscardGraphicsResources();
		}
		EndPaint(m_hwnd, &ps);
	}
}

void MainWindow::OnLButtonDown(int pixelX, int pixelY, DWORD flags)
{
	SetCapture(m_hwnd);
	this->ellipse.point = this->ptLbuttonDown = DPIScale::PixelsToDips(pixelX, pixelY);
	this->ellipse.radiusX = this->ellipse.radiusY = 1.0f;
	InvalidateRect(m_hwnd, NULL, FALSE);
}

void MainWindow::OnMouseMove(int pixelX, int pixelY, DWORD flags)
{
	if (flags & MK_LBUTTON)
	{
		const D2D1_POINT_2F dips = DPIScale::PixelsToDips(pixelX, pixelY);

		const float width = (dips.x - this->ptLbuttonDown.x) / 2;
		const float height = (dips.y - this->ptLbuttonDown.y) / 2;
		const float x1 = this->ptLbuttonDown.x + width;
		const float y1 = this->ptLbuttonDown.y + height;

		this->ellipse = D2D1::Ellipse(D2D1::Point2F(x1, y1), width, height);

		/* repaint client area */
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

void MainWindow::OnLButtonUp()
{
	this->listDrawEllipse.push_back(this->ellipse);
	InvalidateRect(m_hwnd, NULL, FALSE);
	ReleaseCapture();
}

void MainWindow::OnRButtonDown(int pixelX, int pixelY)
{
	/* first clear temp ellipse */
	this->ellipse = D2D1::Ellipse(D2D1::Point2F(0, 0), 0, 0);
	const D2D1_POINT_2F dips = DPIScale::PixelsToDips(pixelX, pixelY);

	for (std::list<D2D1_ELLIPSE>::iterator itr = this->listDrawEllipse.begin();
		itr != this->listDrawEllipse.end(); ++itr)
	{
		if (ellipseContainsPoint(*itr, dips))
		{
			this->listDrawEllipse.erase(itr);
			InvalidateRect(m_hwnd, NULL, FALSE);
			break;
		}
	}
}

void MainWindow::Resize()
{
	if (this->pRenderTarget != NULL)
	{
		RECT rc;
		GetClientRect(this->m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		this->pRenderTarget->Resize(size);
		this->CalculateLayout();
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
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

	return 0;
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		if (FAILED(D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED, &this->pFactory)))
		{
			return -1;  // Fail CreateWindowEx.
		}
		DPIScale::Initialize(this->pFactory);
		return 0;

	case WM_DESTROY:
		this->DiscardGraphicsResources();
		SafeRelease(&this->pFactory);
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		this->OnPaint();
		return 0;
	case WM_SIZE:
		this->Resize();
		return 0;

	case WM_LBUTTONDOWN:
		OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;
	case WM_LBUTTONUP:
		OnLButtonUp();
		return 0;
	case WM_MOUSEMOVE:
		OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;

	case WM_RBUTTONDOWN:
		OnRButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;


	case WM_MBUTTONDOWN:
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		wchar_t cont[256];
		_snwprintf_s(cont, sizeof(cont), L"X:%d, Y:%d", xPos, yPos);

		if (wParam & MK_CONTROL)
		{
			MessageBox(NULL, cont, L"Control + RButton", MB_OK);
		}
		else if (wParam & MK_SHIFT)
		{
			MessageBox(NULL, cont, L"Shift + RButton", MB_OK);
		}
		else
		{
			MessageBox(NULL, cont, L"RButton", MB_OK);
		}
		return 0;
	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}