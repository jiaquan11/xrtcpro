#include "basic_form.h"

#include "stdafx.h"
#include "ui_wnd.h"
#include "main.h"

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
	btn_device_start_ = dynamic_cast<ui::Button*>(FindControl(L"btn_device_start"));
	//初始化XRTC引擎
	xrtc::XRTCEngine::Init(this);
	InitComboCam();

	//将所有控件的事件都交给BasicForm的Notify函数处理
	m_pRoot->AttachBubbledEvent(ui::kEventAll, nbase::Bind(&BasicForm::Notify, this, std::placeholders::_1));
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
	int total = xrtc::XRTCEngine::GetCameraCount();
	if (total <= 0) {
		return;
	}
	combo_cam_ = dynamic_cast<ui::Combo*>(FindControl(L"cam_combo"));
	if (!combo_cam_) {
		return;
	}

	for (int i = 0; i < total; ++i) {
		std::string device_name;
		std::string device_id;
		xrtc::XRTCEngine::GetCameraInfo(i, device_name, device_id);
		ui::ListContainerElement* element = new ui::ListContainerElement();
		element->SetClass(L"listitem");
		element->SetFixedHeight(30);
		element->SetText(nbase::UTF8ToUTF16(device_name));
		element->SetDataID(nbase::UTF8ToUTF16(device_id));
		element->SetTextPadding({ 6, 0, 6, 0 });
		//将列表项添加到下拉列表控件中
		combo_cam_->Add(element);
	}

	int count = combo_cam_->GetCount();//获取下拉列表控件中的列表项个数
	if (count > 0) {
		combo_cam_->SelectItem(0);//默认选中第一个列表项
	}

	combo_cam_->AttachSelect(nbase::Bind(&BasicForm::OnComboCamItemSelected, this, std::placeholders::_1));
}

void BasicForm::InitComboMic() {
}

bool BasicForm::OnComboCamItemSelected(ui::EventArgs* msg) {
	return true;
}

bool BasicForm::OnComboMicItemSelected(ui::EventArgs* msg) {
	return true;
}

//启动摄像头设备
void BasicForm::OnBtnDeviceStartClick() {
	btn_device_start_->SetEnabled(false);//点击按钮后，禁用按钮，避免重复点击
	if (!device_init_) {
		if (StartDevice()) {
			btn_device_start_->SetText(L"停止音视频设备");
		}
	} else {
		if (StopDevice()) {
			btn_device_start_->SetText(L"启动音视频设备");
		}
	}
	btn_device_start_->SetEnabled(true);//启动或停止设备后，启用按钮
}

bool BasicForm::StartDevice() {
	if (!combo_cam_) {
		return false;
	}
	//获取当前选中的摄像头设备ID
	int index = combo_cam_->GetCurSel();
	auto* item = combo_cam_->GetItemAt(index);
	std::wstring w_device_id = item->GetDataID();//获取之前绑定的摄像头设备ID
	cam_source_ = xrtc::XRTCEngine::CreateCamSource(nbase::UTF16ToUTF8(w_device_id));
	cam_source_->Start();
	return true;
}

bool BasicForm::StopDevice() {
	if (!device_init_ || !cam_source_) {
		return false;
	}
	cam_source_->Stop();
	cam_source_->Destroy();
	cam_source_ = nullptr;

	device_init_ = false;
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
	//在UI线程中更新UI
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
	ui_thread_->message_loop()->PostTask(task);//将任务投递到UI线程
}

void BasicForm::OnVideoSourceSuccess(xrtc::IVideoSource*) {
	//api_thread线程回调
	device_init_ = true;
	ShowToast(L"摄像头启动成功", false);
}

void BasicForm::OnVideoSourceFailed(xrtc::IVideoSource* video_source, xrtc::XRTCError err) {
	std::wstring wstr = nbase::StringPrintf(L"摄像头启动失败, err_code:%d", err);
	ShowToast(wstr, true);
}




