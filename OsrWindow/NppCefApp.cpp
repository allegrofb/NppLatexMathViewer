// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.


#include "NppCefApp.h"

#include <string>

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "simple_handler.h"


NppCefApp::NppCefApp() {}

void NppCefApp::OnContextInitialized() {

	CEF_REQUIRE_UI_THREAD();

	CefRefPtr<CefCommandLine> command_line = CefCommandLine::GetGlobalCommandLine();
	CefRefPtr<SimpleHandler> handler(new SimpleHandler(false));

	// Information used when creating the native window.
	CefWindowInfo window_info;
	window_info.SetAsWindowless(NULL);
	window_info.SetAsPopup(NULL, "NppCefApp");

	CefBrowserSettings browser_settings;
	std::string url = "file://D:/Program Files/Notepad++/plugins/cef/example.html";

	// Create the first browser window.
	CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings, NULL);

}
