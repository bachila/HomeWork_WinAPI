#include <windows.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <tchar.h>
#include <sstream>
#include <Windowsx.h>

/*1)	Написать приложение, позволяющее при нажатии левой кнопки мыши изменить текст в заголовке окна стандартного приложения «Калькулятор»,
а при нажатии правой кнопки мыши сместить вправо кнопку «пуск», изменив на ней надпись.
2)	Написать приложение, обладающее следующей функциональностью :
•	при нажатии кнопки <Enter> окно позиционируется в левый верхний угол экрана с размерами(300Х300);
•	с помощью клавиш управления курсором осуществляется перемещение окна.*/


typedef std::basic_string<TCHAR> tstring;
#ifdef _UNICODE
#define TO_TSTRING std::to_wstring
#else
#define TO_TSTRING std::to_string
#endif

static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("Win32 Guided Tour Application");
HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI _tWinMain(HINSTANCE hInstance /*дескриптор текущего приложения*/, HINSTANCE hPrevInstance/*дескриптор предыдущего приложения*/, LPTSTR lpCmdLine/*переданные параметры программе*/, INT nCmdShow/*возможность отображение консоли*/)
{
	WNDCLASSEX wcex; /*класс окна, создается обьект окна с параметрами*/
					 /*описсание окна*/
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));


	/*регистрация приложения*/
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	hInst = hInstance;

	/*создание окна*/
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 100,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	/*показывают и обнавляют окно*/
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	/*цикл обработки сообщений*/
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

/*Оконная процедура*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = TEXT("Hello world!");

	switch (message)
	{

	case WM_KEYDOWN:
		{
			RECT r;
			GetWindowRect(hWnd, &r);
			switch (wParam)
			{
			case VK_RETURN:
				MoveWindow(hWnd, 0, 0, 300, 300, TRUE);
				break;
			case VK_DOWN:
				MoveWindow(hWnd, r.left, r.top + 10, r.right - r.left, r.bottom - r.top, TRUE);
				break;
			case VK_UP:
				MoveWindow(hWnd, r.left, r.top - 10, r.right - r.left, r.bottom - r.top, TRUE);
				break;
			case VK_RIGHT:
				MoveWindow(hWnd, r.left + 10, r.top, r.right - r.left, r.bottom - r.top, TRUE);
				break;
			case VK_LEFT:
				MoveWindow(hWnd, r.left - 10, r.top, r.right - r.left, r.bottom - r.top, TRUE);
				break;
			default:
				break;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			HWND notePad = FindWindow(TEXT("Notepad"),NULL);  // В windows 10 с калькулятором не работает, я сделал с блокнотом
			if (notePad)
				SetWindowText(notePad, TEXT("NewTextNotePad"));
		}
		break;

	case WM_RBUTTONDOWN:
		{
			RECT r;

			HWND parrent = FindWindow(TEXT("Shell_TrayWnd"), NULL);
			HWND start = FindWindowEx(parrent, NULL, TEXT("Start"), TEXT("Пуск"));
			GetWindowRect(start, &r);
			if (start)
			{
				//MoveWindow(start, 100, 100, 25, 25, TRUE);
				SetWindowText(start, TEXT("New text Start"));
			}
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// Here your application is laid out.
		// For this introduction, we just print out "Hello, World!"
		// in the top left corner.
		TextOut(hdc,
			5, 5,
			greeting, _tcslen(greeting));
		// End application-specific layout section.

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}