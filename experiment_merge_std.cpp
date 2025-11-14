// компилировать: g++ -O2 -std=gnu++17 experiment_merge_std.cpp -o bench_std
#include <bits/stdc++.h>
#include "array_generator.hpp"
#include "mergesort.hpp"
#include "sort_tester.hpp"
using namespace std;

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // параметры запуска: [повторы=7] [seed=2025]
    int repeats = (argc > 1 ? stoi(argv[1]) : 7);
    uint64_t seed = (argc > 2 ? stoull(argv[2]) : 2025);

    GenConfig cfg;                   // 500..100000, шаг 100; значения 0..10000
    ArrayGenerator gen(cfg, seed);

    // CSV заголовок
    cout << "kind,N,trial,ms\n";

    auto sizes = gen.sizes();
    // 3 категории: случайные, обратно отсортированные, почти отсортированные
    struct K { const char* name; ArrayGenerator::Kind kind; };
    vector<K> kinds = {
        {"random",        ArrayGenerator::Kind::Random},
        {"descending",    ArrayGenerator::Kind::Descending},
        {"almost_sorted", ArrayGenerator::Kind::AlmostSorted}
    };

    for (auto k : kinds) {
        for (int n : sizes) {
            auto base = gen.get(n, k.kind);
            // замеряем 'repeats' раз и печатаем по каждому прогону
            for (int r = 0; r < repeats; ++r) {
                long long ms = SortTester::time_once(
                    base, [](vector<int>& a){ ms_std::merge_sort(a); }
                );
                cout << k.name << "," << n << "," << (r+1) << "," << ms << "\n";
            }
        }
    }
    return 0;
}
