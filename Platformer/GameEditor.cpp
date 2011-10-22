#include "GameEditor.h"
#include "UIViewManager.h"
#include <Awesomium/awesomium_capi.h>
#include "UIView.h"
#include "AweString.h"
#include "Game.h"
#include "RenderModule.h"

GameEditor::GameEditor(Game* game)
{
	_game = game;

	UIViewManager::GetInstance()->Init();

	_uiview = new UIView(_game->getRenderer().GetScreenWidth(), _game->getRenderer().GetScreenHeight());
	//_uiview->LoadURL("http://www.google.co.uk");
	_uiview->LoadURL("E:/GameDev/Platformer/Platformer/Debug/data/ui/editor/generic.html");

	_uiview->Focus();

	awe_webview_set_transparent(_uiview->GetWebview(), true);

	_uiview->AddListener(this);

	//AweString objName("jsObject");
	//awe_webview_create_object(_uiview->GetWebview(), objName.awe_str());

	//AweString funcName("trace");
	//awe_webview_set_object_callback(_uiview->GetWebview(), objName.awe_str(), funcName.awe_str());

	//AweString clickHandler("clickHandler");
	//awe_webview_set_object_callback(_uiview->GetWebview(), objName.awe_str(), clickHandler.awe_str());
	
	_uiview->SetJSCallback("jsObject", "clickHandler", this, reinterpret_cast<FuncPtr>(&GameEditor::OnJSCallback));
}


GameEditor::~GameEditor(void)
{
}

void GameEditor::Update()
{
	awe_webcore_update();
}

void GameEditor::Draw()
{
	/**
	 * Update all the UIView's buffer.
	 */
	UIViewManager::GetInstance()->UpdateViewBuffer();

	/**
	 * Then draw all the UIViews
	 */
	UIViewManager::GetInstance()->Draw();
}

void GameEditor::MsgHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	UIViewManager::GetInstance()->MsgHandler(hWnd, msg, wParam, lParam);
}

void GameEditor::OnBeginNavigation( UIView* uiview, const std::string& url, const std::string& frameName )
{

}

void GameEditor::OnBeginLoading( UIView* uiview, const std::string& url, const std::string& frameName, int status_code, const std::string& mime_type )
{

}

void GameEditor::OnFinishLoading( UIView* uiview )
{

}

void GameEditor::OnJSCallback( UIView* uiview, const std::string& objName, const std::string& callbackName, const awe_jsarray* arguments )
{
	std::cout << "objName: " << objName << " callbackName: " << callbackName << std::endl;
	/*
	if(objName == "jsObject" && callbackName == "trace"){
		const awe_jsvalue* value = awe_jsarray_get_element(arguments, 0);
		const awe_string* trace_string = awe_jsvalue_to_string(value);

		std::cout << "javascript trigger c++ function: " << AweString::std_str(trace_string) << std::endl;
	}
	*/
}

void GameEditor::OnChangeCursor( UIView* uiview, awe_cursor_type cursor )
{

}

void GameEditor::OnOpenExternalLink( UIView* uiview, const std::string& url, std::string& sourceUrl )
{

}

void GameEditor::OnRequestFileChooser( UIView* caller, bool selectMultipleFiles, const std::string& title, const std::string& defaultPath )
{

}
