#pragma once

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

#define FOREACH_TABLE(table, iter, type)  for(vector<type>::iterator (iter) = (table).raw.begin() + ((table).getVisible() ? 0 : 1); (iter) != (table).raw.end(); (iter)++)

template<typename T>
class table {
private:
	unordered_map <size_t, T*> indexTable;
	typename vector<T>::iterator iter;
	bool visible;

public:
	vector<T> raw;
	int type;
	table() 
	{
		T temp;
		this->raw.push_back(temp);
		this->indexTable.insert({ raw.size() - 1, &(this->raw.back()) });
		this->type = temp.type;
	}
	void setPivot(T object)
	{
		this->change(object, 0);
	}
	T getPivot()
	{
		return this->raw.at(0);
	}

	void setVisible(bool visible)
	{
		this->visible = visible;
	}
	bool getVisible()
	{
		return this->visible;
	}

	int insert(T object)
	{
		T temp = object;
		this->raw.push_back(temp);
		this->indexTable.insert({ this->raw.size() - 1, &(this->raw.back()) });
		return raw.size()-1;
	}
	T& find(size_t index)
	{
		if(index < this->raw.size())
			return this->raw.at(index);
	}
	void change(T object, size_t index)
	{
		this->raw.at(index) = object;
		typename unordered_map<size_t, T*>::iterator iter = this->indexTable.find(index);
		iter->second = &(this->raw.at(index));
	}

	bool remove(size_t index)
	{
		return this->remove(this->find(index));
	}
	bool remove(T object)
	{
		this->indexTable.erase(this->find)
	}
	~table() {}
};