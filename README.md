# SET3_KimSD  
## A2  
`demo_array_generator.cpp` — короткая проверка генератора: печатает первые элементы для разных N/видов.  
`experiment_merge_std.cpp` — прогон стандартного MERGE SORT на всех N и типах входов; пишет `merge_std_results.csv` (kind,N,trial,ms).  
`experiment_merge_hybrid.cpp` — прогон гибрида на всех N, типах и порогах T∈{5,10,20,30,50}; пишет `merge_hybrid_results.csv` (kind,N,threshold,trial,ms).
  
`mergesort.hpp` — стандартный рекурсивный MERGE SORT.  
`hybrid.hpp` — гибрид MERGE+INSERTION SORT с параметром threshold.  
`array_generator.hpp` — генератор тестовых массивов (random / descending / almost_sorted) и шкалы размеров N.  
`sort_tester.hpp` — стенд замеров: мерит время сортировки на копиях входа (мс, std::chrono).
  
`plot_merge_std.py` — читает `merge_std_results.csv`, строит 3 графика времени vs N, сохраняет `merge_std_random.png`, `merge_std_descending.png`, `merge_std_almost_sorted.png` + агрегированные `merge_std_< kind >_agg.csv`.  

`plot_merge_hybrid.py` — читает `merge_hybrid_results.csv`, строит 3 графика времени vs N с кривыми по threshold, сохраняет `merge_hybrid_< kind >.png` + таблицы `best_threshold_per_N.csv`, `best_threshold_global.csv`.  