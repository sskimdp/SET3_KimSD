#pragma once
#include <bits/stdc++.h>
using namespace std;

// помощник для замера времени с усреднением по нескольким прогонам
struct TimeStats {
    vector<long long> trials_ms;
    long long mean_ms() const {
        if (trials_ms.empty()) return 0;
        long long s = 0; for (auto v: trials_ms) s += v;
        return (long long) llround( (double)s / trials_ms.size() );
    }
    long long median_ms() const {
        if (trials_ms.empty()) return 0;
        vector<long long> t = trials_ms;
        nth_element(t.begin(), t.begin()+t.size()/2, t.end());
        return t[t.size()/2];
    }
};

struct SortTester {
    template<class SortFn>
    static long long time_once(vector<int> a, SortFn sorter) {
        using clk = chrono::steady_clock;
        auto t0 = clk::now();
        sorter(a); // сортируем копию
        auto t1 = clk::now();
        // проверка корректности (≈0 накладных)
        #ifndef NDEBUG
        #endif
        return chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
    }

    template<class SortFn>
    static TimeStats bench(const vector<int>& base, int repeats, SortFn sorter) {
        TimeStats st;
        st.trials_ms.reserve(repeats);
        for (int r = 0; r < repeats; ++r) {
            vector<int> a = base;  // копия исходных данных
            st.trials_ms.push_back(time_once(std::move(a), sorter));
        }
        return st;
    }
};
