#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <queue>
#include <stack>
#include <vector>
#include <iostream>
#include "../core/Song.h"

using namespace std;

class Player {
private:
    shared_ptr<Song> current = nullptr;

public:
    // history (LIFO) dan queue (FIFO) of shared_ptr<Song>
    stack<shared_ptr<Song>> history;
    queue<shared_ptr<Song>> nextQueue;

    // optional pointer to library for fallback (set by Menu)
    vector<shared_ptr<Song>>* library = nullptr;

    Player() = default;

    // link library pointer for fallback recommendations
    void linkLibrary(vector<shared_ptr<Song>>* lib) { library = lib; }

    // start playing a song immediately
    void play(const shared_ptr<Song>& s) {
        if (current) history.push(current);
        current = s;
        showNowPlaying();
    }

    // show now playing (console output)
    void showNowPlaying() {
        if (!current) {
            cout << "No song is playing.\n";
            return;
        }
        cout << "Now Playing: " << current->brief() << "\n";
    }

    // stop playback
    void stop() {
        current = nullptr;
        while (!nextQueue.empty()) nextQueue.pop();
        while (!history.empty()) history.pop();
        cout << "Playback stopped.\n";
    }

    // push a song into queue
    void enqueue(const shared_ptr<Song>& s) {
        nextQueue.push(s);
    }

    // play next: first from queue, else fallback behaviour
    // fallback logic can be implemented from Menu using similarity graph;
    // here we only consume queue or pop from library linearly if provided.
    void next() {
        if (!nextQueue.empty()) {
            if (current) history.push(current);
            current = nextQueue.front();
            nextQueue.pop();
            showNowPlaying();
            return;
        }

        // simplistic fallback: if library set, find next by index
        if (library && current) {
            // find current index
            int idx = -1;
            for (size_t i = 0; i < library->size(); ++i) {
                if ((*library)[i]->id == current->id) { idx = (int)i; break; }
            }
            if (idx >= 0 && idx + 1 < (int)library->size()) {
                history.push(current);
                current = (*library)[idx + 1];
                showNowPlaying();
                return;
            }
        }

        // if nothing else, stop
        cout << "No next song available.\n";
    }

    // go to previous song from history
    void prev() {
        if (history.empty()) {
            cout << "No previous song.\n";
            return;
        }
        current = history.top();
        history.pop();
        showNowPlaying();
    }

    // get current pointer
    shared_ptr<Song> now() const { return current; }
};

#endif // PLAYER_H