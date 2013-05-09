
#include "sc4hook.h"


#include <iostream>
#include <fstream>

#include <QWidget>
#include <QThread>


#include "api_hook.h"
#include <QApplication>


using namespace std;

static Sc4Hook* theHook = NULL;


static unsigned beginPaintSysCall;
static BYTE* beginPaint;

HDC WINAPI hookedBeginPaint(HWND hWnd, PAINTSTRUCT* lpPaint)
{
    if(theHook)
        theHook->setHwnd(hWnd);

    HDC result;
    asm ("push    %2\n"
         "push    %3\n"
         "call    *%4\n"
         : "=a" (result)
         : "a" (beginPaintSysCall), "g" (lpPaint), "g" (hWnd), "m" (beginPaint)
         : "memory"
        );
    return result;
}


Sc4Hook::Sc4Hook()
{
    m_hMainWindow = NULL;
    m_hOverlay = NULL;

    MessageBoxA(NULL,"I'm in your process stealing all yer bytes",NULL,0);
    hook("User32.dll","BeginPaint",beginPaintSysCall,beginPaint,reinterpret_cast<const void *>(reinterpret_cast<ptrdiff_t>(hookedBeginPaint)));



}

void Sc4Hook::setHwnd(HWND hWnd)
{
    if(m_hMainWindow != NULL)
        return;

    m_hMainWindow = hWnd;
    string args = GetCommandLineA();

    MessageBoxA(hWnd,args.c_str(),0,0);

    if(args.find("-FullscreenWindowed") != string::npos)
    {
        LONG lStyle = GetWindowLong(hWnd, GWL_STYLE);
        lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
        SetWindowLong(hWnd, GWL_STYLE, lStyle);

        SetWindowPos(hWnd,0,0,0,0,0, SWP_NOSIZE);
    }


    size_t overlayOption = args.find("-OverlayHandle:");
    if(overlayOption != string::npos)
    {
        MessageBoxA(hWnd,"Setting parent",0,0);
        m_hOverlay = (HWND)atoi( args.substr(overlayOption+15).c_str() );
        ::SetParent(m_hOverlay,m_hMainWindow);
    }


}

void Sc4Hook::run()
{
   /* QApplication app();

    QWidget* widget = new QWidget;
    widget->show();
    */
}


BOOL APIENTRY DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpvReserved
)
{
    if( theHook == NULL )
    {
       // Increase reference count via LoadLibrary
       char lib_name[MAX_PATH];
       ::GetModuleFileNameA( hinstDLL, lib_name, MAX_PATH );
       ::LoadLibraryA( lib_name );

       //Setup the hook
       theHook = new Sc4Hook();
   }

    return TRUE;
}
