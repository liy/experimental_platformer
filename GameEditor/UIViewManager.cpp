#include "UIViewManager.h"
#include "UIView.h"
#include <Awesomium/awesomium_capi.h>

typedef std::pair<awe_webview*, UIView*> ViewPair;

UIViewManager* UIViewManager::INSTANCE = NULL;

UIViewManager::UIViewManager(void)
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