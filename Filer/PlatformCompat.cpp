#include "stdafx.h"
#include "PlatformCompat.h"
#include <ShObjIdl.h>
#include <ShlObj_Core.h>

int PlatformCompat::ExecuteAssociatedApp(QString itemPath)
{
#ifdef Q_OS_WIN
	//Modifierありの場合はOSの関連付けでアプリ起動
	LPCWSTR lpFile = reinterpret_cast<LPCWSTR>(itemPath.replace('/', '\\').utf16());
	SHELLEXECUTEINFOW sei = {
		sizeof(sei),           // cbSize;
		0,                     // fMask
		NULL,                  // hwnd
		NULL,                  // lpVerb
		lpFile,                // lpFile
		NULL,				   // lpParameters
		NULL,                  // lpDirectory
		SW_SHOWNORMAL,         // nShow
		0,                     // hInstApp
		NULL,                  // lpIDList
		NULL,                  // lpClass
		NULL,                  // hkeyClass
		0,                     // dwHotKey
		NULL,                  // hIcon
		NULL                   // hProcess
	};

	if (!ShellExecuteEx(&sei))
	{
		HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
		qDebug() << "ShellExecuteEx had error: " << hr;
		return hr;
	}
#endif
	return 0;
}

/// <summary>
/// 
/// </summary>
/// <param name="itemPathList"></param>
/// <returns></returns>
int PlatformCompat::MoveToTrash(QList<QString> itemPathList)
{
	if (itemPathList.size() == 0)
		return -1;

	HRESULT hr;

	/*

	DeleteItemsをつかうため色々調査実装中。
	サンプルはVscodeにて

	*/
	int itemNum = itemPathList.size();
	PCIDLIST_ABSOLUTE_ARRAY pIdlArray = new LPCITEMIDLIST[itemNum];
	PIDLIST_ABSOLUTE abSourcePidl;
	SFGAOF attrs;
	for (int i = 0; i < itemNum; ++i)
	{
		LPCWSTR cstr = reinterpret_cast<LPCWSTR>(itemPathList[i].replace('/', '\\').utf16());
		hr = SHParseDisplayName(cstr, NULL, &abSourcePidl, 0, &attrs);
		if (FAILED(hr))
			return hr;
		pIdlArray[i] = abSourcePidl;
	}


	//
	// Initialize COM as STA.
	//
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOperation* pfo;

		//
		// Create the IFileOperation interface 
		//
		hr = CoCreateInstance(CLSID_FileOperation,
			NULL,
			CLSCTX_ALL,
			IID_PPV_ARGS(&pfo));
		if (SUCCEEDED(hr))
		{
			hr = pfo->SetOperationFlags(FOF_ALLOWUNDO);
			if (SUCCEEDED(hr))
			{
				IShellItemArray* pShellItemArr = NULL;
				hr = SHCreateShellItemArrayFromIDLists(itemNum, pIdlArray, &pShellItemArr);
				if (SUCCEEDED(hr))
				{
					hr = pfo->DeleteItems(pShellItemArr);
					if (SUCCEEDED(hr))
					{
						hr = pfo->PerformOperations();
					}
				}
				pShellItemArr->Release();
			}
			pfo->Release();
		}

		CoUninitialize();
	}

	delete[] pIdlArray;

	return hr;
}
