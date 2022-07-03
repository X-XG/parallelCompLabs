#include <vector>
#include <mpi.h>
#include <fstream>
#include <iostream>
using namespace std;

#define DATA_PATH "data.txt"
#define OUTPUT_PATH "output.txt"
#define M 2000
#define N 200000

int main(int argc, char* argv[]) {
    int rank;
    int size;
    int i, j;
    int sended;
    int received;
    int value;
    int vec[N];
    int res[M];
    double start, end;
    MPI_Status status;
    ifstream fin;
    ofstream fout;
    pair<int, int> tmp;
    pair<int, int>line[N];
    vector<vector<pair<int, int> > >mat(M);

    MPI_Init(0, 0);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        int a, b, c;
        fin.open(DATA_PATH);

        for (i = 0;i < N;i++) {
            fin >> vec[i];
        }

        while (!fin.eof()) {
            fin >> a >> b >> c;
            tmp = pair<int, int>(b, c);
            mat[a].push_back(tmp);
        }

        fin.close();
    }

    if (rank == 0) {
        start = MPI_Wtime();
    }

    MPI_Bcast(vec, N, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (sended = 0;sended < size - 1;sended++) {
            MPI_Send(mat[sended].data(), mat[sended].size(), MPI_2INT, sended + 1, sended, MPI_COMM_WORLD);
        }
    }

    if (rank == 0) {
        received = 0;
        while (true) {
            MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            res[status.MPI_TAG] = value;
            received++;
            if (received == M) {
                end = MPI_Wtime();
                cout << (end - start) << 's' << endl;
                fout.open(OUTPUT_PATH);
                for (i = 0;i < M;i++) {
                    fout << res[i] << endl;
                }
                // MPI_Finalize();
                exit(0);
            }
            else if (sended < M) {
                MPI_Send(mat[sended].data(), mat[sended].size(), MPI_2INT, status.MPI_SOURCE, sended, MPI_COMM_WORLD);
                sended++;
            }
        }
    }
    else {
        while (true) {
            value = 0;
            int receive_count = 0;
            MPI_Recv(line, N + 1, MPI_2INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_2INT, &receive_count);

            for (i = 0;i < receive_count;i++) {
                value += line[i].second * vec[line[i].first];
            }

            MPI_Send(&value, 1, MPI_INT, 0, status.MPI_TAG, MPI_COMM_WORLD);
        }
    }

    return 0;
}
