#pragma once

#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

template<typename T>
class table {
private:
	unordered_map <string, T*> nameTable;
	unordered_map <size_t, T*> indexTable;
	vector<T> raw;
	vector<T>::iterator iter;

	template <typename type>
	bool tableInsert(unordered_map<type, T*>& objTable, const pair<type, T*>& object)
	{
		return objTable->insert(object);
	}
public:
	table() {}
	bool insert(T object)
	{
		bool err = true;
		this->raw.push_back(object);
		err |= !(this->tableInsert(this->nameTable, { this->raw.back().name, &(this->raw.back()) }));
		err |= !(this->tableInsert(this->indexTable, { raw.size() - 1, &(this->raw.back()) }));
		return err;
	}

	T find(size_t index)
	{
		return (*(this->indexTable.find(index)));
	}
	T find(string name)
	{
		return (*(this->nameTable.find(name)));
	}

	void initLoop()
	{
		iter = this->raw.begin();
	}
	T elementLoop()
	{
		if (iter == this->raw.end())
			return nullptr;
		T result = (*iter);
		iter++;
		return result;
	}

	bool remove(size_t index)
	{
		return this->remove(this->find(index));
	}
	bool remove(string name)
	{
		return this->remove(this->find(name));
	}
	bool remove(T object)
	{
		this->nameTable.erase(object.name);
		this->indexTable.erase(this->find)
	}
	

	~table() {}

};