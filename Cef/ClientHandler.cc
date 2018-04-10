// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "ClientHandler.h"
#include "resource_util.h"

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"


namespace {

SimpleHandler* g_instance = NULL;


// Add example Providers to the CefResourceManager.
void SetupResourceManager(CefRefPtr<CefResourceManager> resource_manager) {
	if (!CefCurrentlyOn(TID_IO)) {
		// Execute on the browser IO thread.
		CefPostTask(TID_IO, base::Bind(SetupResourceManager, resource_manager));
		return;
	}

	const std::string& test_origin = shared::kTestOrigin;

	// Add the Provider for bundled resource files.
#if defined(OS_WIN)
	// Read BINARY resources from the executable.
	resource_manager->AddProvider(
		shared::CreateBinaryResourceProvider(test_origin), 100, std::string());
#elif defined(OS_POSIX)
	// Read individual resource files from a directory on disk.
	std::string resource_dir;
	if (shared::GetResourceDir(resource_dir)) {
		resource_manager->AddDirectoryProvider(test_origin, resource_dir, 100,
			std::string());
	}
#endif
}

}  // namespace

SimpleHandler::SimpleHandler(bool use_views)
    : use_views_(use_views), is_closing_(false) {
  DCHECK(!g_instance);
  g_instance = this;
}

SimpleHandler::~SimpleHandler() {
  g_instance = NULL;
}

// static
SimpleHandler* SimpleHandler::GetInstance() {
  return g_instance;
}

void SimpleHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                  const CefString& title) {
  CEF_REQUIRE_UI_THREAD();

  if (use_views_) {
    // Set the title of the window using the Views framework.
    CefRefPtr<CefBrowserView> browser_view =
        CefBrowserView::GetForBrowser(browser);
    if (browser_view) {
      CefRefPtr<CefWindow> window = browser_view->GetWindow();
      if (window)
        window->SetTitle(title);
    }
  } else {
    // Set the title of the window using platform APIs.
    PlatformTitleChange(browser, title);
  }
}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Add to the list of existing browsers.
  browser_list_.push_back(browser);
}

bool SimpleHandler::DoClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Closing the main window requires special handling. See the DoClose()
  // documentation in the CEF header for a detailed destription of this
  // process.
  if (browser_list_.size() == 1) {
    // Set a flag to indicate that the window close should be allowed.
    is_closing_ = true;
  }

  // Allow the close. For windowed browsers this will result in the OS close
  // event being sent.
  return false;
}

void SimpleHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Remove from the list of existing browsers.
  BrowserList::iterator bit = browser_list_.begin();
  for (; bit != browser_list_.end(); ++bit) {
    if ((*bit)->IsSame(browser)) {
      browser_list_.erase(bit);
      break;
    }
  }

  if (browser_list_.empty()) {
    // All browser windows have closed. Quit the application message loop.
    CefQuitMessageLoop();
  }
}

void SimpleHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                ErrorCode errorCode,
                                const CefString& errorText,
                                const CefString& failedUrl) {
  CEF_REQUIRE_UI_THREAD();

  // Don't display an error for downloaded files.
  if (errorCode == ERR_ABORTED)
    return;

  // Display a load error message.
  std::stringstream ss;
  ss << "<html><body bgcolor=\"white\">"
        "<h2>Failed to load URL "
     << std::string(failedUrl) << " with error " << std::string(errorText)
     << " (" << errorCode << ").</h2></body></html>";
  frame->LoadString(ss.str(), failedUrl);
}

void SimpleHandler::CloseAllBrowsers(bool force_close) {
  if (!CefCurrentlyOn(TID_UI)) {
    // Execute on the UI thread.
    CefPostTask(TID_UI, base::Bind(&SimpleHandler::CloseAllBrowsers, this,
                                   force_close));
    return;
  }

  if (browser_list_.empty())
    return;

  BrowserList::const_iterator it = browser_list_.begin();
  for (; it != browser_list_.end(); ++it)
    (*it)->GetHost()->CloseBrowser(force_close);
}



void SimpleHandler::PlatformTitleChange(CefRefPtr<CefBrowser> browser,
	const CefString& title) {
	CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
	SetWindowText(hwnd, std::wstring(title).c_str());
}

void SimpleHandler::OnSize(int width, int height)
{
	BrowserList::const_iterator it = browser_list_.begin();
	for (; it != browser_list_.end(); ++it)
	{
		HWND hwnd = (*it)->GetHost()->GetWindowHandle();
		::MoveWindow(hwnd, 0, 0, width, height, TRUE);
	}
}

void SimpleHandler::ExecuteJavascript(std::string content)
{
	BrowserList::const_iterator it = browser_list_.begin();
	for (; it != browser_list_.end(); ++it)
	{
		int count = (*it)->GetFrameCount();
		CefRefPtr<CefFrame> mainFrame = (*it)->GetMainFrame();
		CefRefPtr<CefFrame> focusedFrame = (*it)->GetFocusedFrame();

		int i = 0;

		if (mainFrame.get())
		{
			CefString strCode(content);
			CefString strUrl(L"");
			mainFrame->ExecuteJavaScript(strCode, strUrl, 0);
			//mainFrame->ExecuteJavaScript()
			i++;
		}

		if (focusedFrame.get())
		{
			//mainFrame->ExecuteJavaScript()
			i++;
		}

		break;
	}

	return;
}



