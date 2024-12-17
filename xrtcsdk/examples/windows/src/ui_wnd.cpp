#include "ui_wnd.h"

CWndUI::CWndUI(HWND parent) : hwnd_(parent) {
    video_wnd_.Create(parent, L"VideoWin", WS_CHILD | WS_VISIBLE, 0, 0, 0, 160, 90);
    video_wnd_.SetFitMode(false);
    video_wnd_.SetBkColor(0);
    this->Attach(video_wnd_.handle(), true);
}