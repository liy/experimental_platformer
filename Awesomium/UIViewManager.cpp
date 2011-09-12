#include "UIViewManager.h"

#include "UIView.h"

UIViewManager* UIViewManager::INSTANCE = NULL;

UIViewManager::UIViewManager(void)
{
	awe_webcore_initialize_default();
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

UIView* UIViewManager::Create( unsigned int w, unsigned int h )
{
	awe_webview* wv = awe_webcore_create_webview(w, h, false);
	UIView* uiView = new UIView();
	uiView->Init(wv, w, h);

	_uiViewsMap[wv] = uiView;

	return uiView;
}

UIView* UIViewManager::GetUIView( awe_webview* wv )
{
	return _uiViewsMap[wv];
}

void UIViewManager::Update()
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
