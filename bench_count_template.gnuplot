set terminal png
#set output 'plot.ps'

set xlabel "Number of elements in associative array" 
set ylabel "Time of each operation (in nanoseconds)"

#set yrange [0:*]

#set linestyle 1 lt 1 lw 3 
#set linestyle 2 lt 2 lw 3 
#set linestyle 3 lt 3 lw 3 

#plot "2-grams.txt" title '2-grams' w l ls 1, \ 
# "3-grams.txt" title '3-grams' w l ls 2, \ 
# "4-grams.txt" title '4-grams' w l ls 3
#plot [*:*] [0:] "bench_count_map_grow.tmp" \
#    smooth frequency \
#    title "SuperTitle" \
#    with linespoints
