#include "main.h"
#include "object.h"
#include "table.h"

HINSTANCE nInst;


int frame = 0;

table<object_rectangle<int>> topBar, botBar;
table<object_image> botIcon, topIcon;
table<object_text> topBarText;

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
pair<HPEN, HBRUSH> draw_init(HDC& dc, HPEN& hPen, HBRUSH& hBrush, int back, int border, int back_color, int border_color)
{
	if (!border)
		hPen = (HPEN)GetStockObject(NULL_PEN);
	else
		hPen = CreatePen(0, border, border_color);

	if (!back)
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	else
		hBrush = CreateSolidBrush(back_color);

	return {(HPEN)SelectObject(dc, hPen), (HBRUSH)SelectObject(dc, hBrush) };
}
void draw_delete(HDC& dc, HPEN& hPen, HBRUSH& hBrush, pair<HPEN, HBRUSH> old)
{
	SelectObject(dc, old.first);
	DeleteObject(hPen);
	SelectObject(dc, old.second);
	DeleteObject(hBrush);
}

void drawBlt(HDC * dc, int x, int y, int w, int h, int srcx, int srcy, HDC * bDC)
{
	BitBlt(*dc, x, y, w, h, *bDC, srcx , srcy, SRCCOPY);
	BitBlt(*dc, x, y, w, h, *bDC, srcx + w , srcy, SRCPAINT);
}
void drawProc(HDC * dc, int x, int y, int w, int h, int back, int border, int back_color, int border_color,
	function<bool(HDC, int, int, int, int)> draw)
{
	HPEN hPen;
	HBRUSH hBrush;
	pair<HPEN, HBRUSH> old = draw_init(*dc, hPen, hBrush, back, border, back_color, border_color);

		draw(*dc, x, y, x + w, y + h);

	draw_delete(*dc, hPen, hBrush, old);
}
void drawText(HDC * dc, int x, int y, int w, int h, int size, string text)
{
	RECT rcDraw;
	HFONT	hFont = CreateFont(size, 0, 0, 0, 400, FALSE, FALSE, 0, ANSI_CHARSET, 0, 0, 0, 0 | FF_SWISS, TEXT("¸¼Àº°íµñ")),
			hOldFont = (HFONT)SelectObject(*dc, hFont);

	SetRect(&rcDraw, x, y, x + w, y + h);
	DrawText(*dc, text.c_str(), -1, &rcDraw, DT_HIDEPREFIX);

	SelectObject(*dc, hOldFont);
	DeleteObject(hFont);
}


void draw_loop_text(HDC * dc, table<object_text> table)
{
	object_text pivot = table.getPivot();
	FOREACH_TABLE(table, it, object_text)
		if(it->visible)
			drawText(dc, pivot.x + it->x, pivot.y + it->y, it->w, it->h, it->size, it->text);
}
template <typename T>
void draw_loop_blt(HDC * dc, table<T> table)
{
	T pivot = table.getPivot();
	HDC bDC = CreateCompatibleDC(*dc);
	SelectObject(bDC, pivot.bitmap);

	FOREACH_TABLE(table, it, T)
		if(it->visible)
			drawBlt(dc, it->x + pivot.x, it->y + pivot.y, it->w, it->h, it->srcx, it->srcy, &bDC);

	DeleteDC(bDC);
}
template <typename T>
void draw_loop_proc(HDC * dc, table<T> table)
{
	function <bool(HDC, int, int, int, int)> proc;

	FOREACH_TABLE(table, it, T)
	{
		if (it->visible)
		{
			switch (it->type)
			{
			case ELLIPSE:
				proc = Ellipse;
				break;
			case RECTANGLE:
				proc = Rectangle;
				break;
			default:
				break;
			}
			drawProc(dc, it->x, it->y, it->w, it->h, it->back, it->border, it->back_color, it->border_color, proc);
		}
	}
}
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			object_rectangle<int>	tbox(-2, 1, SIZEW + 4, 40, true, 0, 2, 0xffffff, 0x00000),
									bbox(-2, SIZEH-60, SIZEW + 4, 60, true, 0, 2, 0xffffff, 0x00000);

			topBar.setVisible(true);
			botBar.setVisible(true);
			topBar.setPivot(tbox);
			botBar.setPivot(bbox);

			object_text textbox(-2, 1, SIZEW + 4, 50, true, 0, ""),
						nowPlay(95, 9, 150, 25, true, 22, "Áö±Ý Àç»ýÁßÀÎ À½¾Ç");

			topBarText.setPivot(textbox);
			topBarText.insert(nowPlay);

			object_image botIconMap(LoadBitmap(nInst, MAKEINTRESOURCE(MENUICONS)), 0, SIZEH-60, 0, 0, false),
				iMusic(NULL, 40, 5, 50, 50, true, 0, 0), iPlaylist(NULL, 130, 5, 50, 50, true, 100, 0),
				iGenre(NULL, 220, 5, 50, 50, true, 0, 50), iAlbum(NULL, 310, 5, 50, 50, true, 100, 50);
			botIcon.setVisible(false);
			botIcon.setPivot(botIconMap);
			botIcon.insert(iMusic);
			botIcon.insert(iPlaylist);
			botIcon.insert(iGenre);
			botIcon.insert(iAlbum);

			object_image topIconMap(LoadBitmap(nInst, MAKEINTRESOURCE(TOPBARICONS)), 0, 5, 0, 0, false),
				iPlay(NULL, 5, 0, 30, 30, false, 0, 0), iPause(NULL, 5, 0, 30, 30, true, 60, 0),
				iNext(NULL, 45, 0, 30, 30, true, 0, 30), iSearch(NULL, SIZEW-35, 0, 30, 30, true, 60, 30);
			topIcon.setVisible(false);
			topIcon.setPivot(topIconMap);
			topIcon.insert(iPlay);
			topIcon.insert(iPause);
			topIcon.insert(iNext);
			topIcon.insert(iSearch);

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
				draw_loop_text(&mDC, topBarText);
				draw_loop_blt(&mDC, topIcon);
				draw_loop_proc(&mDC, topBar);

				draw_loop_blt(&mDC, botIcon);
				draw_loop_proc(&mDC, botBar);
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

