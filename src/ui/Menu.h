#ifndef MENU_H
#define MENU_H
#include <iostream>
#include <limits>
#include <cstdlib>
#include "../structures/Library.h"
#include "../structures/PlaylistManager.h"
#include "../structures/CategoryIndex.h"
#include "../structures/SimilarityGraph.h"
#include "../core/Player.h"
#include "../core/Auth.h"
#include "../utils/FileIO.h"
using namespace std;

static void waitEnter() {
    cout << "Press ENTER to continue..."; cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

class Menu {
public:
    Library lib;
    PlaylistManager plm;
    CategoryIndex idx;
    Graph graph;
    Player player;
    Auth auth;

    string songsPath = "C:/music-player/data/songs.csv";
    string usersPath = "C:/music-player/data/users.csv";

    Menu() {
        // load data
        auto songs = FileIO::loadSongs(songsPath);
        for (auto &s: songs) {
            lib.insert(s);
            idx.addSong(s);
        }
        auto users = FileIO::loadUsers(usersPath);
        auth.loadUsers(users);
        auth.ensureAdmin();
        graph.buildFromLibrary(lib);
    }

    ~Menu() {
        // persist on exit
        FileIO::saveSongs("C:/music-player/data/song.csv", lib.allSorted());
        FileIO::saveUsers("C:/music-player/data/user.csv", auth.dumpUsers());
    }

    void mainLoop() {
        while(true) {
            system("cls"); // use "cls" on Windows if you want
            cout << "=== Music Console App ===\n";
            cout << "1. Login\n2. Register\n0. Exit\nChoose: ";
            int cmd; if (!(cin>>cmd)) { cin.clear(); cin.ignore(10000,'\n'); continue; }
            cin.ignore(10000,'\n');
            if (cmd==0) break;
            if (cmd==1) handleLogin();
            else if (cmd==2) handleRegister();
        }
    }

    void handleRegister() {
        string u,e,p;
        cout<<"username: "; getline(cin,u);
        cout<<"email: "; getline(cin,e);
        cout<<"password: "; getline(cin,p);
        if (auth.registerUser(User(u,e,p,false))) cout<<"Registered.\n"; else cout<<"Username exists.\n";
        waitEnter();
    }
    void handleLogin() {
        string u,p;
        cout<<"username: "; getline(cin,u);
        cout<<"password: "; getline(cin,p);
        User* usr = auth.login(u,p);
        if (!usr) { cout<<"Login failed.\n"; waitEnter(); return; }
        if (usr->isAdmin) adminLoop(*usr); else userLoop(*usr);
    }

    // ---- admin
    void adminLoop(User& admin) {
        while(true) {
            system("cls");
            cout << "[ADMIN] Hello " << admin.username << "\n";
            cout << "1. Add song\n2. List songs\n3. Edit song\n4. Delete song\n5. Rebuild graph\n0. Logout\nChoose: ";
            int c; if (!(cin>>c)) { cin.clear(); cin.ignore(10000,'\n'); continue; }
            cin.ignore(10000,'\n');
            if (c==0) break;
            if (c==1) adminAdd();
            else if (c==2) adminList();
            else if (c==3) adminEdit();
            else if (c==4) adminDelete();
            else if (c==5) { graph.buildFromLibrary(lib); cout<<"Graph rebuilt.\n"; waitEnter(); }
        }
    }
    void adminAdd() {
        int id,y; double d;
        string t,art,alb,genre;
        cout<<"id: "; cin>>id; cin.ignore(10000,'\n');
        cout<<"title: "; getline(cin,t);
        cout<<"artist: "; getline(cin,art);
        cout<<"album: "; getline(cin,alb);
        cout<<"genre: "; getline(cin,genre);
        cout<<"year: "; cin>>y; cin.ignore(10000,'\n');
        cout<<"duration(sec): "; cin>>d; cin.ignore(10000,'\n');
        auto s = make_shared<Song>(id,t,art,alb,genre,y,d);
        if (lib.insert(s)) { idx.addSong(s); graph.buildFromLibrary(lib); cout<<"Added.\n"; }
        else cout<<"ID exists.\n";
        waitEnter();
    }
    void adminList() {
        auto all = lib.allSorted();
        cout<<"Library:\n";
        for (auto &s: all) cout << "  " << s->brief() << "\n";
        waitEnter();
    }
    void adminEdit() {
        cout<<"id to edit: "; int id; cin>>id; cin.ignore(10000,'\n');
        auto s = lib.find(id);
        if (!s) { cout<<"not found\n"; waitEnter(); return; }
        string t,art,alb,genre; int y; double d;
        cout<<"title (enter keep): "; getline(cin,t);
        cout<<"artist (enter keep): "; getline(cin,art);
        cout<<"album (enter keep): "; getline(cin,alb);
        cout<<"genre (enter keep): "; getline(cin,genre);
        cout<<"year (0 keep): "; cin>>y; cin.ignore(10000,'\n');
        cout<<"duration (0 keep): "; cin>>d; cin.ignore(10000,'\n');
        Song nd = *s;
        if (!t.empty()) nd.title = t;
        if (!art.empty()) nd.artist = art;
        if (!alb.empty()) nd.album = alb;
        if (!genre.empty()) nd.genre = genre;
        if (y!=0) nd.year = y;
        if (d!=0) nd.duration = d;
        lib.update(id, nd);
        // rebuild indices (simple approach)
        idx.cls();
        for (auto &ss: lib.allSorted()) idx.addSong(ss);
        graph.buildFromLibrary(lib);
        cout<<"Updated.\n"; waitEnter();
    }
    void adminDelete() {
        cout<<"id to delete: "; int id; cin>>id; cin.ignore(10000,'\n');
        // remove from playlists
        int removed = plm.removeSongFromAll(id);
        // remove from lib
        if (lib.remove(id)) {
            idx.removeSong(id);
            graph.removeNode(id);
            cout<<"Deleted. Removed references from playlists: "<<removed<<"\n";
        } else cout<<"Not found.\n";
        waitEnter();
    }

    // ---- user
    void userLoop(User& usr) {
        while(true) {
            system("cls");
            cout << "[USER] Hello " << usr.username << "\n";
            cout << "1. Search song by id\n2. Search by genre\n3. Play song by id\n4. Create playlist & add\n5. Show playlists\n6. Enqueue song\n7. Play next / prev / similar\n0. Logout\nChoose: ";
            int c; if (!(cin>>c)) { system("cls"); cin.ignore(10000,'\n'); continue; }
            cin.ignore(10000,'\n');
            if (c==0) break;
            if (c==1) { cout<<"id: "; int id; cin>>id; cin.ignore(10000,'\n'); auto s = lib.find(id); if (s) cout<<s->brief()<<"\n"; else cout<<"not found\n"; waitEnter(); }
            else if (c==2) {
                cout<<"genre: "; string g; getline(cin,g);
                auto v = idx.getByGenre(g);
                if (v.empty()) cout<<"none\n"; for (auto &x: v) cout<<x->brief()<<"\n";
                waitEnter();
            } else if (c==3) {
                cout<<"id: "; int id; cin>>id; cin.ignore(10000,'\n'); auto s = lib.find(id); if (s) player.play(s); else cout<<"not found\n"; waitEnter();
            } else if (c==4) {
                cout<<"playlist name: "; string nm; getline(cin,nm);
                auto pl = plm.create(nm);
                cout<<"how many to add? "; int k; cin>>k; cin.ignore(10000,'\n');
                for (int i=0;i<k;i++) { cout<<"id: "; int id; cin>>id; cin.ignore(10000,'\n'); auto s = lib.find(id); if (s) pl->addSong(s); else cout<<"not found\n"; }
                cout<<"created\n"; waitEnter();
            } else if (c==5) {
                auto all = plm.all();
                for (auto &p: all) p->show();
                waitEnter();
            } else if (c==6) {
                cout<<"id to enqueue: "; int id; cin>>id; cin.ignore(10000,'\n'); auto s = lib.find(id); if (s) player.enqueue(s); else cout<<"not found\n"; waitEnter();
            } else if (c==7) {
                cout<<"1.next from queue 2.prev from history 3.next similar\nChoose: "; int op; cin>>op; cin.ignore(10000,'\n');
                if (op==1) { if (!player.playNextFromQueue()) cout<<"queue empty\n"; waitEnter(); }
                else if (op==2) { if (!player.playPrevFromHistory()) cout<<"history empty\n"; waitEnter(); }
                else if (op==3) {
                    if (!player.current) { cout<<"no current\n"; waitEnter(); continue; }
                    auto nb = graph.topK(player.current->id, 1);
                    if (nb.empty()) {
                        // fallback random
                        auto all = lib.allSorted();
                        if (!all.empty()) {
                            auto pick = all[rand()%all.size()];
                            player.play(pick);
                        } else cout<<"no songs\n";
                    } else {
                        auto pick = lib.find(nb[0]);
                        if (pick) player.play(pick); else cout<<"neighbor not present\n";
                    }
                    waitEnter();
                }
            }
        }
    }
};
#endif
