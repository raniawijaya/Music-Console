#ifndef SONG_H
#define SONG_H

#include <string>
#include <algorithm>

using namespace std;

// Helper: lowercase (ignore-case search)
inline string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

struct Song {
    int id = 0;
    string title;
    string artist;
    string album;
    string genre;
    int year = 0;
    double duration = 0.0;

    Song() = default;

    Song(int _id, const string& _title, const string& _artist,
         const string& _album, const string& _genre,
         int _year, double _duration)
        : id(_id), title(_title), artist(_artist),
          album(_album), genre(_genre), year(_year), duration(_duration)
    {}

    // short description for lists
    string brief() const {
        return to_string(id) + " | " + title + " - " + artist + " (" + genre + ")";
    }

    // full details (useful for view)
    string full() const {
        return to_string(id) + " | " + title + " - " + artist + " | " + album
               + " | " + genre + " | " + to_string(year) + " | " + to_string(duration);
    }

    // match helpers (ignore-case)
    bool matchTitle(const string& q) const {
        if (q.empty()) return true;
        return toLower(title).find(toLower(q)) != string::npos;
    }

    bool matchGenre(const string& q) const {
        if (q.empty()) return true;
        return toLower(genre).find(toLower(q)) != string::npos;
    }
};

#endif // SONG_H
