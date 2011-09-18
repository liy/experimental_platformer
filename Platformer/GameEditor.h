#pragma once
#include <dinput.h>
#include <dinputd.h>
#include <stdio.h>
#include <strsafe.h>
#include <vector>

class Game;
class UIViewManager;
class UIView;

class GameEditor
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

	// Handling user mouse click
	void MsgHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void MouseMoved(int x, int y);

	void LeftMouseDown(int x, int y);
	void LeftMouseUp(int x, int y);
	void RightMouseDown(int x, int y);
	void RightMouseUp(int x, int y);

	void KeyPressed(unsigned char key, int x, int y);

protected:
	Game* _game;

	UIView* _uiview;
};

