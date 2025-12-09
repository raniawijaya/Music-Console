#ifndef AUTH_H
#define AUTH_H

#include <vector>
#include <string>
#include "User.h"

class Auth {
private:
    vector<User>* users;

public:
    Auth(vector<User>* u) : users(u) {}

    User* login(const string& username, const string& password) {
        for (auto &u : *users) {
            if (u.username == username && u.password == password) {
                return &u;
            }
        }
        return nullptr;
    }

    bool registerUser(const string& username,
                      const string& email,
                      const string& password,
                      const string& role)
    {
        for (auto &u : *users) {
            if (u.username == username)
                return false;
        }

        users->emplace_back(username, email, password, role);
        return true;
    }
};

#endif
