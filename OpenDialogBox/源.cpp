
#ifndef UNICODE
#define UNICODE
#endif /* UNICODE */

#ifndef _UNICODE
#define _UNICODE
#endif /* _UNICODE */

#include <Windows.h>
#include <shobjidl.h>


#if 0
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr))
	{
		return -1;
	}

	IFileOpenDialog *pFileOpenDialog;

	// Create the FileOpenDialog object.
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
		IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpenDialog));

	if (SUCCEEDED(hr))
	{
		// Show the Open dialog box.
		hr = pFileOpenDialog->Show(NULL);

		// Get the file name from the dialog box.
		if (SUCCEEDED(hr))
		{
			IShellItem *pItem;
			hr = pFileOpenDialog->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{
				LPWSTR pszFilePath;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

				// Display the file name to the user.
				if (SUCCEEDED(hr))
				{
					MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
					CoTaskMemFree(pszFilePath);
				}
				else
				{
					MessageBox(NULL, L"Get File Path Failed", L"File Path", MB_RETRYCANCEL);
				}
				pItem->Release();
			}
		}
		pFileOpenDialog->Release();
	}
	CoUninitialize();

	return 0;
}

#elif 0

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	HRESULT res = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(res))
	{
		return -1;
	}

	IFileOpenDialog *pFileOpenDialog;
	res = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
		IID_IFileOpenDialog, reinterpret_cast<void **>(&pFileOpenDialog));
	if (SUCCEEDED(res))
	{
		res = pFileOpenDialog->Show(NULL);
		if (SUCCEEDED(res))
		{
			IShellItem *pItem;
			res = pFileOpenDialog->GetResult(&pItem);
			if (SUCCEEDED(res))
			{
				LPWSTR pFileName;
				res = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pFileName);
				if (SUCCEEDED(res))
				{
					MessageBox(NULL, pFileName, L"You Select File Name", MB_OK);
					CoTaskMemFree(pFileName);
				}
				else
				{
					MessageBox(NULL, L"You Select Failed", L"You Select Failed", MB_OK);
				}
			}
			pItem->Release();
		}
		pFileOpenDialog->Release();
	}


	CoUninitialize();
	return 0;
}
#elif 1

#include <atlbase.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	LRESULT res = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(res))
	{
		return 0;
	}

	CComPtr<IFileOpenDialog> pFileOpenDialog;
	res = pFileOpenDialog.CoCreateInstance(__uuidof(FileOpenDialog));
	if (SUCCEEDED(res))
	{
		res = pFileOpenDialog->Show(NULL);
		if (SUCCEEDED(res))
		{
			CComPtr<IShellItem> pItem;
			res = pFileOpenDialog->GetResult(&pItem);
			if (SUCCEEDED(res))
			{
				LPWSTR pFileName = NULL;
				res = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pFileName);
				if (SUCCEEDED(res))
				{
					MessageBox(NULL, pFileName, L"OK", MB_OK);
					CoTaskMemFree(pFileName);
				}
				else
				{
					MessageBox(NULL, L"Get Display Name Failed", L"Error", MB_OK);
				}
			}
			else
			{
				MessageBox(NULL, L"Get Select Item Failed", L"Error", MB_OK);
			}
		}
		else
		{
			MessageBox(NULL, L"You Close Select Window", L"Info", MB_OK);
		}
	}

	CoUninitialize();
	return 0;
}

#elif 0
#include <atlbase.h> // Contains the declaration of CComPtr.

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		CComPtr<IFileOpenDialog> pFileOpen;

		// Create the FileOpenDialog object.
		hr = pFileOpen.CoCreateInstance(__uuidof(FileOpenDialog));
		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			hr = pFileOpen->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				CComPtr<IShellItem> pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
						CoTaskMemFree(pszFilePath);
					}
				}

				// pItem goes out of scope.
			}

			// pFileOpen goes out of scope.
		}
		CoUninitialize();
	}
	return 0;
}
#endif 