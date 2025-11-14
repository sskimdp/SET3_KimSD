#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def read_data(csv_path: str) -> pd.DataFrame:
    df = pd.read_csv(csv_path)
    # приводим типы
    df["N"]  = pd.to_numeric(df["N"],  errors="coerce")
    df["ms"] = pd.to_numeric(df["ms"], errors="coerce")
    df = df.dropna(subset=["N", "ms"]).copy()
    df["N"] = df["N"].astype(int)
    return df

def aggregate_kind(df_kind: pd.DataFrame) -> pd.DataFrame:
    g = df_kind.groupby("N")["ms"]
    out = pd.DataFrame({
        "N": g.median().index.values.astype(int),
        "median": g.median().values.astype(float),
        "p05": g.quantile(0.05).values.astype(float),
        "p95": g.quantile(0.95).values.astype(float),
        "count": g.size().values.astype(int),
    }).sort_values("N")
    return out

def plot_one(agg: pd.DataFrame, kind: str, out_png: str):
    plt.figure()
    # линия — медиана, полоса — 5–95 перцентили
    x = agg["N"].to_numpy()
    y = agg["median"].to_numpy()
    lo = agg["p05"].to_numpy()
    hi = agg["p95"].to_numpy()

    plt.plot(x, y, label="median time, ms")
    plt.fill_between(x, lo, hi, alpha=0.2, label="5–95 percentile")

    plt.xlabel("N")
    plt.ylabel("Время, мс")
    plt.title(f"MERGE SORT — время vs N — {kind}")
    plt.legend()
    plt.tight_layout()
    plt.savefig(out_png, dpi=200)
    plt.close()

def main():
    ap = argparse.ArgumentParser(description="Графики для стандартного MERGE SORT по CSV")
    ap.add_argument("--csv", required=True, help="путь к merge_std_results.csv")
    ap.add_argument("--outdir", default=".", help="куда сохранять png/csv (по умолчанию текущая папка)")
    args = ap.parse_args()

    os.makedirs(args.outdir, exist_ok=True)
    df = read_data(args.csv)

    expected_kinds = ["random", "descending", "almost_sorted"]
    kinds = [k for k in expected_kinds if k in df["kind"].unique().tolist()]
    if not kinds:
        raise SystemExit("В CSV нет ожидаемых значений столбца 'kind'.")

    for k in kinds:
        sub = df[df["kind"] == k]
        agg = aggregate_kind(sub)

        # сохраняем агрегированную таблицу
        out_csv = os.path.join(args.outdir, f"merge_std_{k}_agg.csv")
        agg.to_csv(out_csv, index=False)

        # рисуем график
        out_png = os.path.join(args.outdir, f"merge_std_{k}.png")
        plot_one(agg, k, out_png)

        print(f"[OK] {k:14s} -> {out_png}  | agg: {out_csv}  | N: {agg['N'].min()}..{agg['N'].max()} (rows={len(agg)})")

if __name__ == "__main__":
    main()
