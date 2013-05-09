#ifndef SC4HOOK_H
#define SC4HOOK_H

#include "windows.h"
#include "Sc4Hook_global.h"
#include <QThread>

extern "C" DECLSPEC_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD
Reason, LPVOID LPV);


class Sc4Hook : public QThread {
public:
    Sc4Hook();

    void setHwnd(HWND);

    void run();

private:
    HWND m_hOverlay;
    HWND m_hMainWindow;
};


#endif // SC4HOOK_H
