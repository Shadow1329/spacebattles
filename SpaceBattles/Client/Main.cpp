#include "CConnectionManager.h"
#include "CGraphicsManager.h"
#include "CControlManager.h"

#include <windows.h>



//переменные
HWND g_hWnd = NULL;




//объявление функций
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);  // Создание окна
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);       // Функция окна








//Main func
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//Init window
	if (InitWindow(hInstance, nCmdShow) == 0)
	{
		MessageBox(NULL, L"Window init error", L"Error!", MB_ICONERROR);
		return 0;
	}


	//Init graphics
	if (CGraphicsManager::getInstance()->Init(g_hWnd) == 0) return 0;
	if (CGraphicsManager::getInstance()->Reset() == 0) return 0;

	//Create connection
	CConnectionManager::getInstance()->Init();

	//Main message loop
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//Render
			CGraphicsManager::getInstance()->Render();
		}
	}


	//Release Direct3D interfaces
	CGraphicsManager::getInstance()->Release();


	return (int)msg.wParam;
}




//регистрация класса и создание окна
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Регистрация класса
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = (HBRUSH)(WHITE_PEN);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"SpaceBattleClass";
	wc.lpszMenuName = NULL;

	if (!RegisterClassEx(&wc)) return 0;

	// Создание окна
	g_hWnd = CreateWindow(wc.lpszClassName,
		L"Space Battle",
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		800,
		600,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (!g_hWnd) return 0;

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	return 1;
}




//Обработка сообщений окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		CControlManager::getInstance()->setKeyState(wParam, true);
		break;

	case WM_KEYUP:
		CControlManager::getInstance()->setKeyState(wParam, false);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}




