#ifndef PLAYLIST_MANAGER_H
#define PLAYLIST_MANAGER_H

#include <map>
#include <string>
#include <vector>
#include <memory>
#include "Playlist.h"

using namespace std;

/*
 Playlist Manager:
 maps[username][playlistName] = Playlist*
*/
class PlaylistManager {
public:
    map<string, map<string, Playlist*>> maps;

    ~PlaylistManager() {
        for (auto &u : maps)
            for (auto &p : u.second)
                delete p.second;
    }

    // Create playlist
    Playlist* create(const string &username, const string &name) {
        if (maps[username].count(name) == 0) {
            maps[username][name] = new Playlist(name);
        }
        return maps[username][name];
    }

    // Get playlist
    Playlist* get(const string &username, const string &name) {
        if (maps.count(username) == 0) return nullptr;
        if (maps.at(username).count(name) == 0) return nullptr;
        return maps.at(username).at(name);
    }

    // List playlist names
    vector<string> list(const string &username) {
        vector<string> out;
        if (maps.count(username) == 0) return out;

        for (auto &p : maps[username]) {
            out.push_back(p.first);
        }
        return out;
    }

    vector<string> getPlaylistNames(const string &username) {
        return list(username);
    }

    // Remove song from ALL playlists for ALL users
    void removeSongFromAll(int id) {
        for (auto &u : maps) {
            for (auto &pl : u.second) {
                pl.second->removeSong(id);
            }
        }
    }
};

#endif
