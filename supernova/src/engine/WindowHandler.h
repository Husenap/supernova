#pragma once

#include <windows.h>

class CWindowHandler
{
public:
	CWindowHandler();
	~CWindowHandler();

	bool Init();

	HWND GetWindowHandle() { return myWindowHandle; }
	void CloseWindow();

	bool ShouldClose() { return mShouldClose; }

	static LRESULT CALLBACK WndProc(HWND aHwnd, UINT aMessage, WPARAM wParam, LPARAM lParam);

private:
	bool mShouldClose;
	HWND myWindowHandle;
};

