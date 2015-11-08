#pragma once

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

#define FOREACH_TABLE(table, iter, type)  for(vector<type>::iterator (iter) = (table).raw.begin(); (iter) != (table).raw.end(); (iter)++)



template<typename T>
class table {
private:
	int x, y;
	
	unordered_map <size_t, T*> indexTable;
	typename vector<T>::iterator iter;

	template <typename type>
	void tableInsert(unordered_map<type, T*>& objTable, const pair<type, T*>& object)
	{
		objTable.insert(object);
	}
public:
	vector<T> raw;
	table() {}
	table(int x, int y) : this->x(x), this->y(y) {}
	int setx(int x)
	{
		return swap_ref(this->x, x);
	}
	int sety(int y)
	{
		return swap_ref(this->y, y);
	}
	pair<int, int> setxy(int x, int y)
	{
		return{ this->setx(x), this->sety(y) };
	}
	bool insert(T object)
	{
		T temp = object;
		bool err = true;
		this->raw.push_back(temp);
		this->tableInsert(this->indexTable, { raw.size() - 1, &(this->raw.back()) });
		return err;
	}

	T find(size_t index)
	{
		return (*(this->indexTable.find(index)));
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