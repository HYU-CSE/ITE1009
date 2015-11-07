#include "main.h"
#include "object.h"

HINSTANCE nInst;

HBITMAP topIcons[3];

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG messages;
	WNDCLASSEX win_main;

	SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);

	HICON hICON = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	win_main.hInstance = nInst = hInstance;
	win_main.style = CS_HREDRAW | CS_VREDRAW;
	win_main.cbSize = sizeof(WNDCLASSEX);
	win_main.cbClsExtra = 0;
	win_main.cbWndExtra = 0;
	win_main.lpszClassName = TITLE;
	win_main.lpszMenuName = NULL;
	win_main.lpfnWndProc = WindowProcedure;
	win_main.hIcon = hICON;
	win_main.hIconSm = hICON;
	win_main.hCursor = LoadCursor(NULL, IDC_ARROW);
	win_main.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	if (!RegisterClassEx(&win_main))
		return 0;

	hwnd = CreateWindowEx(0, TITLE, TITLE, WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT,
		CW_USEDEFAULT, SIZEWW, SIZEHH, HWND_DESKTOP, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);

	ZeroMemory(&messages, sizeof(messages));
	while (messages.message != WM_QUIT)
	{
		if (PeekMessage(&messages, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&messages);
			DispatchMessage(&messages);
		}
	}
	return messages.wParam;
}
void draw_rectangle(HDC * dc, int x, int y, int w, int h, int border,int back, int borderCol, int backCol)
{
	HPEN hPen, hOldPen;
	HBRUSH hBrush, hOldBrush;

	if (!border)
		hPen = (HPEN)GetStockObject(NULL_PEN);
	else
		hPen = CreatePen(0, border, borderCol);

	if (!back)
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	else
		hBrush = CreateSolidBrush(backCol);

	hOldPen = (HPEN)SelectObject(*dc, hPen);
	hOldBrush = (HBRUSH)SelectObject(*dc, hBrush);

		Rectangle(*dc, x, y, x + w, y + h);

	SelectObject(*dc, hOldPen);
	DeleteObject(hPen);
	SelectObject(*dc, hOldBrush);
	DeleteObject(hBrush);
}
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			
			for (int i = 0; i < 3; i++)
				topIcons[i] = LoadBitmap(nInst, MAKEINTRESOURCE(1001 + i));
			break;
		}
		case WM_SIZE:
		{
			break;
		}
		case WM_MOUSEWHEEL:
		{
			break;
		}
		case WM_KEYDOWN:
		{
			break;
		}
		case WM_LBUTTONDOWN:
		{

			break;
		}
		case WM_COMMAND:
		{
			break;
		}
		case WM_MOUSEMOVE:
		{
			point.x = lParam & 0xffff;
			if (point.x > 60000) point.x = 0;
			point.y = lParam >> 16;
			//if (!mainPage)
				//InvalidateRect(hwnd, NULL, FALSE);
			break;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			RECT		mRC;
			HDC			hdc, mDC;
			HBITMAP		mBitmap, mOldBitmap;

			hdc = BeginPaint(hwnd, &ps);

			GetClientRect(hwnd, &mRC);
			mDC = CreateCompatibleDC(hdc);
			mBitmap = CreateCompatibleBitmap(hdc, mRC.right, mRC.bottom);
			mOldBitmap = (HBITMAP)SelectObject(mDC, mBitmap);
			PatBlt(mDC, 0, 0, mRC.right, mRC.bottom, WHITENESS);
			SetBkMode(mDC, TRANSPARENT);
			SetStretchBltMode(mDC, COLORONCOLOR);
			{//draw

			}
			BitBlt(hdc, 0, 0, mRC.right, mRC.bottom, mDC, 0, 0, SRCCOPY);
			SelectObject(mDC, mOldBitmap);
			DeleteObject(mBitmap);
			DeleteDC(mDC);

			EndPaint(hwnd, &ps);
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

