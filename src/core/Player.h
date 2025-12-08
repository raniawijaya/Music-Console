#ifndef PLAYER_H
#define PLAYER_H
#include <memory>
#include <stack>
#include <queue>
#include "../core/Song.h"
using namespace std;

class Player {
public:
    shared_ptr<Song> current;
    stack<shared_ptr<Song>> history;
    queue<shared_ptr<Song>> upNext;

    void play(shared_ptr<Song> s) {
        if (!s) { cout << "[player] nothing to play\n"; return; }
        if (current) history.push(current);
        current = s;
        cout << "[PLAY] " << s->brief() << "\n";
    }
    void stop() {
        if (!current) { cout << "[player] no song playing\n"; return; }
        cout << "[STOP] " << current->brief() << "\n";
        current = nullptr;
    }
    void enqueue(shared_ptr<Song> s) {
        upNext.push(s);
        cout << "[ENQUEUE] " << s->brief() << "\n";
    }
    bool playNextFromQueue() {
        if (upNext.empty()) return false;
        auto s = upNext.front(); upNext.pop();
        play(s); return true;
    }
    bool playPrevFromHistory() {
        if (history.empty()) return false;
        auto s = history.top(); history.pop();
        play(s); return true;
    }
};
#endif
