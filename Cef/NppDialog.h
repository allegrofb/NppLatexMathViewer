#ifndef NPP_DIALOG_H
#define NPP_DIALOG_H

#include "DockingDlgInterface.h"
#include <string>
#include "resource1.h"
#include "resource.h"
#include "NppCefApp.h"

class NppDialog : public DockingDlgInterface
{
public :
	NppDialog() : DockingDlgInterface(IDD_PLUGIN_LATEXMATH){};

	virtual void init(HINSTANCE hInst, HWND parent)
	{
		DockingDlgInterface::init(hInst, parent);

		wchar_t exeFullPath[MAX_PATH]; // Full path  
		GetModuleFileNameW(NULL, exeFullPath, MAX_PATH);
		std::wstring strPath(exeFullPath);
		int pos = strPath.find_last_of('\\', strPath.length());
		_path = strPath.substr(0, pos);  // Return the directory without the file name  
		_path = _path + L"\\plugins\\cef";
		SetDllDirectory(_path.c_str());

	}

    virtual void display(bool toShow = true) 
	{
        DockingDlgInterface::display(toShow);
		if (toShow)
		{
			_showFlag = 1;
			::SetFocus(::GetDlgItem(_hSelf, ID_GOLINE_EDIT));
		}
    };

	void setParent(HWND parent2set)
	{
		_hParent = parent2set;
	};

	void CleanUp();
	void Shutdown();
	void InitCef();
	void CreateBrowser();
	int GetShowFlag() { return _showFlag; };

protected :
	virtual BOOL CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);

private :

    int getLine() const {
        BOOL isSuccessful;
        int line = ::GetDlgItemInt(_hSelf, ID_GOLINE_EDIT, &isSuccessful, FALSE);
        return (isSuccessful?line:-1);
    };

private:
	int _flag = 0;
	int _showFlag = 0;
	std::wstring _path;
};

#endif //NPP_DIALOG_H
