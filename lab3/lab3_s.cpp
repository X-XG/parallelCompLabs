#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>
#include <time.h>
using namespace std;

#define DATA_PATH "data.txt"
#define M 2000
#define N 200000
#define TYPE int

int main(int argc, char* argv[]) {
    int size;
    int i, j;
    TYPE value;
    TYPE vec[N];
    TYPE res[M];
    clock_t start, end;
    ifstream fin;
    ofstream fout;
    pair<int, TYPE> tmp;
    pair<int, TYPE>line[N];
    vector<vector<pair<int, TYPE> > >mat(M);


    int a, b;
    TYPE c;
    fin.open(DATA_PATH);

    for (i = 0;i < N;i++) {
        fin >> vec[i];
    }

    while (!fin.eof()) {
        fin >> a >> b >> c;
        tmp = pair<int, TYPE>(b, c);
        mat[a].push_back(tmp);
    }

    fin.close();

    start = clock();

    for (i = 0;i < M;i++) {
        value = 0;
        int receive_count = 0;
        memcpy(line, mat[i].data(), sizeof(pair<int, TYPE>) * mat[i].size());
        receive_count = mat[i].size();

        for (j = 0;j < receive_count;j++) {
            value += line[j].second * vec[line[j].first];
        }

        res[i] = value;

    }

    end = clock();

    cout << (double)(end - start) / CLOCKS_PER_SEC << endl;
    return 0;
}
