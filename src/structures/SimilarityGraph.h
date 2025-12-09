#ifndef SIMILARITY_GRAPH_H
#define SIMILARITY_GRAPH_H

#include <unordered_map>
#include <vector>
#include <memory>
#include <cmath>
#include "../core/Song.h"

using namespace std;

class SimilarityGraph {
public:
    // adjacency list
    unordered_map<int, vector<pair<int,double>>> g;

    // recreate graph from library list
    void build(const vector<shared_ptr<Song>>& songs) {
        g.clear();

        for (auto &a : songs) {
            for (auto &b : songs) {
                if (a->id == b->id) continue;

                double w = similarity(a, b);
                if (w > 0)
                    g[a->id].push_back({ b->id, w });
            }
        }
    }

    // simple similarity
    double similarity(shared_ptr<Song> a, shared_ptr<Song> b) {
        double score = 0.0;

        if (a->genre == b->genre) score += 2.0;
        if (a->artist == b->artist) score += 1.0;

        int diff = abs(a->year - b->year);
        score += max(0.0, 1.0 - diff / 10.0);

        return score;
    }

    // recommended sorted by weight desc
    vector<int> recommend(int id) {
        vector<int> res;
        if (!g.count(id)) return res;

        auto vec = g[id];
        sort(vec.begin(), vec.end(),
             [](auto &x, auto &y) { return x.second > y.second; });

        for (auto &x : vec) res.push_back(x.first);
        return res;
    }
};

#endif
