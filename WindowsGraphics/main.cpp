#ifndef UNICODE
#define UNICODE
#endif /* UNICODE */

#ifndef _UNICODE
#define _UNICODE
#endif /* _UNICODE */


#include <windows.h>
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

class MainWindow : public BaseWindow<MainWindow>
{
	ID2D1Factory            *pFactory;
	ID2D1HwndRenderTarget   *pRenderTarget;
	ID2D1SolidColorBrush    *pBrush;
	D2D1_ELLIPSE            ellipse;

	void    CalculateLayout();
	HRESULT CreateGraphicsResources();
	void    DiscardGraphicsResources();
	void    OnPaint();
	void    Resize();

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
		ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
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
		this->pRenderTarget->FillEllipse(this->ellipse, this->pBrush);

		hr = this->pRenderTarget->EndDraw();
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
		{
			/* if a device becomes invalid, release device-dependent resource */
			this->DiscardGraphicsResources();
		}
		EndPaint(m_hwnd, &ps);
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
	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}