#pragma once

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <functional>
#include "object.h"
using namespace std;

#define FOREACH_TABLE(table, iter, type)  for(vector<type>::iterator (iter) = (table).raw.begin() + ((table).getVisible() ? 0 : 1); (iter) != (table).raw.end(); (iter)++)

template<typename T>
class table {
private:
	T nullity;
	unordered_map <size_t, T*> indexTable;
	typename vector<T>::iterator iter;
	bool visible;

public:
	vector<T> raw;
	int type;
	table();

	void setPivot(T);
	T getPivot();

	void setVisible(bool);
	bool getVisible();

	T& find(size_t);
	int insert(T);
	void change(T, size_t);

	bool remove(size_t);
	bool remove(T);
	~table() {}
};
//
/*---------------------------------------------------------*/
//
template <typename  T>
table<T>::table()
{
	T temp;
	this->raw.push_back(temp);
	this->indexTable.insert({ raw.size() - 1, &(this->raw.back()) });
	this->type = temp.type;
	this->nullity = temp;
}

template<typename T>
void table<T>::setPivot(T object)
{
	this->change(object, 0);
}
template<typename T>
T table<T>::getPivot()
{
	return this->raw.at(0);
}

template<typename T>
void table<T>::setVisible(bool visible)
{
	this->visible = visible;
}
template<typename T>
bool table<T>::getVisible()
{
	return this->visible;
}

template<typename T>
int table<T>::insert(T object)
{
	T temp = object;
	this->raw.push_back(temp);
	this->indexTable.insert({ this->raw.size() - 1, &(this->raw.back()) });
	return raw.size() - 1;
}

template<typename T>
T& table<T>::find(size_t index)
{
	if (index < this->raw.size())
		return this->raw.at(index);
	return nullity;
}

template<typename T>
void table<T>::change(T object, size_t index)
{
	this->raw.at(index) = object;
	typename unordered_map<size_t, T*>::iterator iter = this->indexTable.find(index);
	iter->second = &(this->raw.at(index));
}

template <typename T>
bool table<T>::remove(size_t index)
{
	return this->remove(this->find(index));
}
template <typename T>
bool table<T>::remove(T object)
{
	this->indexTable.erase(this->find)
}