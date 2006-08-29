set terminal png
#set output 'plot.ps'

set logscale x 2
#set logscale y 1.2

set xrange   [1:*]
set yrange   [1:5000]

set xlabel "Slowness of hash function" 
set ylabel "Time of the operation (in nanoseconds)"
