#pragma once

#include <unordered_map>
#include <string>
#include <vector>
using namespace std;

typedef struct MusicInfo {
	string name, album, artist;
public:
	MusicInfo() {}
	MusicInfo(const MusicInfo& info) : name(info.name), album(info.album), artist(info.artist) {}
	MusicInfo(string name, string album, string artist) : name(name), album(album), artist(artist) {}
	bool operator== (const MusicInfo& info) const
	{
		return this->name == info.name && this->album == info.album && this->artist == info.artist;
	}
	bool operator() (MusicInfo& inf1, MusicInfo& inf2)
	{
		return inf1 == inf2;
	}
}musicInfo;

musicInfo createInfo(string name, string album, string artist)
{
	musicInfo temp(name, album, artist);
	return temp;
}

static musicInfo nullInfo("", "", "");
#define nInfo nullInfo

namespace std {
	template<>
	struct hash<musicInfo>
	{
		std::size_t operator()(const musicInfo& info) const {
			return ((hash<string>()(info.name) ^ (hash<string>()(info.album) << 4) >> 4) ^ hash<string>()(info.artist));
		}
	};
}

#define FOREACH_LIST(list, i, info) musicInfo info = list.find(0); for(size_t i = 0;i < list.size(); info = list.find(i),i++)

class listElements {
private:
	vector<size_t> nVector;
	vector<musicInfo> raw;
	unordered_map<string, vector<size_t>> tableName, tableAlbum, tableArtist;
	unordered_map<musicInfo, size_t> table;

	size_t findIndex(const musicInfo& info)
	{
		unordered_map<musicInfo, size_t> ::const_iterator iter = this->table.find(info);
		if (iter != this->table.end())
			return iter->second;
		return SIZE_MAX;
	}
	template <typename T>
	vector<musicInfo> find(const unordered_map<T, vector<size_t>>& uMAP, const T& str)
	{
		vector<musicInfo> result;
		typename unordered_map<T, vector<size_t>>::const_iterator iter = uMAP.find(str);
		if (iter != uMAP.end())
		{
			for (size_t idx : iter->second)
				result.push_back(this->raw.at(idx));
		}
		return result;
	}
	template <typename T>
	void tableErase(unordered_map<T, vector<size_t>>& uMAP, const T& str, size_t idx)
	{
		typename unordered_map<T, vector<size_t>>::iterator iter = uMAP.find(str);
		if (iter != uMAP.end())
			for (size_t i = 0; i < iter->second.size(); i++)
				if (iter->second[i] == idx)
				{
					iter->second.erase(iter->second.begin() + idx);
					return;
				}
	}
	template <typename T>
	void tableInsert(unordered_map<T, vector<size_t>>& uMAP, const T& str, size_t idx)
	{
		typename unordered_map<T, vector<size_t>>::iterator iter = uMAP.find(str);
		if (iter != uMAP.end())
			iter->second.push_back(idx);
		else
		{
			vector<size_t> temp(1, idx);
			uMAP.insert({ str, temp });
		}
	}
public:
	int x, y, w, h, margin = 50;

	listElements() {}
	~listElements() {}
	int maxY()
	{
		return (this->raw.size() - 2) * this->margin - (this->margin/(1.3));
	}
	size_t size()
	{
		return this->raw.size();
	}
	void insert(const musicInfo& info)
	{
		musicInfo temp(info);
		this->raw.push_back(temp);
		this->table.insert({ temp, this->raw.size() - 1 });
		this->tableInsert(this->tableName, temp.name, this->raw.size() - 1);
		this->tableInsert(this->tableAlbum, temp.album, this->raw.size() - 1);
		this->tableInsert(this->tableArtist, temp.artist, this->raw.size() - 1);
	}
	musicInfo find(size_t idx)
	{
		if (idx < this->raw.size())
			return this->raw.at(idx);
		return nInfo;
	}
	vector<musicInfo> findName(string name)
	{
		return this->find(tableName, name);
	}
	vector<musicInfo> findAlbum(string album)
	{
		return this->find(tableAlbum, album);
	}
	vector<musicInfo> findArtist(string artist)
	{
		return this->find(tableArtist, artist);
	}
	void remove(musicInfo info)
	{
		size_t idx = this->findIndex(info);
		musicInfo temp = this->raw.at(idx);
		this->table.erase(temp);
		this->tableErase(this->tableName, temp.name, idx);
		this->tableErase(this->tableAlbum, temp.album, idx);
		this->tableErase(this->tableArtist, temp.artist, idx);
		this->raw.erase(this->raw.begin() + idx);
	}
};