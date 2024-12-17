#pragma once

#include <atomic>

#include "stdafx.h"

class MainThread;

class BasicForm : public ui::WindowImplBase
{
public:
	BasicForm(MainThread* ui_thread);
	~BasicForm();

	/**
	 * 一下三个接口是必须要覆写的接口，父类会调用这三个接口来构建窗口
	 * GetSkinFolder		接口设置你要绘制的窗口皮肤资源路径
	 * GetSkinFile			接口设置你要绘制的窗口的 xml 描述文件
	 * GetWindowClassName	接口设置窗口唯一的类名称
	 */
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;

	/**
	 * 收到 WM_CREATE 消息时该函数会被调用，通常做一些控件初始化的操作
	 */
	virtual void InitWindow() override;

	/**
	 * 收到 WM_CLOSE 消息时该函数会被调用
	 */
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void ShowMax();
	ui::Control* CreateControl(const std::wstring& pstrClass) override;
	bool Notify(ui::EventArgs* msg);

	static const std::wstring kClassName;

private:
	void InitComboCam();
	void InitComboMic();
	bool OnComboCamItemSelected(ui::EventArgs* msg);
	bool OnComboMicItemSelected(ui::EventArgs* msg);
	void OnBtnDeviceStartClick();
	bool StartDevice();
	bool StopDevice();
	void OnBtnPreviewClick();
	bool StartPreview();
	bool StopPreview();
	void OnBtnPushClick();
	bool StartPush();
	bool StopPush();
	void OnBtnPullClick();
	bool StartPull();
	bool StopPull();

	void ShowToast(const std::wstring& toast, bool err);
	void CallOnUIThread(const std::function<void(void)>& task);

private:
	MainThread* ui_thread_;
	ui::Combo* combo_cam_ = nullptr;
	ui::Combo* combo_mic_ = nullptr;
	ui::Button* btn_device_start_ = nullptr;
	ui::Button* btn_prev_ = nullptr;
	ui::Button* btn_push_ = nullptr;
	ui::Button* btn_pull_ = nullptr;
	ui::RichEdit* edit_host_ = nullptr;
	ui::RichEdit* edit_uid_ = nullptr;
	ui::RichEdit* edit_stream_name_ = nullptr;
	ui::RichEdit* edit_pull_stream_name_ = nullptr;
	ui::Label* label_network_tips_ = nullptr;
};

