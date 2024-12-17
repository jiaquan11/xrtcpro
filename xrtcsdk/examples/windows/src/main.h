#ifndef WIN_DEMO_MAIN_H_
#define WIN_DEMO_MAIN_H_

#include "stdafx.h"


// 主线程(UI线程)
class MainThread : public nbase::FrameworkThread {
public:
    MainThread() : nbase::FrameworkThread("MainThread") {}
    virtual ~MainThread() {}

private:
    // 初始化主线程
    void Init() override;
    void Cleanup() override;
};


#endif // WIN_DEMO_MAIN_H_