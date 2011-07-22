#pragma once

#include <windows.h>

class IWinMenuEventListener
{
public:
	IWinMenuEventListener(void);
	~IWinMenuEventListener(void);

	virtual void WinMenuEventHandler(WPARAM id)=0;
};

