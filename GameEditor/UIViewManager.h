#pragma once

#include <map>

typedef struct _awe_string awe_string;
typedef struct _awe_webview awe_webview;
typedef enum _awe_loglevel awe_loglevel;

class UIView;

class UIViewManager
{
	friend class UIView;

private:
	static UIViewManager* INSTANCE;

	/**
	 * Store the mapping between awe_webview and UIView. This is for easy retrieving of the UIView when awe_webview callback is triggered.
	 */
	typedef std::map<awe_webview*, UIView*> Map;

	/**
	 * Add the UIView to this manager.
	 */
	void AddUIView(UIView* uiview);

public:
	static UIViewManager* GetInstance(){
		if(INSTANCE == NULL){
			INSTANCE = new UIViewManager();
		}
		return INSTANCE;
	}

	~UIViewManager(void);

	/**
	 * 
	 */
	void Init();

	/**
	 * Get the UIView by using awe_webview key.
	 */
	UIView* GetUIView(awe_webview* webview);

	/**
	 * Update all the UIViews buffer.
	 */
	void UpdateViewBuffer();

	/**
	 * Draw all the UIViews.
	 */
	void Draw();

protected:
	UIViewManager(void);

	Map _uiViewsMap;
};

