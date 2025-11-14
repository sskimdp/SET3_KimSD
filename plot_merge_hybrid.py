import argparse, os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def read_data(csv_path: str) -> pd.DataFrame:
    df = pd.read_csv(csv_path)
    df["N"] = pd.to_numeric(df["N"], errors="coerce")
    df["threshold"] = pd.to_numeric(df["threshold"], errors="coerce")
    df["ms"] = pd.to_numeric(df["ms"], errors="coerce")
    df = df.dropna(subset=["N","threshold","ms"]).copy()
    df["N"] = df["N"].astype(int)
    df["threshold"] = df["threshold"].astype(int)
    return df

def aggregate(df: pd.DataFrame) -> pd.DataFrame:
    g = df.groupby(["kind","threshold","N"])["ms"]
    agg = pd.DataFrame({
        "median": g.median(),
        "p05": g.quantile(0.05),
        "p95": g.quantile(0.95),
        "count": g.size()
    }).reset_index().sort_values(["kind","threshold","N"])
    return agg

def plot_by_kind(agg: pd.DataFrame, kind: str, out_png: str):
    plt.figure()
    sub = agg[agg["kind"]==kind]
    for T, part in sub.groupby("threshold"):
        part = part.sort_values("N")
        x = part["N"].to_numpy()
        y = part["median"].to_numpy()
        plt.plot(x, y, label=f"T={T}")
    plt.xlabel("N")
    plt.ylabel("Время, мс")
    plt.title(f"MERGE+INSERTION — время vs N — {kind}")
    plt.legend(title="threshold")
    plt.tight_layout()
    plt.savefig(out_png, dpi=200)
    plt.close()

def best_threshold_summary(agg: pd.DataFrame) -> pd.DataFrame:
    # для каждого (kind,N) выбрать threshold с минимальной медианой
    rows = []
    for (kind, N), part in agg.groupby(["kind","N"]):
        i = part["median"].idxmin()
        rows.append({
            "kind": kind,
            "N": N,
            "best_T": int(agg.loc[i,"threshold"]),
            "best_median_ms": float(agg.loc[i,"median"])
        })
    best = pd.DataFrame(rows)
    # ещё «глобальный» лучший T по виду (по всем N)
    glob = best.groupby(["kind","best_T"]).size().reset_index(name="wins")
    glob = glob.sort_values(["kind","wins"], ascending=[True, False])
    return best, glob

def main():
    ap = argparse.ArgumentParser(description="Графики гибрида MERGE+INSERTION по CSV")
    ap.add_argument("--csv", required=True, help="merge_hybrid_results.csv")
    ap.add_argument("--outdir", default=".", help="куда сохранять png/csv")
    args = ap.parse_args()

    os.makedirs(args.outdir, exist_ok=True)
    df = read_data(args.csv)
    agg = aggregate(df)

    # графики по видам
    kinds = ["random","descending","almost_sorted"]
    for k in kinds:
        if k in agg["kind"].unique():
            out_png = os.path.join(args.outdir, f"merge_hybrid_{k}.png")
            plot_by_kind(agg, k, out_png)
            print(f"[OK] plot: {out_png}")

    # таблица «лучший threshold по каждому N» и «кто чаще выигрывает»
    best, glob = best_threshold_summary(agg)
    best.to_csv(os.path.join(args.outdir, "best_threshold_per_N.csv"), index=False)
    glob.to_csv(os.path.join(args.outdir, "best_threshold_global.csv"), index=False)
    print("[OK] tables: best_threshold_per_N.csv, best_threshold_global.csv")

if __name__ == "__main__":
    main()
