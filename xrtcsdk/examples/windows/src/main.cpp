#include "main.h"

#include <windows.h>

#include "basic_form.h"

enum ThreadId {
    kThreadUI
};

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    AllocConsole();

    // 创建主线程
    MainThread thread;

    // 执行主循环
    thread.RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop);

    return 0;
}

void MainThread::Init() {
    nbase::ThreadManager::RegisterThread(kThreadUI);

    // 获取资源路径，初始化全局参数
    std::wstring theme_dir = nbase::win32::GetCurrentModuleDirectory();
#ifdef _DEBUG
    // Debug 模式下使用本地文件夹作为资源
    // 默认皮肤使用 resources\\themes\\default
    // 默认语言使用 resources\\lang\\zh_CN
    // 如需修改请指定 Startup 最后两个参数
    ui::GlobalManager::Startup(theme_dir + L"resources\\", ui::CreateControlCallback(), true);
#else
    // Release 模式下使用资源中的压缩包作为资源
    // 资源被导入到资源列表分类为 THEME，资源名称为 IDR_THEME
    // 如果资源使用的是本地的 zip 文件而非资源中的 zip 压缩包
    // 可以使用 OpenResZip 另一个重载函数打开本地的资源压缩包
    ui::GlobalManager::OpenResZip(MAKEINTRESOURCE(IDR_THEME), L"THEME", "");
    // ui::GlobalManager::OpenResZip(L"resources.zip", "");
    ui::GlobalManager::Startup(L"resources\\", ui::CreateControlCallback(), false);
#endif

    ui::GlobalManager::EnableAutomation();

    // 创建一个默认带有阴影的居中窗口
    BasicForm* window = new BasicForm(this);
    window->Create(NULL, BasicForm::kClassName.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0, false);
    window->CenterWindow();
    window->ShowWindow();
    window->ShowMax();
}

void MainThread::Cleanup() {
    ui::GlobalManager::Shutdown();
    SetThreadWasQuitProperly(true);
    nbase::ThreadManager::UnregisterThread();
}
