#include <windows.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <vector>
#include <Windowsx.h>

/*
1.	Написать приложение, обладающее следующей функциональностью:
•	при нажатии клавиши <Enter> главное окно позиционируется в левый верхний угол экрана с размерами (300х300) и начинает перемещаться по периметру экрана с определённой скоростью;
•	при нажатии клавиши <Esc> перемещение окна прекращается.

2.	Написать приложение, обладающее следующей функциональностью:
•	при последовательном нажатии клавиши <Enter> дочерние окна главно-го окна приложения «Калькулятор» минимизируются;
•	при последовательном нажатии клавиши <Esc> дочерние окна восста-навливаются в обратном порядке, т.е. дочернее окно, которое минимизи-ровалось последним, первым будет восстановлено.

*/

//Для удобства 2 задание я сделал через левую и правую кнопку мыши

typedef std::basic_string<TCHAR> tstring;
#ifdef _UNICODE
#define TO_TSTRING std::to_wstring
#else
#define TO_TSTRING std::to_string
#endif

BOOL CALLBACK EnumFunc(HWND hW, LPARAM Lp);
static TCHAR szWindowClass[] = _T("MyPerfectProgram");
static TCHAR szTitle[] = _T("Win32 Guided Tour Application");
HINSTANCE hInst;
std::vector<HWND> QueueWindow;
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
	TCHAR greeting[] = TEXT("Нажми \"enter\" - что-бы начать");

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		HWND hCalc = FindWindow(TEXT("CalcFrame"), TEXT("Калькулятор"));
		if (hCalc)
			EnumChildWindows(hCalc, EnumFunc, (LPARAM)hWnd);
	}
	break;
	case WM_RBUTTONDOWN:
	{
		while (!QueueWindow.empty())
		{
			ShowWindow(QueueWindow.back(), SW_SHOWNORMAL);  //SW_MAXIMIZE / SW_SHOWNOACTIVATE / SW_RESTORE
			QueueWindow.pop_back();
		}
	}
	break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_RETURN:
		{
			tstring str = TEXT("Нажмите ESC - что-бы выйти");
			SetWindowText(hWnd, str.c_str());
			MoveWindow(hWnd, 0, 0, 300, 300, TRUE);
			SetTimer(hWnd, 1, 2, NULL);
		}
			break;
		case VK_ESCAPE:
		{
			tstring str = TEXT("Нажмите ENTER - что-бы начать");
			SetWindowText(hWnd, str.c_str());
			KillTimer(hWnd, 1);
		}
			break;
		case VK_CONTROL:
		{
			HWND notePad = FindWindow(TEXT("Notepad"), TEXT("Безымянный — Блокнот"));
			EnumChildWindows(notePad, EnumFunc, lParam);
		}
		break;
		default:
			break;
		}
	}
	break;
	case WM_TIMER:
		{
			if (wParam == 1)
			{
				RECT r;
				GetWindowRect(hWnd, &r);
				int Horres = GetSystemMetrics(SM_CXSCREEN);
				int Vertres = GetSystemMetrics(SM_CYSCREEN);
				if (r.left == 0 && r.bottom != Vertres)
				{
					MoveWindow(hWnd, r.left, r.top + 5, r.right - r.left, r.bottom - r.top, TRUE);
					GetWindowRect(hWnd, &r);
				}
				else if (r.bottom == Vertres && r.right != Horres)
				{
					MoveWindow(hWnd, r.left + 5, r.top, r.right - r.left, r.bottom - r.top, TRUE);
					GetWindowRect(hWnd, &r);
				}
				else if (r.right == Horres && r.top != 0)
				{
					MoveWindow(hWnd, r.left, r.top - 5, r.right - r.left, r.bottom - r.top, TRUE);
					GetWindowRect(hWnd, &r);
				}
				else if (r.top == 0 && r.left != 0)
				{
					MoveWindow(hWnd, r.left - 5, r.top, r.right - r.left, r.bottom - r.top, TRUE);
					GetWindowRect(hWnd, &r);
				}
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

BOOL CALLBACK EnumFunc(HWND hW, LPARAM Lp)
{
	TCHAR buf[256];
	TCHAR bufClass[256];
	GetClassName(hW, bufClass, 256);
	tstring condition = bufClass;
	if (!condition.compare(_T("Button")))
	{
		GetWindowText(hW, buf, 256);
		QueueWindow.push_back(hW);
		ShowWindow(hW, SW_MINIMIZE);
	}

	return 1;
}