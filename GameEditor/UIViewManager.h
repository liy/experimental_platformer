#pragma once

#include <map>

typedef struct _awe_string awe_string;
typedef struct _awe_webview awe_webview;
typedef struct _awe_jsarray awe_jsarray;
typedef enum _awe_loglevel awe_loglevel;
typedef enum _awe_cursor_type awe_cursor_type;

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

	/**
	 * @fn	void UIViewManager::RemoveUIView(UIView* uiview);
	 *
	 * @brief	Removes the user interface view described by uiview.
	 *
	 * @param [in,out]	uiview	If non-null, the uiview.
	 */
	void RemoveUIView(UIView* uiview);

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

	static void OnCallback(awe_webview* caller, const awe_string* object_name, const awe_string* callback_name, const awe_jsarray* arguments);

	static void OnBeginLoading(awe_webview* caller, const awe_string* url, const awe_string* frame_name, int status_code, const awe_string* mime_type);

	static void OnBeginNavigation(awe_webview* caller, const awe_string* url, const awe_string* frame_name);

	static void OnFinishLoading(awe_webview* caller);

	static void OnChangeCursor(awe_webview* caller, awe_cursor_type cursor);

	static void OnOpenExternalLink(awe_webview* caller, const awe_string* url, const awe_string* source);
};

