#include <windows.h>
#include <Windowsx.h> 
#include <tchar.h>
#include <string>
#include "resource.h"
#include <cstdlib>
#include <ctime>
/*1.	Разработать приложение «убегающий статик». Суть приложения: на форме диалогового окна расположен статический элемент управления.
Пользователь пытается подвести курсор мыши к «статику»,
 и, если курсор находится близко со «статиком», элемент управления «убегает». Предусмотреть перемещение «статика» только в пределах диалогового окна.
2.	Разработать диалоговое приложение, позволяющее перетаскивать мышью «статик»,  расположенный на форме.
*/

typedef std::basic_string<TCHAR> tstring;
#ifdef _UNICODE
#define TO_TSTRING std::to_wstring
#else
#define TO_TSTRING std::to_string
#endif

HWND hStatic, hCoord, hBtnMove;
HINSTANCE hInst;
BOOL ClickButton = FALSE;
void MyMoveFunc(RECT, RECT, INT, INT);
WNDPROC OriginalProc;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
	srand(std::time(NULL));
	hInst = hInstance;
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}
BOOL CALLBACK  MoveWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		//фиксируем buttondown
	}
		return TRUE;

	default:
		break;
	}
	return CallWindowProc(OriginalProc, hWnd, message, wParam, lParam);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONUP:
	{
		RECT rBtnMove;
		GetWindowRect(hBtnMove, &rBtnMove);
		INT xPos1 = GET_X_LPARAM(lParam);
		INT yPos1 = GET_Y_LPARAM(lParam);
		MoveWindow(hBtnMove, xPos1, yPos1, rBtnMove.right - rBtnMove.left, rBtnMove.bottom - rBtnMove.top, TRUE);
	}
	return TRUE;
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	case WM_INITDIALOG:
	{
		hStatic = GetDlgItem(hWnd, IDC_BUTTON1);
		hBtnMove = GetDlgItem(hWnd, IDC_BUTTON2);
		hCoord = CreateWindow(TEXT("STATIC"), 0, WS_CHILD | WS_VISIBLE | SS_CENTER, 20, 40, 120, 20, hWnd, 0, hInst, 0);
		OriginalProc = (WNDPROC)SetWindowLong(hBtnMove, GWL_WNDPROC, LONG(MoveWindowProc));
	}
	return TRUE;
	case WM_MOUSEMOVE:
	{
		RECT rtChild, rClientArea;
		INT mouseX = LOWORD(lParam);
		INT mouseY = HIWORD(lParam);
		GetWindowRect(hStatic, &rtChild);
		GetClientRect(hWnd, &rClientArea);
		MapWindowPoints(NULL, hWnd, (LPPOINT)&rtChild, 2);
		if (mouseX > rtChild.right)
		{
			if (mouseX - 10 <= rtChild.right)
			{
				if (mouseY >= rtChild.top && mouseY <= rtChild.bottom)
				{
					MyMoveFunc(rtChild, rClientArea, mouseX, mouseY);
				}
			}
		}
		else if (mouseX < rtChild.left)
		{
			if (mouseX + 10 >= rtChild.left)
			{
				if (mouseY >= rtChild.top && mouseY <= rtChild.bottom)
				{
					MyMoveFunc(rtChild, rClientArea, mouseX, mouseY);
				}
			}
		}

		if (mouseY > rtChild.bottom)
		{
			if (mouseY - 10 <= rtChild.bottom)
			{
				if (mouseX >= rtChild.left && mouseX <= rtChild.right)
				{
					MyMoveFunc(rtChild, rClientArea, mouseX, mouseY);
				}
			}
		}
		else if (mouseY < rtChild.top)
		{
			if (mouseY + 10 >= rtChild.top)
			{
				if (mouseX >= rtChild.left && mouseX <= rtChild.right)
				{
					MyMoveFunc(rtChild, rClientArea, mouseX, mouseY);
				}
			}
		}
	}
	return TRUE;
	}
	return FALSE;
}
void MyMoveFunc(RECT rtChild, RECT rClientArea, INT mouseX, INT mouseY)
{
	INT cordX = rand() % rClientArea.right;
	INT cordY = rand() % rClientArea.bottom;
	while (cordX < mouseX && cordY < mouseY)
	{
		cordX = rand() % rClientArea.right;
		cordY = rand() % rClientArea.bottom;
	}
	MoveWindow(hStatic, cordX, cordY, rtChild.right - rtChild.left, rtChild.bottom - rtChild.top, TRUE);
}