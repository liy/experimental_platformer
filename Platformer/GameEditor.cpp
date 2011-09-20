#include "GameEditor.h"
#include "UIViewManager.h"
#include <Awesomium/awesomium_capi.h>
#include "UIView.h"
#include "AweString.h"

GameEditor::GameEditor(Game* game)
{
	_game = game;

	UIViewManager::GetInstance()->Init();

	_uiview = new UIView(1024, 768);
	awe_webview_focus(_uiview->GetWebview());
	_uiview->LoadURL("http://www.google.co.uk");
	//_uiview->LoadURL("E:/GameDev/Platformer/Platformer/Debug/data/ui/editor/generic.html");

	awe_webview_set_transparent(_uiview->GetWebview(), true);
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












void injectSpecialKey(awe_webview* webview, int keyCode)
{
	// Key Down
	awe_webkeyboardevent e;
	e.is_system_key = false;
	e.modifiers = 0;

	e.text[0] = keyCode;
	e.text[1] = 0;
	e.text[2] = 0;
	e.text[3] = 0;
	e.unmodified_text[0] = keyCode;
	e.unmodified_text[1] = 0;
	e.unmodified_text[2] = 0;
	e.unmodified_text[3] = 0;
	e.virtual_key_code = keyCode;
	e.native_key_code = keyCode;
	e.type = AWE_WKT_KEYDOWN;
	awe_webview_inject_keyboard_event(webview, e);

	// Key Up
	e.type = AWE_WKT_KEYUP;
	awe_webview_inject_keyboard_event(webview, e);

	// update immediately
	awe_webcore_update();
}


void SpecialKeyPressed(awe_webview* webview, int keyCode){
	injectSpecialKey(webview, keyCode);

}




void GameEditor::MsgHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	short x = (short)LOWORD(lParam);
	short y = (short)HIWORD(lParam);

	switch(msg)
	{
		case WM_LBUTTONDOWN:
		{
			LeftMouseDown(x, y);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			RightMouseDown(x, y);
			break;
		}
		case WM_LBUTTONUP:
		{
			LeftMouseUp(x, y);
			break;
		}
		case WM_RBUTTONUP:
		{
			RightMouseUp(x, y);
			break;
		}
		case WM_MOUSEMOVE:
		{
			MouseMoved(x, y);
			break;
		}
		case WM_CHAR:
		case WM_KEYDOWN:
		case WM_KEYUP:
			awe_webview_inject_keyboard_event_win(_uiview->GetWebview(), msg, wParam, lParam);
			break;
		/*
		case WM_KEYDOWN:
		{
			// special key
			switch(wParam)
			{
			case VK_DELETE:
			case VK_BACK:
			case VK_CAPITAL:
			case VK_LEFT:
			case VK_UP:
			case VK_RIGHT:
			case VK_DOWN:
			case VK_PRIOR:
			case VK_NEXT:
			case VK_HOME:
			case VK_END:
			case VK_CONTROL:
			case VK_SHIFT:
			case VK_MENU:
				SpecialKeyPressed(_uiview->GetWebview(), wParam);
				return;
			}
			break;
		}
		case WM_CHAR:
		{
			// special key triggers in WM_CHAR
			switch(wParam)
			{
			case VK_TAB:
			case VK_RETURN:
				SpecialKeyPressed(_uiview->GetWebview(), wParam);
				return;
			}

			// injection of normal character keys
			awe_webkeyboardevent e;
			e.type = AWE_WKT_CHAR;
			e.is_system_key = false;
			e.text[0] = wParam;
			e.text[1] = 0;
			e.text[2] = 0;
			e.text[3] = 0;
			e.unmodified_text[0] = 0;
			e.unmodified_text[1] = 0;
			e.unmodified_text[2] = 0;
			e.unmodified_text[3] = 0;
			e.virtual_key_code = 0;
			e.native_key_code = 0;
			awe_webview_inject_keyboard_event(_uiview->GetWebview(), e);
			break;
		}
		*/
	}
}

void GameEditor::MouseMoved( int x, int y )
{
	awe_webview_inject_mouse_move(_uiview->GetWebview(), x, y);
}

void GameEditor::LeftMouseDown(int x, int y )
{
	awe_webview_inject_mouse_down(_uiview->GetWebview(), AWE_MB_LEFT);
}

void GameEditor::LeftMouseUp(int x, int y)
{
	awe_webview_inject_mouse_up(_uiview->GetWebview(), AWE_MB_LEFT);
}

void GameEditor::RightMouseDown(int x, int y )
{
	awe_webview_inject_mouse_down(_uiview->GetWebview(), AWE_MB_RIGHT);
}

void GameEditor::RightMouseUp(int x, int y)
{
	awe_webview_inject_mouse_up(_uiview->GetWebview(), AWE_MB_RIGHT);
}
