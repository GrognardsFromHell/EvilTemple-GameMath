
#include <windows.h>
#include <gl/glew.h>
#include <gl/wglew.h>
#include <stdio.h>

#include "scene.h"

#include "../common/common.h"

#include <QtCore/QFile>
#include <QtCore/QByteArray>
//#include <QtXmlPatterns/QXmlSchema>
//#include <QtXmlPatterns/QXmlSchemaValidator>
//#include <QtXmlPatterns/QAbstractMessageHandler>

const LPWSTR WindowClassName = L"OpenGLTest";

// Forward declarations
LRESULT CALLBACK WindowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
HGLRC StartOpenGL(HDC drawingContext);
void StopOpenGL(HGLRC glContext);
double GetTime();
bool CheckMultisample(HDC drawingContext);

static int multisamplePixelFormat = -1;
/*
class MyMessageHandler : public QAbstractMessageHandler {
protected:
	void handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, 
		const QSourceLocation &sourceLocation)
	{
		MessageBoxA(NULL, qPrintable(description), "Error", MB_OK|MB_ICONERROR);
	}
};*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	QString commandLine = QString::fromLocal8Bit(lpCmdLine);

	if (commandLine.startsWith('"')) {
		commandLine = commandLine.mid(1, commandLine.length() - 2);
	}

	/*
	QFile schemaFile("material.xsd");

	MyMessageHandler errorHandler;

	if (!schemaFile.open(QIODevice::ReadOnly)) {
		MessageBox(NULL, TEXT("Unable to open material.xsd"), TEXT("ERROR"), MB_OK|MB_ICONERROR);
		return -1;
	}

	QXmlSchema schema;
	schema.setMessageHandler(&errorHandler);
	if (!schema.load(&schemaFile)) {
		MessageBox(NULL, TEXT("Unable to parse material.xsd"), TEXT("ERROR"), MB_OK|MB_ICONERROR);
		schemaFile.close();
		return -1;
	}
	schemaFile.close();

	QFile materialFile("material.xml");
	
	if (!materialFile.open(QIODevice::ReadOnly)) {
		MessageBox(NULL, TEXT("Unable to open material.xml"), TEXT("ERROR"), MB_OK|MB_ICONERROR);
		schemaFile.close();
		return -1;
	}
	
	QXmlSchemaValidator validator(schema);
	validator.setMessageHandler(&errorHandler);
	if (!validator.validate(&materialFile)) {
		materialFile.close();
		return -1;
	}

	materialFile.close();*/

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
		WS_CAPTION|WS_POPUPWINDOW,
		0, 0, 800, 600,
		0, 0, hInstance, 0);

	HDC drawingContext = GetDC(windowHandle);
	HGLRC glContext = StartOpenGL(drawingContext);

	GLenum err = glewInit();

	if (GLEW_OK != err) {
		char message[2048];
		sprintf(message, "Error: %s\n", glewGetErrorString(err));
		MessageBoxA(NULL, message, "OpenGL Error", MB_OK|MB_ICONERROR);
		return false;
	}

	if (!GLEW_VERSION_2_0) {
		MessageBoxA(NULL, "OpenGL 2.0 is required.", "OpenGL Error", MB_OK|MB_ICONERROR);
		return false;
	}

	if (CheckMultisample(drawingContext)) {
		wglMakeCurrent(NULL, NULL);
		DestroyWindow(windowHandle);

		windowHandle = CreateWindow(WindowClassName, L"OpenGL Tests", 
			WS_CAPTION|WS_POPUPWINDOW,
			0, 0, 800, 600,
			0, 0, hInstance, 0);
		drawingContext = GetDC(windowHandle);
		glContext = StartOpenGL(drawingContext);
	}

	ShowWindow(windowHandle, SW_NORMAL);

	if (!CreateScene(800, 600, qPrintable(commandLine))) {
		MessageBox(windowHandle, L"Unable to create OpenGL scene.", L"Error", MB_OK|MB_ICONERROR);
		DestroyWindow(windowHandle);
		return -1;
	}
	
	double startTime = GetTime();

	std::vector<double> elapsedTimes(100);
	int elapsedIndex = 0;

	Timer timer;
	MSG message;
	while (true) {

		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);

			if (message.message == WM_QUIT) {
				break;
			}
		} else {
			timer.start();
			DrawScene(GetTime() - startTime);
			elapsedTimes[elapsedIndex] = timer.end();
			elapsedIndex = (elapsedIndex + 1) % 100;

			// We just overflowed, now update the FPS
			if (elapsedIndex == 0) {
				std::sort(elapsedTimes.begin(), elapsedTimes.end(), std::less<double>());
				
				char windowTitle[512];
				float average = 0;
				int totalSamples = 0;
				for (int i = 10; i < 90; ++i) {
					average += elapsedTimes[i];
					totalSamples++;
				}
				average /= (float)totalSamples;
				sprintf(windowTitle, "OpenGL (Avg ms per Frame: %0.2f)", (float)average);
				SetWindowTextA(windowHandle, windowTitle);
			}
			
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
		} else {
			KeyPressed(wParam);
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
	if (multisamplePixelFormat > -1) {
		format = multisamplePixelFormat;
	} else {
		format = ChoosePixelFormat(drawingContext, &pfd);
	}
	SetPixelFormat(drawingContext, format, &pfd);
	
	// Create an OpenGL context and make it the current context
	HGLRC glContext = wglCreateContext(drawingContext);
	wglMakeCurrent(drawingContext, glContext);

	return glContext;
}

bool CheckMultisample(HDC drawingContext)
{
	float fAttributes[] = {0, 0};
	int pixelFormat;
	UINT numFormats;
	int iAttributes[] = {
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_SAMPLE_BUFFERS_ARB, 1,
		WGL_SAMPLES_ARB, 4,
		0, 0 };

		if (wglChoosePixelFormatARB(drawingContext, iAttributes, fAttributes, 1, &pixelFormat, &numFormats)
			&& numFormats >= 1) {
				multisamplePixelFormat = pixelFormat;
				return true;
		} else {
			return false;
		}
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
