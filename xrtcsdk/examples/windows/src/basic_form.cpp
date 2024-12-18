﻿#include "basic_form.h"

#include "stdafx.h"
#include "ui_wnd.h"
#include "main.h"

#include "xrtc/xrtc.h"

const std::wstring BasicForm::kClassName = L"Basic";

BasicForm::BasicForm(MainThread* ui_thread) :
    ui_thread_(ui_thread)
{
}

BasicForm::~BasicForm()
{
}

std::wstring BasicForm::GetSkinFolder()
{
	return L"basic";
}

std::wstring BasicForm::GetSkinFile()
{
	return L"basic.xml";
}

std::wstring BasicForm::GetWindowClassName() const
{
	return kClassName;
}

void BasicForm::InitWindow() {
	xrtc::XRTCEngine::Init();
}

LRESULT BasicForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

void BasicForm::ShowMax() {
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

ui::Control* BasicForm::CreateControl(const std::wstring& pstrClass) {
	//自定义控件
	if (!_tcscmp(pstrClass.c_str(), _T("VideoWnd"))) {
		CWndUI* wnd = new CWndUI(this->m_hWnd);
		return wnd;
	}

	return nullptr;
}

bool BasicForm::Notify(ui::EventArgs* msg) {
	std::wstring name = msg->pSender->GetName();
	if (msg->Type == ui::kEventClick) {
		if (L"btn_device_start" == name) {
			OnBtnDeviceStartClick();
		}
		else if (L"btn_prev" == name) {
			OnBtnPreviewClick();
		}
		else if (L"btn_push" == name) {
			OnBtnPushClick();
		}
		else if (L"btn_pull" == name) {
			OnBtnPullClick();
		}
	}
	return true;
}

void BasicForm::InitComboCam() {
	
}

void BasicForm::InitComboMic() {
}

bool BasicForm::OnComboCamItemSelected(ui::EventArgs* msg) {
	return true;
}

bool BasicForm::OnComboMicItemSelected(ui::EventArgs* msg) {
	return true;
}

void BasicForm::OnBtnDeviceStartClick() {
}

bool BasicForm::StartDevice() {
	return true;
}

bool BasicForm::StopDevice() {
	return true;
}

void BasicForm::OnBtnPreviewClick() {
}

bool BasicForm::StartPreview() {
	return true;
}

bool BasicForm::StopPreview() {
	return true;
}

void BasicForm::OnBtnPushClick() {
}

bool BasicForm::StartPush() {
	return true;
}

bool BasicForm::StopPush() {
	return true;
}

void BasicForm::OnBtnPullClick() {
}

bool BasicForm::StartPull() {
	return true;
}

bool BasicForm::StopPull() {
	return true;
}

void BasicForm::ShowToast(const std::wstring& toast, bool err) {
	CallOnUIThread([=]() {
		ui::Label* toast_text = dynamic_cast<ui::Label*>(FindControl(L"toast_text"));
		if (toast_text) {
			if (err) {
				toast_text->SetStateTextColor(ui::kControlStateNormal, L"red");
			}

			toast_text->SetText(toast);
		}
	});
}

void BasicForm::CallOnUIThread(const std::function<void(void)>& task) {
	ui_thread_->message_loop()->PostTask(task);
}




