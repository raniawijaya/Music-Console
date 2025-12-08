#ifndef SIMILARITYGRAPH_H
#define SIMILARITYGRAPH_H
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "../core/Song.h"
#include "../structures/Library.h"
using namespace std;

/*
 Graph adjacency list: map song id -> vector of (neighbor id, weight)
 Weight is heuristic similarity.
*/
class Graph {
private:
    unordered_map<int, vector<pair<int,double>>> adj;
public:
    void cls() { adj.clear(); }
    void addEdge(int a, int b, double w) {
        if (a==b) return;
        adj[a].push_back({b,w});
        adj[b].push_back({a,w});
    }
    void removeNode(int id) {
        adj.erase(id);
        for (auto &kv: adj) {
            auto &vec = kv.second;
            vec.erase(remove_if(vec.begin(), vec.end(), [&](const pair<int,double>&p){ return p.first==id; }), vec.end());
        }
    }
    vector<int> topK(int id, int k=3) {
        vector<int> out;
        if (adj.find(id)==adj.end()) return out;
        auto vec = adj[id];
        sort(vec.begin(), vec.end(), [](auto &a, auto &b){ return a.second > b.second; });
        for (int i=0;i<(int)vec.size() && (int)out.size()<k;i++) out.push_back(vec[i].first);
        return out;
    }
    void buildFromLibrary(const Library& lib) {
        cls();
        auto all = lib.allSorted();
        int n = all.size();
        for (int i=0;i<n;i++) {
            for (int j=i+1;j<n;j++) {
                double score = 0;
                if (all[i]->artist == all[j]->artist) score += 5;
                if (all[i]->genre == all[j]->genre) score += 3;
                if (abs(all[i]->year - all[j]->year) <= 1) score += 1;
                if (score>0) addEdge(all[i]->id, all[j]->id, score);
            }
        }
    }
};
#endif
