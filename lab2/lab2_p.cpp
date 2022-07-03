#include <iostream>
#include <fstream>
#include <omp.h>
#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <random>
using namespace std;

#define NODE_NUM 100000
#define P_NUM 2

int main() {
    int node[NODE_NUM] = { 0 };
    for (int i = 1;i < NODE_NUM;i++) {
        node[i] = INT32_MAX / 2;
    }
    vector<tuple<int, int, int> >edge;

    ifstream fmap;
    fmap.open("map.txt");
    while (!fmap.eof()) {
        int i, j, d;
        fmap >> i >> j >> d;
        edge.push_back(tuple<int, int, int>(i, j, d));
    }
    fmap.close();
    int size = edge.size();

    double start = omp_get_wtime();

#pragma omp parallel num_threads(P_NUM)
    {
        int count = 0;
        bool change = true;
        vector<int> hash(size);
        iota(hash.begin(), hash.end(), 0);
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(hash.begin(), hash.end(), default_random_engine(seed));

        while (change) {
            count++;
            change = false;
            for (auto ind : hash) {
                auto tup = edge[ind];
                if (node[get<1>(tup)] > node[get<0>(tup)] + get<2>(tup)) {
                    node[get<1>(tup)] = node[get<0>(tup)] + get<2>(tup);
                    change = true;
                }

            }
        }
        cout << count << endl;
    }

    double end = omp_get_wtime();
    double time = end - start;
    cout << time << 's' << endl;

    ofstream fans;
    fans.open("output.txt");
    for (int i = 0;i < NODE_NUM;i++) {
        if (node[i] == INT32_MAX / 2) {
            fans << 'X' << endl;
        }
        fans << node[i] << endl;
    }

    return 0;
}