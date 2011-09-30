#include "UIViewManager.h"
#include "UIView.h"
#include <iostream>
#include <Awesomium/awesomium_capi.h>
#include "AweString.h"
#include "UIViewListener.h"


UIViewManager* UIViewManager::INSTANCE = NULL;

UIViewManager::UIViewManager(void): _focusedUIView(nullptr)
{
	
}


UIViewManager::~UIViewManager(void)
{
	Map::iterator i;
	for(i = _uiViewsMap.begin(); i != _uiViewsMap.end(); ++i){
		// ?? no need to destroy the webview? If the associated UIView is deleted then the webview will be destroyed automatically.
		// awe_webview_destroy(i->first);
		delete i->second;
	}
	_uiViewsMap.clear();

	awe_webcore_shutdown();
}

void UIViewManager::Init()
{
	awe_webcore_initialize(true, true, true, awe_string_empty(), awe_string_empty(), awe_string_empty(), AWE_LL_NORMAL, false, 
		awe_string_empty(), true, awe_string_empty(), awe_string_empty(), awe_string_empty(), awe_string_empty(), 
		awe_string_empty(), awe_string_empty(), true, 0, false, true, awe_string_empty());
}

void UIViewManager::AddUIView(UIView* uiview)
{
	_uiViewsMap[uiview->GetWebview()] = uiview;
}

void UIViewManager::RemoveUIView( UIView* uiview )
{
	Map::iterator itr = _uiViewsMap.find(uiview->GetWebview());
	if(itr != _uiViewsMap.end()){
		_uiViewsMap.erase(itr);
	}
}


UIView* UIViewManager::GetUIView( awe_webview* wv )
{
	return _uiViewsMap[wv];
}

void UIViewManager::UpdateViewBuffer()
{
	Map::iterator i;
	for(i = _uiViewsMap.begin(); i != _uiViewsMap.end(); ++i){
		i->second->UpdateBuffer();
	}
}

void UIViewManager::Draw()
{
	Map::iterator i;
	for(i = _uiViewsMap.begin(); i != _uiViewsMap.end(); ++i){
		i->second->Draw();
	}
}

void UIViewManager::OnCallback( awe_webview* caller, const awe_string* object_name, const awe_string* callback_name, const awe_jsarray* arguments )
{
	UIView* uiview = UIViewManager::GetInstance()->GetUIView(caller);
	std::vector<UIViewListener*> listeners = uiview->GetListeners();
	int len = listeners.size();
	for(int i=0; i<len; ++i){
		listeners[i]->OnCallback(uiview, AweString::std_str(object_name), AweString::std_str(callback_name), arguments);
	}
}

void UIViewManager::OnBeginLoading( awe_webview* caller, const awe_string* url, const awe_string* frame_name, int status_code, const awe_string* mime_type )
{
	UIView* uiview = UIViewManager::GetInstance()->GetUIView(caller);
	std::vector<UIViewListener*> listeners = uiview->GetListeners();
	int len = listeners.size();
	for(int i=0; i<len; ++i){
		listeners[i]->OnBeginLoading(uiview, AweString::std_str(url), AweString::std_str(frame_name), status_code, AweString::std_str(mime_type));
	}
}

void UIViewManager::OnBeginNavigation( awe_webview* caller, const awe_string* url, const awe_string* frame_name )
{
	UIView* uiview = UIViewManager::GetInstance()->GetUIView(caller);
	std::vector<UIViewListener*> listeners = uiview->GetListeners();
	int len = listeners.size();
	for(int i=0; i<len; ++i){
		listeners[i]->OnBeginNavigation(uiview, AweString::std_str(url), AweString::std_str(frame_name));
	}
}

void UIViewManager::OnFinishLoading(awe_webview* caller)
{
	UIView* uiview = UIViewManager::GetInstance()->GetUIView(caller);
	std::vector<UIViewListener*> listeners = uiview->GetListeners();
	int len = listeners.size();
	for(int i=0; i<len; ++i){
		listeners[i]->OnFinishLoading(uiview);
	}
}

void UIViewManager::OnChangeCursor( awe_webview* caller, awe_cursor_type cursor )
{
	UIView* uiview = UIViewManager::GetInstance()->GetUIView(caller);
	std::vector<UIViewListener*> listeners = uiview->GetListeners();
	int len = listeners.size();
	for(int i=0; i<len; ++i){
		listeners[i]->OnChangeCursor(uiview, cursor);
	}
}

void UIViewManager::OnOpenExternalLink( awe_webview* caller, const awe_string* url, const awe_string* source )
{
	UIView* uiview = UIViewManager::GetInstance()->GetUIView(caller);
	std::vector<UIViewListener*> listeners = uiview->GetListeners();
	int len = listeners.size();
	for(int i=0; i<len; ++i){
		listeners[i]->OnOpenExternalLink(uiview, AweString::std_str(url), AweString::std_str(source));
	}
}

void UIViewManager::OnRequestFileChooser( awe_webview* caller, bool selectMultipleFiles, const awe_string* title, const awe_string* defaultPath )
{
	UIView* uiview = UIViewManager::GetInstance()->GetUIView(caller);
	std::vector<UIViewListener*> listeners = uiview->GetListeners();
	int len = listeners.size();
	for(int i=0; i<len; ++i){
		listeners[i]->OnRequestFileChooser(uiview, selectMultipleFiles, AweString::std_str(title), AweString::std_str(defaultPath));
	}
}


void UIViewManager::Focus( UIView* uiview )
{
	if(_focusedUIView != nullptr){
		awe_webview_unfocus(_focusedUIView->GetWebview());
	}

	_focusedUIView = uiview;
	awe_webview_focus(_focusedUIView->GetWebview());
}

void UIViewManager::Unfocus( UIView* uiview )
{
	if(_focusedUIView != nullptr){
		awe_webview_unfocus(_focusedUIView->GetWebview());
		_focusedUIView = nullptr;
	}
}

void UIViewManager::MsgHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if(_focusedUIView == nullptr)
		return;

	short x = (short)LOWORD(lParam);
	short y = (short)HIWORD(lParam);

	switch(msg)
	{
		// inject the mouse events
		case WM_LBUTTONDOWN:
			awe_webview_inject_mouse_down(_focusedUIView->GetWebview(), AWE_MB_LEFT);
			break;
		case WM_LBUTTONUP:
			awe_webview_inject_mouse_up(_focusedUIView->GetWebview(), AWE_MB_LEFT);
			break;
		case WM_RBUTTONDOWN:
			awe_webview_inject_mouse_down(_focusedUIView->GetWebview(), AWE_MB_RIGHT);
			break;
		case WM_RBUTTONUP:
			awe_webview_inject_mouse_up(_focusedUIView->GetWebview(), AWE_MB_RIGHT);
			break;
		case WM_MOUSEMOVE:
			awe_webview_inject_mouse_move(_focusedUIView->GetWebview(), x, y);
			break;
		// inject the keyboard event
		case WM_CHAR:
		case WM_KEYDOWN:
		case WM_KEYUP:
			awe_webview_inject_keyboard_event_win(_focusedUIView->GetWebview(), msg, wParam, lParam);
			break;
	}
}