#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

typedef struct MusicInfo {
	string name, album, artist, genre;
public:
	MusicInfo() {}
	MusicInfo(const MusicInfo& info) : name(info.name), album(info.album), artist(info.artist), genre(info.genre) {}
	MusicInfo(string name, string album, string artist, string genre) : name(name), album(album), artist(artist), genre(genre) {}
	bool operator== (const MusicInfo& info) const {
		return this->name == info.name && this->album == info.album && this->artist == info.artist && this->genre == info.genre;
	}
	bool operator< (const MusicInfo& info) const {
		return (this->name  + this->album  + this->artist + this->genre) < (info.name  + info.album  + info.artist + this->genre);
	}
	bool operator() (MusicInfo& inf1, MusicInfo& inf2) {
		return inf1 == inf2;
	}
}musicInfo;


musicInfo createInfo(string name, string album, string artist, string genre);

static musicInfo nullInfo("", "", "", "");
#define nInfo nullInfo

namespace std {
	template<>
	struct hash<musicInfo>
	{
		std::size_t operator()(const musicInfo& info) const {
			return ((hash<string>()(info.name) ^ (hash<string>()(info.album) << 4) >> 4) ^ hash<string>()(info.artist));
		}
	};
	template<>
	struct less<musicInfo>
	{
		bool operator()(const musicInfo& inf1, const musicInfo& inf2) const {
			return inf1 < inf2;
		}
	};
}

static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(isspace))));
	return s;
}
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(isspace))).base(), s.end());
	return s;
}

#define FOREACH_LIST(list, i, info)\
	musicInfo (info) = ((list).find(0));\
	for(size_t (i) = 1; (i) <= ((list).size()); (info) = ((list).find((i))),((i)++))

class listElements {
private:
	vector<size_t> nVector;
	vector<musicInfo> raw;
	vector<pair<musicInfo, size_t>> sorted;
	unordered_map<string, vector<size_t>> tableName, tableAlbum, tableArtist;
	unordered_map<musicInfo, size_t> table;

	size_t findSortedIndex(const musicInfo&);
	size_t findIndex(const musicInfo&);

	template <typename T>
	vector<musicInfo> find(const unordered_map<T, vector<size_t>>& uMAP, T str)
	{
		transform(str.begin(), str.end(), str.begin(), toupper);
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
	void tableErase(unordered_map<T, vector<size_t>>& uMAP, size_t idx)
	{
		for (typename unordered_map<T, vector<size_t>>::iterator iter = uMAP.begin(); iter != uMAP.end(); iter++)
			for (vector<size_t>::iterator it = iter->second.begin(); it != iter->second.end(); it++)
				if ((*it) == idx)
				{
					iter->second.erase(it);
					if (iter->second.empty())
						iter = uMAP.erase(iter);
					break;
				}
	}
	void tableInsertString(unordered_map<string, vector<size_t>>&, string, size_t);
	void tableInsert(unordered_map<string, vector<size_t>>&, const string&, size_t);
public:
	size_t select = 0;
	bool selector = false;
	int x, y, w, h, margin = 50;

	listElements(bool stor) : selector(stor) {}
	~listElements() {}

	int maxY();
	size_t size();

	void sort();
	size_t insert(const musicInfo&);
	void remove(const musicInfo&);

	musicInfo find(size_t);
	vector<musicInfo> findName(string);
	vector<musicInfo> findAlbum(string);
	vector<musicInfo> findArtist(string);

	void clear();
};