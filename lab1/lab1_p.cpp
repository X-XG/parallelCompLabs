#include <iostream>
#include <gmpxx.h>
#include <omp.h>
using namespace std;

#define MAX_N 128000
#define P_NUM 8

int main() {
    mpf_set_default_prec(MAX_N * 4);
    mpf_t base[P_NUM];
    mpf_t ans[P_NUM];

    double start = omp_get_wtime();

#pragma omp parallel for num_threads(P_NUM)
    for (int i = 0;i < P_NUM;i++) {
        int s = i * MAX_N / P_NUM + 1;
        mpf_init_set_ui(ans[i], 0);
        mpf_init_set_ui(base[i], 1);
        for (int j = s;j < s + MAX_N / P_NUM; j++) {
            mpf_add(ans[i], ans[i], base[i]);
            mpf_div_ui(base[i], base[i], j);
        }
    }

    for (int i = 1;i < P_NUM;i++) {
        mpf_mul(ans[i], ans[i], base[i - 1]);
        mpf_add(ans[0], ans[0], ans[i]);
        mpf_mul(base[i], base[i], base[i - 1]);
    }

    double end = omp_get_wtime();
    double time = end - start;
    cout << time << 's' << endl;

    gmp_printf("%.100Ff\n", ans[0]);

    return 0;
}