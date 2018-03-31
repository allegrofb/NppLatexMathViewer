
#ifndef CCEFSIMPLE_H
#define CCEFSIMPLE_H

#include <windows.h>

class CCefSimple 
{
public :
	void Init(HINSTANCE hInstance);
	void CleanUp();
	void Init2();

//    virtual void display(bool toShow = true) const {
//        DockingDlgInterface::display(toShow);
//        if (toShow)
//            ::SetFocus(::GetDlgItem(_hSelf, ID_GOLINE_EDIT));
//    };
//
//	void setParent(HWND parent2set){
//		_hParent = parent2set;
//	};
//
//protected :
//	virtual BOOL CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);

private :
	int flag = 0;
	HINSTANCE _instance = NULL;
    //int getLine() const {
    //    BOOL isSuccessful;
    //    int line = ::GetDlgItemInt(_hSelf, ID_GOLINE_EDIT, &isSuccessful, FALSE);
    //    return (isSuccessful?line:-1);
    //};

};

#endif //CCEFSIMPLE_H
