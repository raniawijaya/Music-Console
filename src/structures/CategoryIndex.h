#ifndef CATEGORYINDEX_H
#define CATEGORYINDEX_H
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm> 
#include "../core/Song.h"
using namespace std;

/*
 CategoryIndex: simple hash map from genre/artist to vector<shared_ptr<Song>>
*/
class CategoryIndex {
private:
    unordered_map<string, vector<shared_ptr<Song>>> byGenre;
    unordered_map<string, vector<shared_ptr<Song>>> byArtist;
public:
    void addSong(shared_ptr<Song> s) {
        if (!s) return;
        byGenre[s->genre].push_back(s);
        byArtist[s->artist].push_back(s);
    }
    void removeSong(int id) {
        for (auto &kv: byGenre) {
            auto &v = kv.second;
            v.erase(remove_if(v.begin(), v.end(), [&](const shared_ptr<Song>&p){ return p->id==id; }), v.end());
        }
        for (auto &kv: byArtist) {
            auto &v = kv.second;
            v.erase(remove_if(v.begin(), v.end(), [&](const shared_ptr<Song>&p){ return p->id==id; }), v.end());
        }
    }
    vector<shared_ptr<Song>> getByGenre(const string& g) {
        if (byGenre.find(g)==byGenre.end()) return {};
        return byGenre[g];
    }
    vector<shared_ptr<Song>> getByArtist(const string& a) {
        if (byArtist.find(a)==byArtist.end()) return {};
        return byArtist[a];
    }
    void cls(){ byGenre.clear(); byArtist.clear(); }
};
#endif
