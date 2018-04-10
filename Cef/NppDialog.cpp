//this file is part of notepad++
//Copyright (C)2003 Don HO ( donho@altern.org )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "NppDialog.h"
#include "PluginDefinition.h"
#include "ClientHandler.h"
#include "windows.h"

extern NppData nppData;

BOOL CALLBACK NppDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	int i = 0;
	switch (message) 
	{
		case WM_NOTIFY:
		{
			LPNMHDR	pnmh = (LPNMHDR)lParam;

			if (pnmh->hwndFrom == _hParent)
			{
				switch (LOWORD(pnmh->code))
				{
					case DMN_CLOSE:
					{
						//SimpleHandler::GetInstance()->CloseAllBrowsers(false);
						_showFlag = 0;
						break;
					}
					case DMN_FLOAT:
					{
						_isFloating = true;
						break;
					}
					case DMN_DOCK:
					{
						_isFloating = false;
						break;
					}
					default:
						break;
				}
			}
			break;
		}

		case WM_COMMAND : 
		{
			switch (wParam)
			{
				case IDOK :
				{
					int line = getLine();
					if (line != -1)
					{
						// Get the current scintilla
						int which = -1;
						::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
						if (which == -1)
							return FALSE;
						HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

						::SendMessage(curScintilla, SCI_ENSUREVISIBLE, line-1, 0);
						::SendMessage(curScintilla, SCI_GOTOLINE, line-1, 0);
					}
					return TRUE;
				}
			}
				return FALSE;
		}

		case WM_INITDIALOG:
		{
			CreateBrowser();
			break;
		}

		case WM_SIZE: //改变窗口消息
		{
			// extract size info
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);

			if (SimpleHandler::GetInstance())
			{
				SimpleHandler::GetInstance()->OnSize(width,height);
			}
			//if (_browser)
			//{
			//	CefWindowHandle hwnd = _browser->GetHost()->GetWindowHandle();
			//	::MoveWindow(hwnd, 0, 0, width, height, TRUE);
			//}

			break;
		}
		case WM_CLOSE:
		{
			i++;
			break;
		}

		case WM_DESTROY:
		{
			CleanUp();
			break;
		}
		default :
			return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
	}
}

void NppDialog::CleanUp()
{
	if (_flag)
	{
		CefShutdown();
	}
}

void NppDialog::Shutdown()
{
	if (_flag)
	{
		SimpleHandler::GetInstance()->CloseAllBrowsers(false);
	}
}


void NppDialog::CreateBrowser()
{
	CefRefPtr<SimpleHandler> handler(new SimpleHandler(false));
	CefWindowInfo window_info;
	RECT rect,rectnew;

	GetClientRect(this->_hSelf,&rect);
	rectnew = rect;
	rectnew.top = rect.top + 94;
	rectnew.bottom = rect.bottom;
	rectnew.left = rect.left;
	rectnew.right = rect.right;

	//GetWindowRect(this->_hSelf, &rect);
	window_info.SetAsChild(this->_hSelf, rect);
	CefBrowserSettings settings;
	//CefBrowserHost::CreateBrowser(window_info, handler, "www.baidu.com", settings, NULL);
	CefString url = _path + L"\\example.html";
	CefBrowserHost::CreateBrowser(window_info, handler, url, settings, NULL);

	//CefRefPtr<SimpleHandler> handler(new SimpleHandler(false));
	//CefBrowserSettings browser_settings;
	//std::string url = "file://D:/Program Files/Notepad++/plugins/cef/example.html";
	//CefWindowInfo window_info;
	//window_info.SetAsWindowless(this->_hSelf);

	//CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings,NULL);

	return;
}


void NppDialog::InitCef()
{
	if (_flag)
	{
		return;
	}
	_flag = 1;
	HINSTANCE hInstance = _hInst;

	// Enable High-DPI support on Windows 7 or newer.
	CefEnableHighDPISupport();

	void* sandbox_info = NULL;

#if defined(CEF_USE_SANDBOX)
	// Manage the life span of the sandbox information object. This is necessary
	// for sandbox support on Windows. See cef_sandbox_win.h for complete details.
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
#endif

	// Provide CEF with command-line arguments.
	CefMainArgs main_args(hInstance);


	// CEF applications have multiple sub-processes (render, plugin, GPU, etc)
	// that share the same executable. This function checks the command-line and,
	// if this is a sub-process, executes the appropriate logic.
	//int exit_code = CefExecuteProcess(main_args, NULL, sandbox_info);
	//if (exit_code >= 0) {
	//	// The sub-process has completed so return here.
	//	return;
	//}

	// Optional implementation of the CefApp interface.
	CefRefPtr<NppCefApp> app(new NppCefApp);

	// Populate this structure to customize CEF behavior.
	CefSettings settings;

#if !defined(CEF_USE_SANDBOX)
	settings.no_sandbox = true;
#endif

	settings.multi_threaded_message_loop = 1;

	CefString(&settings.browser_subprocess_path).FromWString(_path + L"\\sub_process.exe");
	//CefString(&settings.browser_subprocess_path).FromASCII("C:/Program Files/Notepad++/plugins/cef/sub_process.exe");

	// Initialize CEF in the main process.
	CefInitialize(main_args, settings, app.get(), sandbox_info);



}
