#include "stdafx.h"
#include "WindowsContextMenu.h"
#include <Windows.h>
#include <commctrl.h>
#include <Shlobj.h>

QMap<HWND, IContextMenu2*> g_context;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_INITMENUPOPUP) {
		IContextMenu2* pContextMenu2 = g_context.find(hwnd).value();
		if (pContextMenu2 != NULL) {
			pContextMenu2->HandleMenuMsg(uMsg, wParam, lParam);
			return 0;
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void showWindowsContext(const QString path, const QPoint* pos)
{
	QString rpath = path;
	rpath.replace(u8"/", u8"\\");
	PIDLIST_ABSOLUTE pidlAbsolute = ILCreateFromPath(reinterpret_cast<LPCWSTR>(rpath.utf16()));

	static QString cname;
	if (cname.isEmpty()) {
		cname = u8"TableEngineWindowsContext";
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = 0;
		wc.lpfnWndProc = WindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = static_cast<HINSTANCE>(GetModuleHandle(0));
		wc.hCursor = 0;
		wc.hbrBackground = 0;
		wc.hIcon = 0;
		wc.hIconSm = 0;
		wc.lpszMenuName = 0;
		wc.lpszClassName = reinterpret_cast<LPCWSTR>(cname.utf16());
		RegisterClassEx(&wc);
	}

	HWND hwnd = CreateWindowEx(0, reinterpret_cast<LPCWSTR>(cname.utf16()),
		L"ContextWindow", WS_OVERLAPPED,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		HWND_MESSAGE, NULL, static_cast<HINSTANCE>(GetModuleHandle(0)), NULL);


	int                 nId;
	HRESULT             hr;
	POINT               pt;
	HMENU               hmenuPopup;
	IContextMenu        *pContextMenu = NULL;
	IShellFolder        *pShellFolder = NULL;
	PITEMID_CHILD       pidlChild;

	SHBindToParent(pidlAbsolute, IID_PPV_ARGS(&pShellFolder), NULL);
	pidlChild = ILFindLastID(pidlAbsolute);

	hr = pShellFolder->GetUIObjectOf(NULL, 1, (LPCITEMIDLIST *)&pidlChild, IID_IContextMenu, NULL, (void **)&pContextMenu);
	IContextMenu2 *pContextMenu2 = NULL;
	if (hr == S_OK) {
		hr = pContextMenu->QueryInterface(IID_PPV_ARGS(&pContextMenu2));
		pContextMenu->Release();
		g_context.insert(hwnd, pContextMenu2);
	}

	if (hr == S_OK) {
		hmenuPopup = CreatePopupMenu();
		pContextMenu2->QueryContextMenu(hmenuPopup, 0, 1, 0x7fff, CMF_NORMAL);

		if (pos == nullptr)
		{
			GetCursorPos(&pt);
		}
		else
		{
			pt.x = pos->x();
			pt.y = pos->y();
		}
		nId = TrackPopupMenu(hmenuPopup, TPM_RETURNCMD, pt.x, pt.y, 0, hwnd, NULL);
	}

	if (nId != 0) {
		CMINVOKECOMMANDINFO ici;
		ici.cbSize = sizeof(CMINVOKECOMMANDINFO);
		ici.fMask = 0;
		ici.hwnd = hwnd;
		ici.lpVerb = (LPCSTR)MAKEINTRESOURCE(nId - 1);
		ici.lpParameters = NULL;
		ici.lpDirectory = NULL;
		ici.nShow = SW_SHOW;

		hr = pContextMenu2->InvokeCommand(&ici);
	}

	if (pContextMenu2 != NULL) {
		g_context.erase(g_context.find(hwnd));
		pContextMenu2->Release();
		pContextMenu2 = NULL;
	}
	if (pShellFolder != NULL)pShellFolder->Release();

	DestroyWindow(hwnd);
	ILFree(pidlAbsolute);
}