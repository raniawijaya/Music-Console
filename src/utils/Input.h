#ifndef INPUT_HELPER_H
#define INPUT_HELPER_H

#include <iostream>
#include <string>
using namespace std;

class Input {
public:
    static string getLine() {
        string x;
        getline(cin, x);
        if (x.size() == 0) getline(cin, x);
        return x;
    }
};

#endif
