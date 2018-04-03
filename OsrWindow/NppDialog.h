#ifndef NPP_DIALOG_H
#define NPP_DIALOG_H

#include "DockingDlgInterface.h"
#include "resource.h"

class NppDialog : public DockingDlgInterface
{
public :
	NppDialog() : DockingDlgInterface(IDD_PLUGINGOLINE_DEMO){};

    virtual void display(bool toShow = true) const {
        DockingDlgInterface::display(toShow);
        if (toShow)
            ::SetFocus(::GetDlgItem(_hSelf, ID_GOLINE_EDIT));
    };

	void setParent(HWND parent2set){
		_hParent = parent2set;
	};

protected :
	virtual BOOL CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);

private :

    int getLine() const {
        BOOL isSuccessful;
        int line = ::GetDlgItemInt(_hSelf, ID_GOLINE_EDIT, &isSuccessful, FALSE);
        return (isSuccessful?line:-1);
    };

};

#endif //NPP_DIALOG_H
