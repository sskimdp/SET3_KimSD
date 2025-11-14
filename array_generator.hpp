#pragma once
#include <bits/stdc++.h>
using namespace std;

// фиксируем параметры из задания
struct GenConfig {
    int minN = 500;        // минимальный размер
    int maxN = 100000;     // максимальный размер (базовый массив)
    int step = 100;        // шаг по размерам
    int minVal = 0;        // диапазон значений
    int maxVal = 10000;
    // для "почти отсортированного": доля случайных обменов (по умолчанию 1%)
    double swapFraction = 0.01;
};

// генератор наборов массивов трёх типов
class ArrayGenerator {
    GenConfig cfg;
    mt19937_64 rng;

    // базовые массивы длины maxN, от них берём префиксы
    vector<int> base_random;
    vector<int> base_desc;       // строго невозрастающий
    vector<int> base_almost;     // почти отсортированный

public:
    // сид задаём явно для воспроизводимости
    explicit ArrayGenerator(const GenConfig& c, uint64_t seed = 2025)
        : cfg(c), rng(seed)
    {
        // случайный базовый массив на maxN
        uniform_int_distribution<int> U(cfg.minVal, cfg.maxVal);
        base_random.resize(cfg.maxN);
        for (int i = 0; i < cfg.maxN; ++i) base_random[i] = U(rng);

        // полностью отсортированный по возрастанию
        vector<int> asc = base_random;
        sort(asc.begin(), asc.end());

        // невозрастающий (обратно отсортированный)
        base_desc = asc;
        reverse(base_desc.begin(), base_desc.end());

        // почти отсортированный: берём asc и делаем немного случайных обменов
        base_almost = asc;
        make_almost_sorted_inplace(base_almost);
    }

    // список размеров: 500, 600, ..., 100000
    vector<int> sizes() const {
        vector<int> s;
        for (int n = cfg.minN; n <= cfg.maxN; n += cfg.step) s.push_back(n);
        return s;
    }

    // получить префикс нужного размера для указанного типа
    enum class Kind { Random, Descending, AlmostSorted };

    vector<int> get(int n, Kind k) const {
        // берём префикс из соответствующего базового массива
        if (n < 0 || n > cfg.maxN) return {};
        switch (k) {
            case Kind::Random:        return vector<int>(base_random.begin(),  base_random.begin()+n);
            case Kind::Descending:    return vector<int>(base_desc.begin(),    base_desc.begin()+n);
            case Kind::AlmostSorted:  return vector<int>(base_almost.begin(),  base_almost.begin()+n);
        }
        return {};
    }

private:
    // делаем "почти отсортированный": небольшое число обменов случайных пар
    void make_almost_sorted_inplace(vector<int>& a) {
        int n = (int)a.size();
        // количество обменов: max(1, floor(f*n)), но не больше n/2
        int s = (int)floor(cfg.swapFraction * n);
        s = max(1, min(s, n/2));
        uniform_int_distribution<int> idx(0, n-1);
        for (int t = 0; t < s; ++t) {
            int i = idx(rng), j = idx(rng);
            if (i == j) continue;
            swap(a[i], a[j]);
        }
    }
};
