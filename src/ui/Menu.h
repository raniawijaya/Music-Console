#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>

#include "../core/User.h"
#include "../core/Auth.h"
#include "../core/Player.h"
#include "../utils/FileIO.h"
#include "../utils/Input.h"
#include "../structures/Library.h"
#include "../structures/PlaylistManager.h"

using namespace std;

class Menu {
private:
    vector<User> users;
    Auth auth;
    Library library;
    PlaylistManager playlistMgr;
    Player player;

public:
    Menu() : auth(&users) {

        cout << "Loading users...\n";
        users = FileIO::loadUsers("data/users.csv");

        cout << "Loading songs...\n";
        FileIO::loadSongs("data/songs.csv", library);

        cout << "Loading playlists...\n";
        FileIO::loadPlaylists(playlistMgr, library, users);
    }

    // ---------------------------------------------------------
    void clear() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    }

    void pause() {
        cout << "\nTekan ENTER untuk melanjutkan...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    // =========================================================
    // MAIN MENU
    // =========================================================
    void run() {
        while (true) {
            clear();
            cout << "=========== MUSIC CONSOLE ==========\n";
            cout << "1. Login\n";
            cout << "2. Register\n";
            cout << "3. Exit\n";
            cout << "Pilih: ";

            int pilih;
            cin >> pilih;
            cin.ignore();

            if (pilih == 1) handleLogin();
            else if (pilih == 2) handleRegister();
            else if (pilih == 3) {
                FileIO::saveSongs("data/songs.csv", library);
                FileIO::saveUsers("data/users.csv", users);
                FileIO::savePlaylists(playlistMgr, users);
                cout << "Bye!\n";
                return;
            }
        }
    }

    // =========================================================
    // LOGIN
    // =========================================================
    void handleLogin() {
        cout << "Username: ";
        string username = Input::getLine();
        cout << "Password: ";
        string password = Input::getLine();

        User* u = auth.login(username, password);

        if (!u) {
            cout << "Login gagal!\n";
            pause();
            return;
        }

        if (u->isAdmin())
            adminDashboard(*u);
        else
            userDashboard(*u);

        // save setelah logout
        FileIO::saveUsers("data/users.csv", users);
        FileIO::savePlaylists(playlistMgr, users);
    }

    // =========================================================
    // REGISTER
    // =========================================================
    void handleRegister() {
        cout << "Username: ";
        string username = Input::getLine();
        cout << "Email: ";
        string email = Input::getLine();
        cout << "Password: ";
        string password = Input::getLine();

        bool ok = auth.registerUser(username, email, password, "user");

        if (!ok) {
            cout << "Username sudah digunakan.\n";
        } else {
            cout << "Akun berhasil dibuat!\n";
            FileIO::saveUsers("data/users.csv", users);
        }

        pause();
    }

    // =========================================================
    // ADMIN DASHBOARD
    // =========================================================
    void adminDashboard(User &u) {
        while (true) {
            clear();
            cout << "========= ADMIN DASHBOARD =========\n";
            cout << "User: " << u.username << "\n";
            cout << "Total Lagu: " << library.all().size() << "\n\n";

            cout << "1. Lihat Library\n";
            cout << "2. Cari Lagu\n";
            cout << "3. Tambah Lagu\n";
            cout << "4. Hapus Lagu\n";
            cout << "5. Logout\n";
            cout << "Pilih: ";

            int pilih;
            cin >> pilih;
            cin.ignore();

            if (pilih == 1) listLibrary();
            else if (pilih == 2) searchSongMenu();
            else if (pilih == 3) addSong();
            else if (pilih == 4) deleteSong();
            else if (pilih == 5) return;
        }
    }

    // =========================================================
    // USER DASHBOARD
    // =========================================================
    void userDashboard(User &u) {
        while (true) {
            clear();
            cout << "========= USER DASHBOARD =========\n";
            cout << "User: " << u.username << "\n";

            cout << "1. Lihat Library\n";
            cout << "2. Cari Lagu\n";
            cout << "3. Play Lagu\n";
            cout << "4. Playlist Menu\n";
            cout << "5. Logout\n";

            int pilih;
            cin >> pilih;
            cin.ignore();

            if (pilih == 1) listLibrary();
            else if (pilih == 2) searchSongMenu();
            else if (pilih == 3) playByTitle();
            else if (pilih == 4) playlistMenu(u);
            else if (pilih == 5) return;
        }
    }

    // =========================================================
    // LIBRARY / SEARCH
    // =========================================================
    void listLibrary() {
        clear();
        cout << "=========== LIBRARY ===========\n";

        for (auto &s : library.all()) {
            cout << left
                 << setw(3)  << s->id << " | "
                 << setw(25) << s->title << " | "
                 << setw(15) << s->artist << " | "
                 << setw(10) << s->genre << "\n";
        }
        pause();
    }

    void searchSongMenu() {
        clear();
        cout << "Cari berdasarkan:\n";
        cout << "1. Judul\n";
        cout << "2. Genre\n";
        cout << "Pilih: ";

        int p;
        cin >> p;
        cin.ignore();

        cout << "Masukkan kata kunci: ";
        string key = Input::getLine();

        vector<shared_ptr<Song>> result =
            (p == 1 ? library.searchTitle(key)
                    : library.searchGenre(key));

        clear();
        cout << "Hasil pencarian:\n\n";
        for (auto &s : result)
            cout << " - " << s->brief() << "\n";

        pause();
    }

    // =========================================================
    // ADMIN CRUD SONG
    // =========================================================
    void addSong() {
        clear();
        cin.ignore();

        string title, artist, album, genre;
        int year;
        double dur;

        cout << "Judul: ";  title = Input::getLine();
        cout << "Artist: "; artist = Input::getLine();
        cout << "Album: ";  album = Input::getLine();
        cout << "Genre: ";  genre = Input::getLine();
        cout << "Tahun: ";  cin >> year;
        cout << "Durasi: "; cin >> dur;
        cin.ignore();

        int newID = library.nextID();
        auto s = make_shared<Song>(newID, title, artist, album, genre, year, dur);
        library.insert(s);

        cout << "Lagu berhasil ditambahkan!\n";
        pause();
    }

    void deleteSong() {
        clear();
        cin.ignore();

        cout << "Masukkan judul lagu yg ingin dihapus: ";
        string key = Input::getLine();

        auto list = library.searchTitle(key);
        if (list.empty()) {
            cout << "Tidak ditemukan.\n";
            pause();
            return;
        }

        cout << "Menghapus: " << list[0]->brief() << "\n";
        library.remove(list[0]->id);

        playlistMgr.removeSongFromAll(list[0]->id);

        cout << "Berhasil dihapus.\n";
        pause();
    }

    // =========================================================
    // PLAY SONG
    // =========================================================
    void playByTitle() {
        clear();
        cin.ignore();

        cout << "Judul lagu: ";
        string key = Input::getLine();

        auto list = library.searchTitle(key);
        if (list.empty()) {
            cout << "Tidak ditemukan!\n";
            pause();
            return;
        }

        player.play(list[0]);
    }

    // =========================================================
    // PLAYLIST OPS
    // =========================================================
    void playlistMenu(User &u) {
        while (true) {
            clear();
            cout << "========= PLAYLIST MENU =========\n";
            cout << "1. Lihat Playlist\n";
            cout << "2. Buat Playlist Baru\n";
            cout << "3. Tambah Lagu ke Playlist\n";
            cout << "4. Hapus Lagu dari Playlist\n";
            cout << "5. Kembali\n";

            int p;
            cin >> p;
            cin.ignore();

            if (p == 1) showUserPlaylists(u);
            else if (p == 2) createPlaylist(u);
            else if (p == 3) addSongToPlaylist(u);
            else if (p == 4) removeSongFromPlaylist(u);
            else if (p == 5) return;
        }
    }

    // show playlists
    void showUserPlaylists(User &u) {
        clear();
        auto list = playlistMgr.getPlaylistNames(u.username);

        cout << "Daftar playlist milik " << u.username << ":\n";
        for (auto &p : list)
            cout << "- " << p << "\n";

        pause();
    }

    void createPlaylist(User &u) {
        clear();
        cin.ignore();

        cout << "Nama playlist: ";
        string name = Input::getLine();

        playlistMgr.create(u.username, name);
        cout << "Playlist dibuat!\n";
        pause();
    }

    void addSongToPlaylist(User &u) {
        clear();
        cin.ignore();

        cout << "Nama playlist: ";
        string name = Input::getLine();

        auto p = playlistMgr.get(u.username, name);
        if (!p) {
            cout << "Playlist tidak ditemukan!\n";
            pause();
            return;
        }

        cout << "Judul lagu: ";
        string key = Input::getLine();

        auto list = library.searchTitle(key);
        if (list.empty()) {
            cout << "Tidak ditemukan.\n";
            pause();
            return;
        }

        p->addSong(list[0]);
        cout << "Lagu ditambahkan.\n";
        pause();
    }

    void removeSongFromPlaylist(User &u) {
        clear();
        cin.ignore();

        cout << "Nama playlist: ";
        string name = Input::getLine();

        auto p = playlistMgr.get(u.username, name);
        if (!p) {
            cout << "Playlist tidak ditemukan!\n";
            pause();
            return;
        }

        cout << "Judul lagu: ";
        string key = Input::getLine();

        auto list = library.searchTitle(key);
        if (list.empty()) {
            cout << "Tidak ditemukan.\n";
            pause();
            return;
        }

        p->removeSong(list[0]->id);
        cout << "Lagu dihapus.\n";
        pause();
    }
};

#endif
