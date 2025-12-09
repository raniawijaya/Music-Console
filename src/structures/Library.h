#ifndef LIBRARY_H
#define LIBRARY_H

#include <map>
#include <memory>
#include <vector>
#include <algorithm>
#include "../core/Song.h"
using namespace std;

/*
 Library uses std::map<int, shared_ptr<Song>>
 Key = song id (unique)
*/
class Library {
private:
    map<int, shared_ptr<Song>> tree;

public:
    Library() {}

    // Insert
    bool insert(shared_ptr<Song> s) {
        if (!s) return false;
        return tree.emplace(s->id, s).second;
    }

    // Find by ID
    shared_ptr<Song> find(int id) const {
        auto it = tree.find(id);
        if (it == tree.end()) return nullptr;
        return it->second;
    }

    // Count
    int count() const {
        return tree.size();
    }

    // Next ID
    int nextID() const {
        if (tree.empty()) return 1;
        return tree.rbegin()->first + 1;
    }

    // Remove by ID
    bool remove(int id) {
        return tree.erase(id) > 0;
    }

    // Return all songs
    vector<shared_ptr<Song>> all() const {
        vector<shared_ptr<Song>> out;
        for (auto &p : tree) out.push_back(p.second);
        return out;
    }

    // Search by Title (case-insensitive)
    vector<shared_ptr<Song>> searchTitle(const string &key) const {
        vector<shared_ptr<Song>> result;
        string lowerKey = toLower(key);

        for (auto &p : tree) {
            string lowerTitle = toLower(p.second->title);
            if (lowerTitle.find(lowerKey) != string::npos)
                result.push_back(p.second);
        }
        return result;
    }

    // Search by Genre (case-insensitive)
    vector<shared_ptr<Song>> searchGenre(const string &key) const {
        vector<shared_ptr<Song>> result;
        string lowerKey = toLower(key);

        for (auto &p : tree) {
            string lowerGenre = toLower(p.second->genre);
            if (lowerGenre.find(lowerKey) != string::npos)
                result.push_back(p.second);
        }
        return result;
    }

private:
    // Helper lowercase
    static string toLower(string s) {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }
};

#endif
