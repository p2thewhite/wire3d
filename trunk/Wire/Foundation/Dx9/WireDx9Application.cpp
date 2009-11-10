#include "WireDx9Application.h"
#include "../../Renderer/Dx9Renderer/WireDx9Renderer.h"

#include <Windows.h>

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

//----------------------------------------------------------------------------
Dx9Application::Dx9Application(const ColorRGBA backgroundColor, const Char*
	pWindowTitle, Int xPosition, Int yPosition, Int width, Int height)
	:
	Application(backgroundColor, pWindowTitle, xPosition, yPosition, width,
		height),
	mWindowID(0)
{
}

//----------------------------------------------------------------------------
Dx9Application::~Dx9Application()
{
	WIRE_DELETE mpRenderer;
	mpRenderer = NULL;
}

//----------------------------------------------------------------------------
LRESULT CALLBACK MsWindowEventHandler(HWND hWnd, UINT msg, WPARAM wParam,
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

	case WM_ERASEBKGND:
		{
			// This tells Windows not to erase the background (and that the
			// application is doing so).
			return 1;
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
	// register the window class
	static char sWindowClass[] = "Wire Application";
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = MsWindowEventHandler;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = 0;
	wc.hIcon         = LoadIcon(0,IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0,IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.lpszClassName = sWindowClass;
	wc.lpszMenuName  = 0;
	RegisterClass(&wc);

	// require the window to have the specified client area
	RECT rect = { 0, 0, mWidth-1, mHeight-1 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// create the application window
	HWND hWnd = CreateWindow(sWindowClass, mpWindowTitle,
		WS_OVERLAPPEDWINDOW, mXPosition, mYPosition,
		rect.right-rect.left+1, rect.bottom-rect.top+1, 0, 0, 0, 0);

	mWindowID = PtrToInt(hWnd);

	mpRenderer = WIRE_NEW Dx9Renderer(hWnd, mWidth, mHeight);
	mpRenderer->SetClearColor(mBackgroundColor);

	if (mpApplication->OnInitialize())
	{
		// display the window
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);

		// start the message pump
		bool isApplicationRunning = true;
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
				mpApplication->OnIdle();
			}
		}
	}

	mpApplication->OnTerminate();
	return 0;
}

//----------------------------------------------------------------------------
void Dx9Application::OnTerminate()
{
}

//----------------------------------------------------------------------------
Bool Dx9Application::OnInitialize()
{
	return true;
}

//----------------------------------------------------------------------------
void Dx9Application::OnIdle()
{
}
