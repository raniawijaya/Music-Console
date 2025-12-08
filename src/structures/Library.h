#ifndef LIBRARY_H
#define LIBRARY_H
#include <map>
#include <memory>
#include <vector>
#include "../core/Song.h"
using namespace std;

/*
 Library uses std::map<int, shared_ptr<Song>> -> RB-tree behavior
 Key = song id
*/
class Library {
private:
    map<int, shared_ptr<Song>> tree;
public:
    Library() {}
    bool insert(shared_ptr<Song> s) {
        if (!s) return false;
        auto res = tree.emplace(s->id, s);
        return res.second;
    }
    shared_ptr<Song> find(int id) const {
        auto it = tree.find(id);
        if (it==tree.end()) return nullptr;
        return it->second;
    }
    bool remove(int id) {
        auto it = tree.find(id);
        if (it==tree.end()) return false;
        tree.erase(it);
        return true;
    }
    bool update(int id, const Song& newdata) {
        auto it = tree.find(id);
        if (it==tree.end()) return false;
        auto s = it->second;
        s->title = newdata.title;
        s->artist = newdata.artist;
        s->album = newdata.album;
        s->genre = newdata.genre;
        s->year = newdata.year;
        s->duration = newdata.duration;
        return true;
    }
    vector<shared_ptr<Song>> allSorted() const {
        vector<shared_ptr<Song>> out;
        out.reserve(tree.size());
        for (auto &p: tree) out.push_back(p.second);
        return out;
    }
    void cls() { tree.clear(); }
};
#endif
