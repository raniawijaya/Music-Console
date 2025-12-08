#ifndef USER_H
#define USER_H
#include <string>
using namespace std;

struct User {
    string username;
    string email;
    string password; // plain for demo - in prod use hashing
    bool isAdmin;

    User() : isAdmin(false) {}
    User(string u, string e, string p, bool admin=false) : username(u), email(e), password(p), isAdmin(admin) {}
    string toCSV() const {
        return username + "," + email + "," + password + "," + (isAdmin ? "1" : "0");
    }
};
#endif
