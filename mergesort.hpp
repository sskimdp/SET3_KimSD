#pragma once
#include <bits/stdc++.h>
using namespace std;

// стандартный рекурсивный merge sort с дополнительной памятью O(n)
// temp-буфер выделяем один раз и протягиваем по рекурсии
namespace ms_std {

static void merge_ranges(vector<int>& a, vector<int>& tmp, int L, int M, int R) {
    int i = L, j = M, k = L;
    while (i < M && j < R) tmp[k++] = (a[i] <= a[j] ? a[i++] : a[j++]);
    while (i < M) tmp[k++] = a[i++];
    while (j < R) tmp[k++] = a[j++];
    // копируем обратно
    for (int t = L; t < R; ++t) a[t] = tmp[t];
}

static void sort_rec(vector<int>& a, vector<int>& tmp, int L, int R) {
    if (R - L <= 1) return;
    int M = L + ((R - L) >> 1);
    sort_rec(a, tmp, L, M);
    sort_rec(a, tmp, M, R);
    // маленькая оптимизация: если уже отсортировано, слияние не нужно
    if (a[M-1] <= a[M]) return;
    merge_ranges(a, tmp, L, M, R);
}

inline void merge_sort(vector<int>& a) {
    vector<int> tmp(a.size());
    sort_rec(a, tmp, 0, (int)a.size());
}

inline bool is_sorted_nondec(const vector<int>& a) {
    for (size_t i = 1; i < a.size(); ++i)
        if (a[i] < a[i-1]) return false;
    return true;
}

}
