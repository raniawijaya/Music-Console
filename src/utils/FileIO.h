#ifndef FILEIO_H
#define FILEIO_H
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "../core/Song.h"
#include "../core/User.h"
using namespace std;

/*
 Simple CSV loader/saver for songs & users.
 Format songs.csv: id,title,artist,album,genre,year,duration
 Format users.csv: username,email,password,isAdmin(0/1)
*/

namespace FileIO {
    vector<shared_ptr<Song>> loadSongs(const string& path) {
        vector<shared_ptr<Song>> out;
        ifstream ifs(path);
        if (!ifs.is_open()) return out;
        string line;
        while (getline(ifs, line)) {
            if (line.empty()) continue;
            // naive CSV split (no complex escaping)
            stringstream ss(line);
            string tok;
            vector<string> f;
            while (getline(ss, tok, ',')) f.push_back(tok);
            if (f.size() < 7) continue;
            try {
                int id = stoi(f[0]);
                string title = f[1];
                string artist = f[2];
                string album = f[3];
                string genre = f[4];
                int year = stoi(f[5]);
                double dur = stod(f[6]);
                out.push_back(make_shared<Song>(id,title,artist,album,genre,year,dur));
            } catch (...) { continue; }
        }
        return out;
    }

    vector<User> loadUsers(const string& path) {
        vector<User> out;
        ifstream ifs(path);
        if (!ifs.is_open()) return out;
        string line;
        while (getline(ifs, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string a,b,c,d;
            getline(ss,a,','); getline(ss,b,','); getline(ss,c,','); getline(ss,d,',');
            bool admin = (d=="1");
            out.emplace_back(a,b,c,admin);
        }
        return out;
    }

    bool saveSongs(const string& path, const vector<shared_ptr<Song>>& songs) {
        ofstream ofs(path);
        if (!ofs.is_open()) return false;
        for (auto &s: songs) ofs << s->toCSV() << "\n";
        return true;
    }

    bool saveUsers(const string& path, const vector<User>& users) {
        ofstream ofs(path);
        if (!ofs.is_open()) return false;
        for (auto &u: users) ofs << u.toCSV() << "\n";
        return true;
    }
}
#endif
