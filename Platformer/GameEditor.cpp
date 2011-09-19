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
	//_uiview->LoadURL("http://www.google.co.uk");
	_uiview->LoadURL("E:/GameDev/Platformer/Platformer/Debug/data/ui/editor/generic.html");

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

void GameEditor::MsgHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	short x = (short)LOWORD(lParam);
	short y = (short)HIWORD(lParam);

	switch(uMsg)
	{
		case WM_LBUTTONDOWN:
		{
			LeftMouseDown(x, y);
		}
		case WM_RBUTTONDOWN:
		{
			RightMouseDown(x, y);

		}
		case WM_LBUTTONUP:
		{
			LeftMouseUp(x, y);
		}
		case WM_RBUTTONUP:
		{
			RightMouseUp(x, y);
		}
		case WM_MOUSEMOVE:
		{
			MouseMoved(x, y);
		}
		case WM_CHAR:
		{

		}
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


void GameEditor::KeyPressed( unsigned char key, int x, int y )
{
	if(key == 8 || key == 127) // Backspace or Delete key
	{
		injectSpecialKey(_uiview->GetWebview(), VK_BACK);
		return;
	}
	else if(key == 9) // Tab key
	{
		injectSpecialKey(_uiview->GetWebview(), VK_TAB);
		return;
	}
	else if(key == 13){
		// enter key
		injectSpecialKey(_uiview->GetWebview(), 13);
		return;
	}

	// special key
	switch(key)
	{
	case VK_LEFT:
		injectSpecialKey(_uiview->GetWebview(), VK_LEFT);
		break;
	case VK_UP:
		injectSpecialKey(_uiview->GetWebview(), VK_UP);
		return;
	case VK_RIGHT:
		injectSpecialKey(_uiview->GetWebview(), VK_RIGHT);
		return;
	case VK_DOWN:
		injectSpecialKey(_uiview->GetWebview(), VK_DOWN);
		return;
	case VK_PRIOR:
		injectSpecialKey(_uiview->GetWebview(), VK_PRIOR);
		return;
	case VK_NEXT:
		injectSpecialKey(_uiview->GetWebview(), VK_NEXT);
		return;
	case VK_HOME:
		injectSpecialKey(_uiview->GetWebview(), VK_HOME);
		return;
	case VK_END:
		injectSpecialKey(_uiview->GetWebview(), VK_END);
		return;
	}

	// injection of normal character keys
	awe_webkeyboardevent e;
	e.type = AWE_WKT_CHAR;
	e.is_system_key = false;
	e.text[0] = key;
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
}
