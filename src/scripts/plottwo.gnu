set terminal fig color
set multiplot
map="active/summary/testing_Dec-geometry-maps"
set size 0.5,0.7
set origin 0.0,0.0
set xtics 0,10,50
set title "December 2014" 
set xlabel "Number of features [-]"
set ylabel "Localization error rate [%]" offset 1.5,0
plot [1:49][0:100] \
sprintf("%s-6.txt",map)  using 1:(100-$2*100) lw 2 with lines title "FreMEn - adaptive",\
sprintf("%s-1.txt",map)  using 1:(100-$2*100) lw 2 with lines title "FreMEn - order 1",\
sprintf("%s-2.txt",map)  using 1:(100-$2*100) lw 2 with lines title "FreMEn - order 2",\
sprintf("%s-0.txt",map)  using 1:(100-$2*100) lw 2 with lines title "Static model"

set key samplen 2 Right left
set title "Comparison (50 features)" 
set size 0.5,0.7
set origin 0.5,0.0
set style data histogram
set key top
set style fill solid border -1
set xlabel "Map age [weeks]"
set style histogram cluster gap 1
plot [-0.6:2.6] [0:50] './active/summary/total.txt' using 7:xtic(1) title "FreMEn - adaptive",\
"" using 3 title "FreMEn - order 1",\
"" using 4 title "FreMEn - order 2",\
"" using 2 title "Static model"
