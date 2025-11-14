#include <bits/stdc++.h>
using namespace std;

struct Circle { double x,y,r; };
struct Rect {
    double xmin, xmax, ymin, ymax;
    bool empty() const { return !(xmin < xmax && ymin < ymax); }
    double area() const { return (xmax - xmin) * (ymax - ymin); }
};

static Rect make_wide(const array<Circle,3>& c){
    double Xmin = min({c[0].x - c[0].r, c[1].x - c[1].r, c[2].x - c[2].r});
    double Xmax = max({c[0].x + c[0].r, c[1].x + c[1].r, c[2].x + c[2].r});
    double Ymin = min({c[0].y - c[0].r, c[1].y - c[1].r, c[2].y - c[2].r});
    double Ymax = max({c[0].y + c[0].r, c[1].y + c[1].r, c[2].y + c[2].r});
    return {Xmin, Xmax, Ymin, Ymax};
}
static Rect make_tight(const array<Circle,3>& c){
    double Xmin = max({c[0].x - c[0].r, c[1].x - c[1].r, c[2].x - c[2].r});
    double Xmax = min({c[0].x + c[0].r, c[1].x + c[1].r, c[2].x + c[2].r});
    double Ymin = max({c[0].y - c[0].r, c[1].y - c[1].r, c[2].y - c[2].r});
    double Ymax = min({c[0].y + c[0].r, c[1].y + c[1].r, c[2].y + c[2].r});
    return {Xmin, Xmax, Ymin, Ymax};
}

static inline bool inside_all(double x, double y, const array<Circle,3>& c){
    for(int i=0;i<3;i++){
        double dx = x - c[i].x, dy = y - c[i].y;
        if (dx*dx + dy*dy > c[i].r * c[i].r) return false;
    }
    return true;
}

// оценка для фиксированного N и выбранного прямоугольника
static double mc_estimate(long long N, const Rect& R, const array<Circle,3>& c, mt19937_64& rng){
    if (R.empty()) return 0.0;
    uniform_real_distribution<double> U(0.0, 1.0);
    long long M=0;
    const double w = R.xmax - R.xmin, h = R.ymax - R.ymin;
    for(long long i=0;i<N;i++){
        double x = R.xmin + U(rng)*w;
        double y = R.ymin + U(rng)*h;
        M += inside_all(x,y,c);
    }
    return (double)M / (double)N * R.area();
}

int main(int argc, char** argv){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    array<Circle,3> C {{
        {1.0, 1.0, 1.0},
        {1.5, 2.0, sqrt(5.0)/2.0},
        {2.0, 1.5, sqrt(5.0)/2.0}
    }};
    Rect Rwide  = make_wide(C);
    Rect Rtight = make_tight(C);

    // диапазон N и число повторов — можно менять
    long long Nmin = 100, Nmax = 100000, step = 500;
    int repeats = 30; // K=30 повторов
    unsigned long long seed0 = 2025;

    if (argc>=6){
        Nmin   = atoll(argv[1]);
        Nmax   = atoll(argv[2]);
        step   = atoll(argv[3]);
        repeats= atoi(argv[4]);
        seed0  = strtoull(argv[5], nullptr, 10);
    }

    // CSV-вывод в stdout: mode,N,seed,Shat,time_ms
    cout << "mode,N,seed,Shat,time_ms\n";
    for (string mode : {"wide","tight"}){
        const Rect& R = (mode=="wide"? Rwide : Rtight);
        for (long long N = Nmin; N <= Nmax; N += step){
            for (int rep=0; rep<repeats; ++rep){
                unsigned long long seed = seed0 + rep + (unsigned long long)N*1315423911ULL;
                mt19937_64 rng(seed);
                auto t0 = chrono::steady_clock::now();
                double Shat = mc_estimate(N, R, C, rng);
                auto t1 = chrono::steady_clock::now();
                double ms = chrono::duration<double, std::milli>(t1 - t0).count();
                cout << mode << "," << N << "," << seed << "," << std::setprecision(15) << std::fixed << Shat << "," << ms << "\n";
            }
        }
    }
    return 0;
}
