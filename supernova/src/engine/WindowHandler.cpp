#include "WindowHandler.h"

#include "../precompiled.h"

CWindowHandler::CWindowHandler() { }

CWindowHandler::~CWindowHandler() { }

bool CWindowHandler::Init()
{
	mShouldClose = false;

	WNDCLASS windowclass = {};

	windowclass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowclass.lpfnWndProc = WndProc;
	windowclass.lpszClassName = "supernova";
	RegisterClass(&windowclass);

	int dwStyle = WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_SIZEBOX | WS_POPUP;

	myWindowHandle = CreateWindow("supernova",
		"supernova",
		dwStyle,
		0, 0, WIDTH, HEIGHT,
		NULL, NULL, NULL, NULL);

	RECT rect;
	GetClientRect(myWindowHandle, &rect);

	SetWindowPos(myWindowHandle, HWND_TOP, 0, 0, WIDTH + (WIDTH - (rect.right - rect.left)), HEIGHT + (HEIGHT - (rect.bottom - rect.top)), SWP_SHOWWINDOW);

	SetWindowLongPtr(myWindowHandle, GWLP_USERDATA, (LONG_PTR)this);

	return true;
}

LRESULT CALLBACK CWindowHandler::WndProc(HWND aHwnd, UINT aMessage, WPARAM wParam, LPARAM lParam)
{
	CWindowHandler* window = (CWindowHandler*)GetWindowLongPtr(aHwnd, GWLP_USERDATA);

	switch (aMessage)
	{
	case WM_KEYDOWN: case WM_KEYUP:
	case WM_LBUTTONDOWN: case WM_RBUTTONDOWN: case WM_MBUTTONDOWN:
	case WM_LBUTTONUP: case WM_RBUTTONUP: case WM_MBUTTONUP:
	case WM_MOUSEMOVE: case WM_MOUSEWHEEL:
		//IWorld::Input().OnInput(aMessage, wParam, lParam);
		return 0;
		break;
	case WM_SYSKEYDOWN: case WM_SYSKEYUP:
		//if (wParam == Input::Key_Alt)
		{
			//IWorld::Input().OnInput(aMessage, wParam, lParam);
			return 0;
		}
		break;
	case WM_SIZE:
		//if (wParam == SIZE_RESTORED)
		{
			//IEngine::SetWindowSize({ LOWORD(lParam), HIWORD(lParam) });
		}
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		//IEngine::GetEngine().Shutdown();
		window->mShouldClose = true;
		return 0;
		break;
	}

	return DefWindowProc(aHwnd, aMessage, wParam, lParam);
}

void CWindowHandler::CloseWindow()
{
	DestroyWindow(myWindowHandle);
}

