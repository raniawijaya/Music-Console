#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H
#include <vector>
#include <memory>
#include "Playlist.h"
using namespace std;

/*
 PlaylistManager manages multiple playlists (per system).
 It enables propagating deletes across all playlists.
*/
class PlaylistManager {
private:
    vector<unique_ptr<Playlist>> lists;
public:
    Playlist* create(const string& name) {
        lists.push_back(make_unique<Playlist>(name));
        return lists.back().get();
    }
    vector<Playlist*> all() {
        vector<Playlist*> out;
        for (auto &u: lists) out.push_back(u.get());
        return out;
    }
    int removeSongFromAll(int id) {
        int total=0;
        for (auto &p: lists) total += p->removeAllReferences(id);
        return total;
    }
};
#endif
