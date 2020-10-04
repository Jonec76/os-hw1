#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

long long int c = 1ULL << 32;

void M(long long int *A, long long int p, long long int q, long long int r) {
    long long int n1 = q - p + 1;
    long long int n2 = r - q;
    long long int *L = new long long int[n1 + 1];
    long long int *R = new long long int[n2 + 1];

    for (long long int i = 0; i < n1; i++) {
        L[i] = A[p + i - 1];
    }
    for (long long int j = 0; j < n2; j++) {
        R[j] = A[q + j];
    }
    L[n1] = c;
    R[n2] = c;
    long long int i = 0;
    long long int j = 0;
    for (long long int k = p - 1; k < r; k++) {
        if (L[i] <= R[j]) {
            A[k] = L[i];
            i = i + 1;
        } else {
            A[k] = R[j];
            j = j + 1;
        }
    }
    delete[] L;
    delete[] R;
};

void merge_sort(long long int *A, long long int p, long long int r) {
    if (p < r) {
        long long int q = (p + r) / 2;
        merge_sort(A, p, q);
        merge_sort(A, q + 1, r);
        M(A, p, q, r);
    }
};