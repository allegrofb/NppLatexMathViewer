// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef NPP_CEFAPP_H
#define NPP_CEFAPP_H

#include "include/cef_app.h"

// Implement application-level callbacks for the browser process.
class NppCefApp : public CefApp, public CefBrowserProcessHandler {
 public:
	 NppCefApp();

  // CefApp methods:
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
      OVERRIDE {
    return this;
  }

  // CefBrowserProcessHandler methods:
  virtual void OnContextInitialized() OVERRIDE;

 private:
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(NppCefApp);
};

#endif  // NPP_CEFAPP_H
