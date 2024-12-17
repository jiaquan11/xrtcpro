#ifndef WIN_DEMO_UI_WND_H_
#define WIN_DEMO_UI_WND_H_

#include <duilib/UIlib.h>

#include "video_wnd.h"

class CWndUI : public ui::Control {
public:
    CWndUI(HWND parent);

    virtual void SetVisible(bool bVisible /* = true */) override {
        ui::Control::SetInternVisible(bVisible);
        if (hwnd_) {
            ::ShowWindow(hwnd_, bVisible);
        }
    }

    virtual void SetInternVisible(bool bVisible = true) override {
        ui::Control::SetInternVisible(bVisible);
        if (hwnd_) {
            ::ShowWindow(hwnd_, bVisible);
        }
    }

    virtual void SetPos(ui::UiRect rc) override {
        ui::Control::SetPos(rc);
        if (hwnd_) {
            ::SetWindowPos(hwnd_, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
    }

    HWND GetVideoWnd() {
        return hwnd_;
    }

public:
    BOOL Attach(HWND hWndNew, BOOL bSize = FALSE) {
        if (!::IsWindow(hWndNew))
        {
            return FALSE;
        }

        hwnd_ = hWndNew;
        if (bSize) {
            const RECT& rc = GetPos();
            ::SetWindowPos(hwnd_, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        return TRUE;
    }

    HWND Detach() {
        HWND wnd = hwnd_;
        hwnd_ = NULL;
        return wnd;
    }

protected:
    HWND hwnd_;
    CVideoWnd video_wnd_;
};


#endif // WIN_DEMO_UI_WND_H_