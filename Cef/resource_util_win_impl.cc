// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "resource_util.h"

#include "resource1.h"

namespace shared {

int GetResourceId(const std::string& resource_path) {
  //if (resource_path == "logo.png")
  //  return IDS_LOGO_PNG;
  //if (resource_path == "resource_manager.html")
  //  return IDS_RESOURCE_MANAGER_HTML;

  if (resource_path == "example.html")
    return IDR_HTML_EXAMPLE;
	
  return 0;
}

}  // namespace shared
