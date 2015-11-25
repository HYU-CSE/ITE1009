#include "musicList.h"

musicInfo createInfo(string name, string album, string artist, string genre)
{
	musicInfo temp(name, album, artist, genre);
	return temp;
}

size_t listElements::findIndex(const musicInfo& info)
{
	for (size_t i = 0; i < this->raw.size(); i++)
		if (this->raw[i] == info)
			return i;
	return -1;
}
void listElements::tableInsertString(unordered_map<string, vector<size_t>>& uMAP, string str, size_t idx)
{
	transform(str.begin(), str.end(), str.begin(), toupper);
	unordered_map<string, vector<size_t>>::iterator iter = uMAP.find(str);
	if (iter != uMAP.end())
	{
		if(std::find(iter->second.begin(), iter->second.end(), idx) == iter->second.end())
			iter->second.push_back(idx);
	}
	else
	{
		vector<size_t> temp(1, idx);
		uMAP.insert({ str, temp });
	}
}
void listElements::tableInsert(unordered_map<string, vector<size_t>>& uMAP, const string& str, size_t idx)
{
	vector<string> words(1, str);
	string diver = " /-";
	for (size_t i = 0, j, pivot = 0; i <= str.size(); i++)
	{
		if (i == str.size())
		{
			words.push_back(str.substr(pivot, i - pivot));
			break;
		}
		for (j = 0; j < diver.size(); j++)
			if (str[i] == diver[j])
				break;
		if (j - diver.size())
		{
			words.push_back(str.substr(pivot, i - pivot));
			pivot = i + 1;
			i++;
		}
	}
	std::sort(words.begin(), words.end());
	words.erase(unique(words.begin(), words.end()), words.end());
	for (string s : words)
		for (size_t i = 1, j; i <= s.size(); i++)
		{
			for (j = 0; j < diver.size(); j++)
				if (s[i - 1] == diver[j])
					break;
			if (j == diver.size())
				tableInsertString(uMAP, s.substr(0, i), idx);
		}
}
int listElements::maxY()
{
	return (this->raw.size() - 2) * this->margin  - this->margin/2 + 7;
}
size_t listElements::size()
{
	return this->raw.size();
}
void listElements::sort()
{
	this->sorted.clear();
	for (size_t idx = 0; idx < this->raw.size(); idx++)
		this->sorted.push_back({ this->raw[idx],idx });
	if (selector)
		std::sort(this->sorted.begin(), this->sorted.end(), [](const pair<musicInfo,size_t>& p, const pair<musicInfo, size_t>& q) {
		return p.first.album < q.first.album || (p.first.album == q.first.album && p.first.name < q.first.name);
	});
	else
		std::sort(this->sorted.begin(), this->sorted.end(), [](const pair<musicInfo, size_t>& p, const pair<musicInfo, size_t>& q) {
		return p.first.name < q.first.name || (p.first.name == q.first.name && p.first.album < q.first.album);
	});
}
size_t listElements::insert(const musicInfo& info)
{
	musicInfo temp(info);
	this->raw.push_back(temp);
	this->table.insert({ temp, this->raw.size() - 1 });
	this->tableInsert(this->tableName, temp.name, this->raw.size() - 1);
	this->tableInsert(this->tableAlbum, temp.album, this->raw.size() - 1);
	this->tableInsert(this->tableArtist, temp.artist, this->raw.size() - 1);
	return this->raw.size() - 1;
}
musicInfo listElements::find(size_t idx)
{
	if (idx < this->sorted.size())
		return this->sorted.at(idx).first;
	return nInfo;
}
vector<musicInfo> listElements::findName(string name)
{
	return this->find(tableName, name);
}
vector<musicInfo> listElements::findAlbum(string album)
{
	return this->find(tableAlbum, album);
}
vector<musicInfo> listElements::findArtist(string artist)
{
	return this->find(tableArtist, artist);
}
void listElements::remove(const musicInfo& info)
{
	if (info.name == "" && info.album == "" && info.artist == "" && info.genre == "")
		return;
	size_t idx = this->findIndex(info);
	if (idx > this->raw.size())
		return;
	this->table.erase(info);
	this->tableErase(this->tableName, idx);
	this->tableErase(this->tableAlbum, idx);
	this->tableErase(this->tableArtist, idx);
	this->raw.erase(this->raw.begin() + idx);
	for (vector<pair<musicInfo, size_t>>::iterator it = this->sorted.begin(); it != this->sorted.end(); it++)
		if (it->first == info)
		{
			it = this->sorted.erase(it);
			break;
		}
}
void listElements::clear()
{
	this->table.clear();
	this->tableName.clear();
	this->tableAlbum.clear();
	this->tableArtist.clear();
	this->raw.clear();
	this->sorted.clear();
}