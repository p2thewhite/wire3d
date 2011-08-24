// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireDx9Application.h"

#include "WireRenderer.h"
#include "WireDx9RendererInput.h"

#include <Windows.h>

#pragma comment(lib,"d3d9.lib")

using namespace Wire;

// These are in the VC7.1 basetsd.h, but not the VC6 basetsd.h.
#ifndef IntToPtr
#define IntToPtr(i) ((VOID*)(INT_PTR)((int)i))
#endif

#ifndef PtrToUint
#define PtrToUint(p) ((UINT)(UINT_PTR)(p))
#endif

#ifndef PtrToInt
#define PtrToInt(p)  ((INT)(INT_PTR)(p))
#endif

const UInt Application::KEY_ESCAPE = VK_ESCAPE;

//----------------------------------------------------------------------------
Dx9Application::Dx9Application(const ColorRGBA& rBackgroundColor, const Char*
	pWindowTitle, Int xPosition, Int yPosition, UInt width, UInt height,
	Bool isFullscreen, Bool useVSync)
	:
	Application(rBackgroundColor, pWindowTitle, xPosition, yPosition, width,
		height, isFullscreen, useVSync),
	mWindowID(0)
{
	_set_error_mode(_OUT_TO_MSGBOX);

	DEVMODE mode;
	mode.dmSize = sizeof(DEVMODE);
	mode.dmDriverExtra = 0;
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &mode);
	if (width == 0 && height == 0)
	{
		mWidth = mode.dmPelsWidth;
		mHeight = mode.dmPelsHeight;
	}
}

//----------------------------------------------------------------------------
Dx9Application::~Dx9Application()
{
	WIRE_DELETE mpRenderer;
	mpRenderer = NULL;
}

//----------------------------------------------------------------------------
LRESULT CALLBACK WireMsWindowEventHandler(HWND hWnd, UINT msg, WPARAM wParam,
	LPARAM lParam)
{
	Dx9Application* pApp = static_cast<Dx9Application*>(
		Application::GetApplication());

	if (!pApp || !pApp->GetWindowID())
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	switch (msg) 
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			if (pApp->GetRenderer())
			{
				pApp->OnIdle();
			}

			EndPaint(hWnd, &ps);
			return 0;
		}

	case WM_CHAR:
		{
			UChar key = static_cast<UChar>(wParam);

			// quit application if the KEY_TERMINATE key is pressed
			if (key == pApp->KEY_TERMINATE)
			{
				PostQuitMessage(0);
				return 0;
			}
		}

	case WM_SIZE:
		{
			if (wParam != SIZE_MINIMIZED)
			{
				UInt width = static_cast<UInt>(LOWORD(lParam));
				UInt height = static_cast<UInt>(HIWORD(lParam));
				pApp->OnResize(width, height);
			}
			return 0;
		}


	case WM_ERASEBKGND:
		{
			// This tells Windows not to erase the background (and that the
			// application is doing so).
			return 1;
		}

	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//----------------------------------------------------------------------------
Int Dx9Application::GetWindowID() const
{
	return mWindowID;
}

//----------------------------------------------------------------------------
Int Dx9Application::Main(Int, Char*[])
{
	s_pApplication->KEY_TERMINATE = Application::KEY_ESCAPE;

	// allow work to be done before the window and renderer is created
	if (!s_pApplication->OnPrecreate())
	{
		return -1;
	}

	// register the window class
	static Char s_WindowClass[] = "Wire Application";
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WireMsWindowEventHandler;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = 0;
	wc.hIcon         = LoadIcon(0,IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0,IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.lpszClassName = s_WindowClass;
	wc.lpszMenuName  = 0;
	RegisterClass(&wc);

	// require the window to have the specified client area
	RECT rect = { 0, 0, mWidth-1, mHeight-1 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// create the application window
	HWND hWnd = CreateWindow(s_WindowClass, mpWindowTitle,
		WS_OVERLAPPEDWINDOW, mXPosition, mYPosition,
		rect.right-rect.left+1, rect.bottom-rect.top+1, 0, 0, 0, 0);

	mWindowID = PtrToInt(hWnd);

	PdrRendererInput input;
	input.WindowHandle = hWnd;

	mpRenderer = WIRE_NEW Renderer(input, mWidth, mHeight, mIsFullscreen,
		mUseVSync);
	mpRenderer->SetClearColor(mBackgroundColor);

	if (s_pApplication->OnInitialize())
	{
		// display the window
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);

		// start the message pump
		Bool isApplicationRunning = true;
		while (isApplicationRunning)
		{
			MSG msg;
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					isApplicationRunning = false;
					continue;
				}

				HACCEL hAccel = 0;
				if (!TranslateAccelerator(hWnd, hAccel, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				s_pApplication->OnIdle();
			}
		}
	}

	s_pApplication->OnTerminate();
	return 0;
}

//----------------------------------------------------------------------------
Bool Dx9Application::OnPrecreate()
{
	return true;
}

//----------------------------------------------------------------------------
Bool Dx9Application::OnInitialize()
{
	return true;
}

//----------------------------------------------------------------------------
void Dx9Application::OnTerminate()
{
}

//----------------------------------------------------------------------------
void Dx9Application::OnIdle()
{
}
