// demo_array_generator.cpp
#include <bits/stdc++.h>
#include "array_generator.hpp"
using namespace std;

int main() {
    GenConfig cfg;                       // дефолт строго из задания
    ArrayGenerator gen(cfg, 42);         // фиксированный сид для повторяемости

    // пример: выводим по 5 элементов из трёх типов для N = 500 и N = 100000
    for (int n : {500, 100000}) {
        auto A = gen.get(n, ArrayGenerator::Kind::Random);
        auto B = gen.get(n, ArrayGenerator::Kind::Descending);
        auto C = gen.get(n, ArrayGenerator::Kind::AlmostSorted);

        cout << "N=" << n << "\n";
        cout << "random     : ";
        for (int i=0; i<min(5, n); ++i) cout << A[i] << " ";
        cout << "\n";

        cout << "descending : ";
        for (int i=0; i<min(5, n); ++i) cout << B[i] << " ";
        cout << "\n";

        cout << "almost-sort: ";
        for (int i=0; i<min(5, n); ++i) cout << C[i] << " ";
        cout << "\n\n";
    }

    // пример: как получить «лестницу» размеров
    // vector<int> S = gen.sizes();  // 500, 600, ..., 100000

    return 0;
}
