set terminal postscript
set output 'plot.ps'

set xlabel "Number of words in corpus" 
set ylabel "Number of unique N-grams" 
 
set linestyle 1 lt 1 lw 3 
set linestyle 2 lt 2 lw 3 
set linestyle 3 lt 3 lw 3 
 
plot "2-grams.txt" title '2-grams' w l ls 1, \ 
 "3-grams.txt" title '3-grams' w l ls 2, \ 
 "4-grams.txt" title '4-grams' w l ls 3
