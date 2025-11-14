#include <bits/stdc++.h>
#include "array_generator.hpp"
#include "sort_tester.hpp"
#include "hybrid.hpp"
using namespace std;

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // параметризуем повторы и seed
    int repeats = (argc > 1 ? stoi(argv[1]) : 7);
    uint64_t seed = (argc > 2 ? stoull(argv[2]) : 2025);

    // набор порогов
    vector<int> thresholds = {5, 10, 20, 30, 50};

    GenConfig cfg;
    ArrayGenerator gen(cfg, seed);

    struct K { const char* name; ArrayGenerator::Kind kind; };
    vector<K> kinds = {
        {"random",        ArrayGenerator::Kind::Random},
        {"descending",    ArrayGenerator::Kind::Descending},
        {"almost_sorted", ArrayGenerator::Kind::AlmostSorted}
    };

    // CSV заголовок
    cout << "kind,N,threshold,trial,ms\n";

    auto sizes = gen.sizes();
    for (auto k : kinds) {
        for (int n : sizes) {
            auto base = gen.get(n, k.kind);
            for (int T : thresholds) {
                for (int r = 0; r < repeats; ++r) {
                    long long ms = SortTester::time_once(
                        base,
                        [T](vector<int>& a){ hybrid::merge_insertion_sort(a, T); }
                    );
                    cout << k.name << "," << n << "," << T << "," << (r+1) << "," << ms << "\n";
                }
            }
        }
    }
    return 0;
}
