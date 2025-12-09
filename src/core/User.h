#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

struct User {
    string username;
    string email;
    string password;
    string role;

    User() {}

    // constructor sesuai format CSV:
    // username,email,password,role
    User(string u, string e, string p, string r)
        : username(u), email(e), password(p), role(r) {}

    bool isAdmin() const {
        return role == "admin";
    }
};

#endif
