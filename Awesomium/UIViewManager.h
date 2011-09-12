#pragma once

#include <map>
#include <Awesomium/awesomium_capi.h>

class UIView;

class UIViewManager
{
private:
	static UIViewManager* INSTANCE;

	typedef std::map<awe_webview*, UIView*> Map;

public:
	static UIViewManager* GetInstance(){
		if(INSTANCE == NULL){
			INSTANCE = new UIViewManager();
		}
		return INSTANCE;
	}

	void Update();

	void Draw();

	UIView* Create(unsigned int w, unsigned int h);

	UIView* GetUIView(awe_webview* webview);

protected:
	UIViewManager(void);
	~UIViewManager(void);

	Map _uiViewsMap;
};

