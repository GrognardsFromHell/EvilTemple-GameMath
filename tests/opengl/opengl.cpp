
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "scene.h"

const LPWSTR WindowClassName = L"OpenGLTest";

// Forward declarations
LRESULT CALLBACK WindowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
HGLRC StartOpenGL(HDC drawingContext);
void StopOpenGL(HGLRC glContext);
double GetTime();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// Register the window class
	WNDCLASS windowClass;
	ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(0, IDC_ARROW);
	windowClass.style = CS_OWNDC;	
	windowClass.lpszClassName = WindowClassName;
	windowClass.lpfnWndProc = WindowProcedure;
	RegisterClass(&windowClass);

	// Create the application window
	HWND windowHandle = CreateWindow(WindowClassName, L"OpenGL Tests", 
		WS_CAPTION|WS_POPUPWINDOW|WS_VISIBLE,
		0, 0, 512, 512,
		0, 0, hInstance, 0);

	HDC drawingContext = GetDC(windowHandle);
	HGLRC glContext = StartOpenGL(drawingContext);

	CreateScene();
	
	double startTime = GetTime();

	MSG message;
	while (true) {

		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);

			if (message.message == WM_QUIT) {
				break;
			}
		} else {
			DrawScene(GetTime() - startTime);
			
			SwapBuffers(drawingContext);
		}

	}

	DestroyScene();

	StopOpenGL(glContext);
	ReleaseDC(windowHandle, drawingContext);

	DestroyWindow(windowHandle);

	return message.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CLOSE:
		PostQuitMessage(0);
	case WM_CREATE:
	case WM_DESTROY:
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		return 0;
	default:
		return DefWindowProc(windowHandle, message, wParam, lParam);
	}
}

HGLRC StartOpenGL(HDC drawingContext)
{
	PIXELFORMATDESCRIPTOR pfd;
	int format;

	// We do need to set the pixel format for the drawing context before creating an opengl context
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	format = ChoosePixelFormat(drawingContext, &pfd);
	SetPixelFormat(drawingContext, format, &pfd);

	// Create an OpenGL context and make it the current context
	HGLRC glContext = wglCreateContext(drawingContext);
	wglMakeCurrent(drawingContext, glContext);

	return glContext;
}

void StopOpenGL(HGLRC glContext)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(glContext);
}

double GetTime()
{
	LARGE_INTEGER now, frequency;
	QueryPerformanceCounter(&now);
	QueryPerformanceFrequency(&frequency);

	return (double)now.QuadPart / (double)frequency.QuadPart;
}
