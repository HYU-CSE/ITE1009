#pragma once

#ifdef WIN32
#pragma warning (disable: 4996)
#include <Windows.h>
#endif

#include <utility>
#include "object.h"
#include "table.h"
#include "musicList.h"
#include "main.h"

using namespace std;

pair<HPEN, HBRUSH> draw_init(HDC&, HPEN&, HBRUSH&, int, int, int, int);
void draw_delete(HDC&, HPEN&, HBRUSH&, pair<HPEN, HBRUSH>);

void drawBlt(HDC *, int, int, int, int, int, int,HDC *);
void drawProc(HDC *, int, int, int, int, int, int, int, int, 
	function<bool(HDC, int, int, int, int)>);
void drawText(HDC *, int, int, int, int, int, string, UINT);
void drawText(HDC *, int, int, int, int, int, string);

void draw_loop_text(HDC *, table<object_text>);
void draw_loop_list(HDC *, listElements&);

template <typename T>
void draw_loop_blt(HDC * dc, table<T> table)
{
	T pivot = table.getPivot();
	HDC bDC = CreateCompatibleDC(*dc);
	SelectObject(bDC, pivot.bitmap);

	FOREACH_TABLE(table, it, T)
		if (it->visible)
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