import math
import pandas as pd
import matplotlib.pyplot as plt

S_true = (math.pi/4.0) + 1.25*math.asin(0.8) - 1.0

df = pd.read_csv("results.csv")
# агрегируем по (mode, N)
agg = df.groupby(["mode","N"]).agg(
    S_mean=("Shat","mean"),
    S_std =("Shat","std"),
    count =("Shat","count")
).reset_index()
agg["S_se"] = agg["S_std"] / agg["count"]**0.5
agg["rel_error"] = (agg["S_mean"] - S_true).abs() / S_true

def plot_value_mode(mode, out_png):
    sub = agg[agg["mode"]==mode].sort_values("N")
    N = sub["N"].to_numpy()
    mean = sub["S_mean"].to_numpy()
    se = sub["S_se"].to_numpy()
    plt.figure()
    plt.plot(N, mean, label=f"{mode} mean")
    if se.size and (se>0).any():
        upper = mean + 1.96*se
        lower = mean - 1.96*se
        plt.fill_between(N, lower, upper, alpha=0.2, label="95% CI")
    plt.axhline(S_true, linestyle="--", label="S_true")
    plt.xlabel("N (число точек)")
    plt.ylabel("Оценка площади")
    plt.title(f"Значение оценки площади vs N — {mode}")
    plt.legend()
    plt.tight_layout()
    plt.savefig(out_png, dpi=200)

plot_value_mode("wide",  "S_vs_N_wide.png")
plot_value_mode("tight", "S_vs_N_tight.png")

# график типа 2: относительное отклонение
plt.figure()
for mode in ["wide","tight"]:
    sub = agg[agg["mode"]==mode].sort_values("N")
    plt.plot(sub["N"], sub["rel_error"], label=mode)
plt.yscale("log")
plt.xlabel("N (число точек)")
plt.ylabel("Отн. отклонение |S̄ − S_true| / S_true")
plt.title("Относительное отклонение vs N (лог-шкала по Y)")
plt.legend()
plt.tight_layout()
plt.savefig("RelError_vs_N.png", dpi=200)
