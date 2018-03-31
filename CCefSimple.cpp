
#include "CCefSimple.h"
#include "include/cef_sandbox_win.h"
#include "simple_app.h"

#if defined(CEF_USE_SANDBOX)
// The cef_sandbox.lib static library is currently built with VS2013. It may not
// link successfully with other VS versions.
#pragma comment(lib, "cef_sandbox.lib")
#endif
//
//void CCefSimple::Init(HINSTANCE hInstance)
//{
//
//	// Enable High-DPI support on Windows 7 or newer.
//	CefEnableHighDPISupport();
//
//	void* sandbox_info = NULL;
//
//#if defined(CEF_USE_SANDBOX)
//	// Manage the life span of the sandbox information object. This is necessary
//	// for sandbox support on Windows. See cef_sandbox_win.h for complete details.
//	CefScopedSandboxInfo scoped_sandbox;
//	sandbox_info = scoped_sandbox.sandbox_info();
//#endif
//
//	// Provide CEF with command-line arguments.
//	CefMainArgs main_args(hInstance);
//
//	// Optional implementation of the CefApp interface.
//	CefRefPtr<SimpleApp> app(new SimpleApp);
//
//	// Populate this structure to customize CEF behavior.
//	CefSettings settings;
//
//#if !defined(CEF_USE_SANDBOX)
//	settings.no_sandbox = true;
//#endif
//
//	// Specify the path for the sub-process executable.
//	//CefString(&settings.browser_subprocess_path).FromASCII("plugins/sub_process.exe");
//	CefString(&settings.browser_subprocess_path).FromASCII("C:/Program Files/Notepad++/plugins/sub_process.exe");
//
//
//	settings.multi_threaded_message_loop = 1;
//
//	// Initialize CEF in the main process.
//	CefInitialize(main_args, settings, app.get(), sandbox_info);
//
//
//
//}

void CCefSimple::Init(HINSTANCE hInstance)
{
	SetDllDirectory(L"C:\\Program Files\\Notepad++\\plugins\\cef");
	_instance = hInstance;
}

void CCefSimple::Init2()
{
	if (flag)
	{
		return;
	}
	flag = 1;
	HINSTANCE hInstance = _instance;

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
	CefRefPtr<SimpleApp> app(new SimpleApp);


	// Populate this structure to customize CEF behavior.
	CefSettings settings;


#if !defined(CEF_USE_SANDBOX)
	settings.no_sandbox = true;
#endif


	settings.multi_threaded_message_loop = 1;

	CefString(&settings.browser_subprocess_path).FromASCII("C:/Program Files/Notepad++/plugins/cef/sub_process.exe");

	// Initialize CEF in the main process.
	CefInitialize(main_args, settings, app.get(), sandbox_info);



}


void CCefSimple::CleanUp()
{
	CefShutdown();
}

//
//extern NppData nppData;
//
//BOOL CALLBACK DemoDlg::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message) 
//	{
//		case WM_COMMAND : 
//		{
//			switch (wParam)
//			{
//				case IDOK :
//				{
//					int line = getLine();
//					if (line != -1)
//					{
//						// Get the current scintilla
//						int which = -1;
//						::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
//						if (which == -1)
//							return FALSE;
//						HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
//
//						::SendMessage(curScintilla, SCI_ENSUREVISIBLE, line-1, 0);
//						::SendMessage(curScintilla, SCI_GOTOLINE, line-1, 0);
//					}
//					return TRUE;
//				}
//			}
//				return FALSE;
//		}
//
//		default :
//			return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
//	}
//}

