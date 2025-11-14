#include <bits/stdc++.h>
using namespace std;

struct Circle { double x, y, r; };

// прямоугольник выборки для монте-карло
struct Rect {
    double xmin, xmax, ymin, ymax;
    bool empty() const { return !(xmin < xmax && ymin < ymax); }
    double area() const { return (xmax - xmin) * (ymax - ymin); }
};

static inline bool inside_all(double x, double y, const array<Circle,3>& c){
    // сравнение по квадратам радиусов
    for (int i=0;i<3;i++){
        double dx = x - c[i].x, dy = y - c[i].y;
        if (dx*dx + dy*dy > c[i].r * c[i].r) return false;
    }
    return true;
}

// узкая область: пересечение трех полос по x и y
static Rect make_tight(const array<Circle,3>& c){
    double Xmin = max({c[0].x - c[0].r, c[1].x - c[1].r, c[2].x - c[2].r});
    double Xmax = min({c[0].x + c[0].r, c[1].x + c[1].r, c[2].x + c[2].r});
    double Ymin = max({c[0].y - c[0].r, c[1].y - c[1].r, c[2].y - c[2].r});
    double Ymax = min({c[0].y + c[0].r, c[1].y + c[1].r, c[2].y + c[2].r});
    return {Xmin, Xmax, Ymin, Ymax};
}

// широкая область: общий охватывающий прямоугольник
static Rect make_wide(const array<Circle,3>& c){
    double Xmin = min({c[0].x - c[0].r, c[1].x - c[1].r, c[2].x - c[2].r});
    double Xmax = max({c[0].x + c[0].r, c[1].x + c[1].r, c[2].x + c[2].r});
    double Ymin = min({c[0].y - c[0].r, c[1].y - c[1].r, c[2].y - c[2].r});
    double Ymax = max({c[0].y + c[0].r, c[1].y + c[1].r, c[2].y + c[2].r});
    return {Xmin, Xmax, Ymin, Ymax};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    array<Circle,3> c;
    for (int i=0;i<3;i++){
        if(!(cin>>c[i].x>>c[i].y>>c[i].r)) return 0; // читаем 3 окружности
    }

    // берем tight: дает меньшую дисперсию
    Rect R = make_tight(c);

    // если прямоугольник пуст, то площадь 0
    if (R.empty()){
        cout.setf(std::ios::fixed); cout<<setprecision(15)<<0.0<<"\n";
        return 0;
    }

    const double Srec = R.area();
    if (Srec==0.0){
        cout.setf(std::ios::fixed); cout<<setprecision(15)<<0.0<<"\n";
        return 0;
    }

    // фиксированный сид для детерминируемого вывода
    mt19937 rng(712367);
    uniform_real_distribution<double> U01(0.0, 1.0);

    // адаптивная остановка: хотим |ошибка| ≤ 0.01 - держим 3*SE ≤ 0.003
    const double EPS = 0.003;
    const int    BATCH = 300000;       // добавляем по крупным порциям
    const int    NMIN  = 20000;        // сначала накапливаем достаточно точек
    const int    NMAX  = 5000000;      // страховка по количеству
    const double TIME_LIMIT = 0.98;    // и по времени (лимит 1с)

    auto t0 = chrono::steady_clock::now();

    long long N = 0; // сгенерировано
    long long M = 0; // попало в пересечение

    while (true){
        // контроль времени/лимитов
        auto now = chrono::steady_clock::now();
        double elapsed = chrono::duration<double>(now - t0).count();
        if (elapsed > TIME_LIMIT || N >= NMAX) break;

        int step = (int)min<long long>(BATCH, NMAX - N);

        // генерируем step точек в R
        for (int k=0;k<step;k++){
            double x = R.xmin + U01(rng) * (R.xmax - R.xmin);
            double y = R.ymin + U01(rng) * (R.ymax - R.ymin);
            M += inside_all(x,y,c);
        }
        N += step;

        if (N >= NMIN){
            // оценка стандартной ошибки площади
            double p  = (double)M / (double)N;
            double SE = sqrt(max(0.0, p*(1.0 - p) / (double)N)) * Srec;
            if (3.0 * SE <= EPS) break; // достигли требуемой точности
        }
    }

    double Shat = ((double)M / (double)max(1LL,N)) * Srec;
    cout.setf(std::ios::fixed);
    cout<<setprecision(15)<<Shat<<"\n";
    return 0;
}
