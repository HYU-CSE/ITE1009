#include "drawing.h"

using namespace std;
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

	return{ (HPEN)SelectObject(dc, hPen), (HBRUSH)SelectObject(dc, hBrush) };
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
	BitBlt(*dc, x, y, w, h, *bDC, srcx, srcy, SRCCOPY);
	BitBlt(*dc, x, y, w, h, *bDC, srcx + w, srcy, SRCPAINT);
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
void drawText(HDC * dc, int x, int y, int w, int h, int size, string text, UINT flags)
{
	RECT rcDraw;
	HFONT	hFont = CreateFont(size, 0, 0, 0, 400, FALSE, FALSE, 0, ANSI_CHARSET, 0, 0, 0, 0 | FF_SWISS, TEXT("¸¼Àº°íµñ")),
		hOldFont = (HFONT)SelectObject(*dc, hFont);

	SetRect(&rcDraw, x, y, x + w, y + h);
	DrawText(*dc, text.c_str(), -1, &rcDraw, DT_HIDEPREFIX | flags);

	SelectObject(*dc, hOldFont);
	DeleteObject(hFont);
}
void drawText(HDC * dc, int x, int y, int w, int h, int size, string text)
{
	drawText(dc, x, y, w, h, size, text, 0);
}

void draw_loop_text(HDC * dc, table<object_text> table)
{
	object_text pivot = table.getPivot();
	FOREACH_TABLE(table, it, object_text)
		if (it->visible)
			drawText(dc, pivot.x + it->x, pivot.y + it->y, it->w, it->h, it->size, it->text);
}

void draw_loop_list(HDC * dc, listElements& list)
{
	musicInfo bfo = list.find(0);
	long preC = 0xdddddd, posC = 0xbbbbbb, sel = 0x35bd;
	string str_main, str_sub, str_right, bstr;
	FOREACH_LIST(list, i, info)
	{
		int yy = list.y + (i - 1) * list.margin + 30;

		str_main = info.name;
		switch (list.selector)
		{
		case 0:
			str_sub = info.genre;
			str_right = info.album;
			break;
		case 1:
			str_sub = info.artist;
			str_right = info.album;
			break;
		case 2:
			str_sub = info.artist;
			str_right = info.genre;
			break;
		}

		if ((list.selector && str_right != bstr) || !list.selector)
			preC ^= posC ^= preC ^= posC;

		if (list.select == i - 1)
			posC ^= sel;

		drawProc(dc, 0, yy + 1, SIZEW, list.margin, 1, 0, posC, 0, Rectangle);
		drawProc(dc, 0, yy + 1, SIZEW, list.margin, 1, 0, posC, 0, Rectangle);

		if (list.select == i - 1)
			posC ^= sel;

		MoveToEx(*dc, list.x, yy, NULL);
		LineTo(*dc, list.x + list.w, yy);


		drawText(dc, list.x + 10, yy + 8, list.w, yy + 32, 24, str_main);
		drawText(dc, list.x + 15, yy + 31, list.w, yy + 40, 14, str_sub);
		drawText(dc, list.x - 10, yy + 28, list.w, yy + 25, 16, str_right, DT_RIGHT);
		bstr = str_right;
	}
}