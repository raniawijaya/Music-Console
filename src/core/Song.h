#ifndef SONG_H
#define SONG_H
#include <string>
#include <sstream>
using namespace std;

struct Song {
    int id;
    string title;
    string artist;
    string album;
    string genre;
    int year;
    double duration;

    Song(): id(0), year(0), duration(0.0) {}
    Song(int _id, string _title, string _artist, string _album, string _genre, int _year, double _duration)
    : id(_id), title(move(_title)), artist(move(_artist)), album(move(_album)), genre(move(_genre)), year(_year), duration(_duration) {}

    string brief() const {
        stringstream ss;
        ss << id << " | " << title << " - " << artist << " (" << year << ")";
        return ss.str();
    }

    string toCSV() const {
        // id,title,artist,album,genre,year,duration
        stringstream ss;
        ss << id << "," << escapeCSV(title) << "," << escapeCSV(artist) << "," << escapeCSV(album) << "," << escapeCSV(genre) << "," << year << "," << duration;
        return ss.str();
    }
    static string escapeCSV(const string &s) {
        bool needq = s.find(',')!=string::npos || s.find('"')!=string::npos;
        if (!needq) return s;
        string r;
        r.push_back('"');
        for (char c: s) {
            if (c=='"') r += "\"\"";
            else r.push_back(c);
        }
        r.push_back('"');
        return r;
    }
};
#endif