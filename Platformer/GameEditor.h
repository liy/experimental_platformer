#pragma once
#include <dinput.h>
#include <dinputd.h>
#include <stdio.h>
#include <strsafe.h>
#include <vector>
#include "UIViewListener.h"

class Game;
class UIViewManager;
class UIView;

class GameEditor: public UIViewListener
{
public:
	GameEditor(Game* game);
	~GameEditor(void);

	/**
	 * Update webcore
	 */
	void Update();

	/**
	 * 
	 */
	void Draw();

	/**
	 * @fn	void GameEditor::MsgHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	 *
	 * @brief	Message handler. Delegate the message to UIViewManager
	 *
	 * @param	hWnd  	Handle of the window.
	 * @param	uMsg  	The message.
	 * @param	wParam	The wParam field of the message.
	 * @param	lParam	The lParam field of the message.
	 */
	void MsgHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void OnBeginNavigation(UIView* uiview, const std::string& url, const std::string& frameName);
	void OnBeginLoading(UIView* uiview, const std::string& url, const std::string& frameName, int status_code, const std::string& mime_type);
	void OnFinishLoading(UIView* uiview);
	void OnCallback(UIView* uiview, const std::string& objName, const std::string& callbackName, const awe_jsarray* arguments);
	void OnChangeCursor(UIView* uiview, awe_cursor_type cursor);
	void OnOpenExternalLink(UIView* uiview, const std::string& url, std::string& sourceUrl);

	void OnRequestFileChooser(UIView* caller, bool selectMultipleFiles, const std::string& title, const std::string& defaultPath);

protected:
	Game* _game;

	UIView* _uiview;
};

