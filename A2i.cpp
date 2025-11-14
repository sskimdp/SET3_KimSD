#include <bits/stdc++.h>
using namespace std;

// вставками на полуинтервале [L, R)
static inline void insertion_sort_range(vector<int>& a, int L, int R) {
    for (int i = L + 1; i < R; ++i) {
        int x = a[i];
        int j = i - 1;
        while (j >= L && a[j] > x) { a[j+1] = a[j]; --j; }
        a[j+1] = x;
    }
}

// слияние двух отсортированных кусков [L, M) и [M, R)
static inline void merge_ranges(vector<int>& a, vector<int>& tmp, int L, int M, int R) {
    int i = L, j = M, k = L;
    while (i < M && j < R) tmp[k++] = (a[i] <= a[j] ? a[i++] : a[j++]);
    while (i < M) tmp[k++] = a[i++];
    while (j < R) tmp[k++] = a[j++];
    for (int t = L; t < R; ++t) a[t] = tmp[t];
}

// рекурсивная часть гибрида: если длина маленькая — переходим на вставки
static void sort_rec(vector<int>& a, vector<int>& tmp, int L, int R, int threshold) {
    int len = R - L;
    if (len <= 1) return;
    if (len <= threshold) { insertion_sort_range(a, L, R); return; }

    int M = L + (len >> 1);
    sort_rec(a, tmp, L, M, threshold);
    sort_rec(a, tmp, M, R, threshold);

    // если уже отсортировано на стыке, слияние не нужно
    if (a[M-1] <= a[M]) return;
    merge_ranges(a, tmp, L, M, R);
}

// публичная обёртка
static inline void merge_insertion_sort(vector<int>& a, int threshold) {
    vector<int> tmp(a.size());
    sort_rec(a, tmp, 0, (int)a.size(), threshold);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    const int THRESHOLD = 15;
    merge_insertion_sort(a, THRESHOLD);

    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << a[i];
    }
    cout << '\n';
    return 0;
}
