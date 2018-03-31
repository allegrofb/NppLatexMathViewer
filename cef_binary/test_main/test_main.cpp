// test_main.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <windows.h>


#include "include/cef_sandbox_win.h"
#include "simple_app.h"



#if defined(CEF_USE_SANDBOX)
// The cef_sandbox.lib static library is currently built with VS2013. It may not
// link successfully with other VS versions.
#pragma comment(lib, "cef_sandbox.lib")
#endif

// Program entry-point function.
int APIENTRY wWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine,
	int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

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

	// Optional implementation of the CefApp interface.
	CefRefPtr<SimpleApp> app(new SimpleApp);

	// Populate this structure to customize CEF behavior.
	CefSettings settings;

#if !defined(CEF_USE_SANDBOX)
	settings.no_sandbox = true;
#endif

	// Specify the path for the sub-process executable.
	CefString(&settings.browser_subprocess_path).FromASCII("sub_process.exe");

	//int i = 0;
	//for (;;)
	//	i++;

	// Initialize CEF in the main process.
	CefInitialize(main_args, settings, app.get(), sandbox_info);

	// Run the CEF message loop. This will block until CefQuitMessageLoop() is called.
	CefRunMessageLoop();

	// Shut down CEF.
	CefShutdown();

	return 0;
}

