#ifndef PLAYLIST_H
#define PLAYLIST_H
#include <memory>
#include <string>
#include <iostream>
#include "../core/Song.h"
using namespace std;

/*
 Doubly linked list playlist. Each node stores shared_ptr<Song>.
*/

class Playlist {
public:
    struct Node {
        shared_ptr<Song> song;
        Node* prev;
        Node* next;
        Node(shared_ptr<Song> s): song(s), prev(nullptr), next(nullptr) {}
    };
private:
    Node* head;
    Node* tail;
    string name;
public:
    Playlist(const string& nm="unnamed"): head(nullptr), tail(nullptr), name(nm) {}
    ~Playlist() { cls(); }

    string getName() const { return name; }

    void addSong(shared_ptr<Song> s) {
        Node* node = new Node(s);
        if (!head) head = tail = node;
        else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
    }
    bool removeSongById(int id) {
        Node* cur = head;
        while(cur) {
            if (cur->song && cur->song->id == id) {
                if (cur->prev) cur->prev->next = cur->next; else head = cur->next;
                if (cur->next) cur->next->prev = cur->prev; else tail = cur->prev;
                delete cur;
                return true;
            }
            cur = cur->next;
        }
        return false;
    }
    int removeAllReferences(int id) {
        int cnt=0;
        while(removeSongById(id)) cnt++;
        return cnt;
    }
    void show() const {
        cout << "Playlist: " << name << "\n";
        Node* cur = head;
        while(cur) {
            if (cur->song) cout << "  - " << cur->song->brief() << "\n";
            cur = cur->next;
        }
    }
    Node* first() const { return head; }
    void cls() {
        Node* cur = head;
        while(cur) {
            Node* nx = cur->next;
            delete cur;
            cur = nx;
        }
        head = tail = nullptr;
    }
};
#endif
