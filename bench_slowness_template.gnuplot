set terminal png
#set output 'plot.ps'

set xrange   [1:*]
set yrange   [1:2000]

set logscale x 2

set xlabel "Comparison function slowness" 
set ylabel "Time of each operation (in nanoseconds)"
