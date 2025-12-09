#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <string>
#include <memory>
#include <iostream>
#include "../core/Song.h"
using namespace std;

class Playlist {
public:
    struct Node {
        shared_ptr<Song> song;
        Node* prev;
        Node* next;

        Node(shared_ptr<Song> s)
            : song(s), prev(nullptr), next(nullptr) {}
    };

private:
    string name;
    Node* head;
    Node* tail;

public:
    Playlist(const string& nm = "Unnamed")
        : name(nm), head(nullptr), tail(nullptr) {}

    ~Playlist() { clear(); }

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

    bool removeSong(int id) {
        Node* cur = head;
        while (cur) {
            if (cur->song && cur->song->id == id) {
                if (cur->prev) cur->prev->next = cur->next;
                else head = cur->next;

                if (cur->next) cur->next->prev = cur->prev;
                else tail = cur->prev;

                delete cur;
                return true;
            }
            cur = cur->next;
        }
        return false;
    }

    vector<shared_ptr<Song>> toVector() const {
        vector<shared_ptr<Song>> v;
        Node* cur = head;
        while (cur) {
            v.push_back(cur->song);
            cur = cur->next;
        }
        return v;
    }

    Node* first() const { return head; }

    void show() const {
        cout << "Playlist: " << name << "\n";
        Node* cur = head;
        while (cur) {
            cout << "  - " << cur->song->brief() << "\n";
            cur = cur->next;
        }
    }

    void clear() {
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        head = tail = nullptr;
    }
};

#endif
