#ifndef FILEIO_H
#define FILEIO_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>

#include "../core/Song.h"
#include "../core/User.h"
#include "../structures/Library.h"
#include "../structures/PlaylistManager.h"

using namespace std;

class FileIO {
public:

    // ============================================================
    // LOAD SONGS
    // ============================================================
    static void loadSongs(const string &path, Library &library) {
        ifstream file(path);
        if (!file.is_open()) {
            cout << "[WARNING] songs.csv tidak ditemukan. Membuat baru.\n";
            return;
        }

        string line;
        getline(file, line); // skip header

        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string id, title, artist, album, genre, year, duration;

            getline(ss, id, ',');
            getline(ss, title, ',');
            getline(ss, artist, ',');
            getline(ss, album, ',');
            getline(ss, genre, ',');
            getline(ss, year, ',');
            getline(ss, duration, ',');

            try {
                auto s = make_shared<Song>(
                    stoi(id),
                    title,
                    artist,
                    album,
                    genre,
                    stoi(year),
                    stod(duration)
                );
                library.insert(s);

            } catch (...) {
                cout << "[ERROR] songs.csv baris rusak: " << line << endl;
            }
        }

        file.close();
    }

    // ============================================================
    // SAVE SONGS
    // ============================================================
    static void saveSongs(const string &path, const Library &library) {
        ofstream file(path);
        file << "id,title,artist,album,genre,year,duration\n";

        for (auto &s : library.all()) {
            file << s->id << ","
                 << s->title << ","
                 << s->artist << ","
                 << s->album << ","
                 << s->genre << ","
                 << s->year << ","
                 << s->duration << "\n";
        }

        file.close();
    }

    // ============================================================
    // LOAD USERS (FINAL FIX)
    // FORMAT CSV: username,email,password,role
    // ============================================================
    static vector<User> loadUsers(const string &path) {
        vector<User> out;

        ifstream file(path);
        if (!file.is_open()) {
            cout << "[WARNING] users.csv tidak ditemukan. Membuat baru.\n";
            return out;
        }

        string line;
        getline(file, line); // skip header

        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string username, email, password, role;

            getline(ss, username, ',');
            getline(ss, email, ',');
            getline(ss, password, ',');
            getline(ss, role, ',');

            out.emplace_back(username, email, password, role);
        }

        file.close();
        return out;
    }

    // ============================================================
    // SAVE USERS (FINAL FIX)
    // ============================================================
    static void saveUsers(const string &path, const vector<User> &users) {
        ofstream file(path);
        file << "username,email,password,role\n";

        for (auto &u : users) {
            file << u.username << ","
                 << u.email << ","
                 << u.password << ","
                 << u.role << "\n";
        }

        file.close();
    }

    // ============================================================
    // LOAD PLAYLISTS (per-user CSV)
    // ============================================================
    static void loadPlaylists(PlaylistManager &mgr,
                              const Library &library,
                              const vector<User> &users)
    {
        for (auto &u : users) {

            string filename = "data/playlist_" + u.username + ".csv";
            ifstream file(filename);

            if (!file.is_open()) continue; // belum punya playlist

            string line;
            getline(file, line); // header

            while (getline(file, line)) {
                if (line.empty()) continue;

                stringstream ss(line);
                string playlistName, idStr;

                getline(ss, playlistName, ',');
                getline(ss, idStr, ',');

                int id = stoi(idStr);
                auto s = library.find(id);
                if (!s) continue;

                Playlist* p = mgr.create(u.username, playlistName);
                p->addSong(s);
            }

            file.close();
        }
    }

    // ============================================================
    // SAVE PLAYLISTS (per-user CSV)
    // ============================================================
    static void savePlaylists(const PlaylistManager &mgr,
                              const vector<User> &users)
    {
        for (auto &u : users) {

            string filename = "data/playlist_" + u.username + ".csv";
            ofstream file(filename);

            file << "playlistName,songID\n";

            // jika user punya playlist
            if (mgr.maps.count(u.username)) {
                for (auto &p : mgr.maps.at(u.username)) {
                    Playlist* pl = p.second;

                    for (auto &s : pl->toVector()) {
                        file << pl->getName() << ","
                             << s->id << "\n";
                    }
                }
            }

            file.close();
        }
    }
};

#endif
