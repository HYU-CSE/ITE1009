#pragma once

#ifdef WIN32
#pragma warning ( disable : 4996) 
#endif

#include <stdlib.h>
#include <string>
#include <utility>
#include <functional>

using namespace std;
template <typename T>
class box {
private:
	T x, y, w, h;
	function<T(T&, T&)> swap = [](T& object, T& dest)->T { 
		object ^= dest ^= object ^= dest; 
		return dest; };
public:
	box() {}
	box(T x, T y, T w, T h) : this->x(x), this->y(y), this->w(w), this->h(h) {}
	~box() {}

	T movex(T dest)
	{ return this->swap(this->x, dest); }
	T movey(T dest)
	{ return this->swap(this->y, dest); }
	pair<T, T> move(pair<T, T> dest)
	{ return{ this->expandx(dest.first), this->expandy(dest.second) }; }

	T expandw(T dest)
	{ return this->swap(this->w, dest); }
	T expandh(T dest)
	{ return this->swap(this->h, dest); }
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

class object_raw : public box<int>{
private:

public:
	size_t idx;

};

class object_polygon : public object_raw
{
private:
public:
	object_polygon() {}
	~object_polygon() {}

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