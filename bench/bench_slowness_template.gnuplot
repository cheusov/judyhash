set terminal png
#set output 'plot.ps'

set logscale x 2
#set logscale y 1.2

set xrange   [0:*]
set yrange   [0:5000]

set xlabel "Slowness of compare function" 
set ylabel "Execution time of one operation (in nanoseconds)"
