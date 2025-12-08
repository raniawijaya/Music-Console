#ifndef AUTH_H
#define AUTH_H
#include <vector>
#include <string>
#include "../core/User.h"
using namespace std;

class Auth {
private:
    vector<User> users;
public:
    Auth() {}
    void loadUsers(const vector<User>& from) { users = from; }
    vector<User> dumpUsers() const { return users; }

    bool registerUser(const User& u) {
        for (auto &x: users) if (x.username==u.username) return false;
        users.push_back(u);
        return true;
    }
    User* login(const string& uname, const string& pass) {
        for (auto &x: users) if (x.username==uname && x.password==pass) return &x;
        return nullptr;
    }
    // helper to ensure admin exists
    void ensureAdmin() {
        for (auto &x: users) if (x.isAdmin) return;
        users.emplace_back("admin","admin@mail","admin",true);
    }
};
#endif
