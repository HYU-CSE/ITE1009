#pragma once

#ifdef WIN32
#pragma warning ( disable : 4996) 
#include <Windows.h>
#endif

#include <stdlib.h>
#include <string>
#include <utility>
#include <functional>

using namespace std;

enum TYPE { NONE = 0, ELLIPSE, RECTANGLE, POLYGON, SPRITE, TEXT };

//swap arg1, arg2 by ref, return arg2
template <typename T>
T swap_ref(T& object, T& dest)
{
	object ^= dest ^= object ^= dest;
	return dest;
}

class typer {
public:
	int type;
	typer(int type) : type(type) {}
	~typer() {}
};

template <typename T>
class box : virtual public typer{
private:
public:
	T x, y, w, h;
	bool visible;
	box() : typer(RECTANGLE) {}
	box(const box& _box) : typer(RECTANGLE), x(_box.x), y(_box.y), w(_box.w), h(_box.h), visible(_box.visible) {}
	box(T x, T y, T w, T h, bool visible) : typer(RECTANGLE), x(x), y(y), w(w), h(h), visible(visible) {}
	~box() {}

	T movex(T dest)
	{ return swap_ref(this->x, dest); }
	T movey(T dest)
	{ return swap_ref(this->y, dest); }
	pair<T, T> move(pair<T, T> dest)
	{ return{ this->expandx(dest.first), this->expandy(dest.second) }; }

	T expandw(T dest)
	{ return swap_ref(this->w, dest); }
	T expandh(T dest)
	{ return swap_ref(this->h, dest); }
	pair<T, T> expand(pair<T, T> dest)
	{ return{ this->expandw(dest.first), this->expandh(dest.second) }; }
};

class callback {

};

class colored {
private:
public:
	long back_color;
	int back;
	long border_color;
	int border;
	colored() {}
	colored(int back, int border, long back_color, long border_color) :
		back(back), border(border), back_color(back_color), border_color(border_color){}
	void setInit(int back, int border)
	{
		this->back = back;
		this->border = border;
	}
	void setColor(long back_color, long border_color)
	{
		this->back_color = back_color;
		this->border_color = border_color;
	}
	~colored() {}
};

class object_image : public box<int> {
private:
public:
	int srcx, srcy;
	HBITMAP bitmap;
	object_image() : typer(SPRITE) {}
	object_image(HBITMAP bitmap) : typer(SPRITE), bitmap(bitmap) {}
	object_image(HBITMAP bitmap, int x, int y, int w, int h, bool visible) : typer(SPRITE), bitmap(bitmap), box<int>(x,y,w,h,visible) {}
	object_image(HBITMAP bitmap, int x, int y, int w, int h, bool visible, int srcx, int srcy) :
		typer(SPRITE), bitmap(bitmap), box<int>(x, y, w, h,visible), srcx(srcx), srcy(srcy) {}
	~object_image() {}
};

template<typename T>
class object_rectangle : public box<T>, public colored{
private:
public:
	object_rectangle() : typer(RECTANGLE) {}
	object_rectangle(T x, T y, T w, T h) :typer(RECTANGLE), box<T>(x, y, w, h) {}
	object_rectangle(T x, T y, T w, T h, bool visible, int back, int border, long back_color, long border_color) :
		typer(RECTANGLE), box<T>(x,y,w,h,visible), colored(back,border,back_color,border_color) {}
	~object_rectangle() {}
};

class object_ellipse : public box<int>, public colored{
private:
	int r;
public:
	void setR(int r)
	{
	}
	object_ellipse() : typer(ELLIPSE) {}
	~object_ellipse() {}

};

class object_text : public box<int> {
private:
public:
	int size;
	string text;
	object_text() : typer(TEXT) {}
	object_text(int x, int y, int w, int h, bool visible) : typer(TEXT), box<int>(x, y, w, h, visible) {}
	object_text(int x, int y, int w, int h, bool visible, int size, string text) :
		typer(TEXT), box<int>(x,y,w,h,visible), size(size), text(text) {}
	~object_text() {}
};

class object_button : public box<int>, public callback {
private:
public:
	object_button() :typer(RECTANGLE) {}
	~object_button() {}
};