#pragma once

#ifdef WIN32
#pragma warning ( disable : 4996) 
#endif

#include <stdlib.h>
#include <string>
#include <utility>
#include <functional>

using namespace std;


//swap arg1, arg2 by ref, return arg2
template <typename T>
T swap_ref(T& object, T& dest)
{
	object ^= dest ^= object ^= dest;
	return dest;
}

template <typename T>
class box {
private:
public:
	T x, y, w, h;
	box() {}
	box(const box& _box) : x(_box.x), y(_box.y), w(_box.w), h(_box.h) {}
	box(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {}
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

class BMP {
private:
public:
	BMP() {}
	~BMP() {}
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

class object_raw : public box<int>{
private:

public:
	size_t idx;

};

template<typename T>
class object_rectangle : public box<T>, public colored{
private:
public:
	object_rectangle() {}
	object_rectangle(T x, T y, T w, T h) : box<T>(x, y, w, h) {}
	object_rectangle(T x, T y, T w, T h, int back, int border, long back_color, long border_color) :
		box<T>(x,y,w,h), colored(back,border,back_color,border_color) {}
	~object_rectangle() {}
};

class object_ellipse : public box<int>, public colored{
private:
	int r;
public:
	void setR(int r)
	{
	}
	object_ellipse() {}
	~object_ellipse() {}

};

class object_text : public object_raw {
private:
public:
	object_text() {}
	~object_text() {}
};
class object_image : public object_raw, public BMP {
private:
public:
	object_image() {}
	~object_image() {}
};
class object_button : public object_raw, public callback {
private:
public:
	object_button() {}
	~object_button() {}
};