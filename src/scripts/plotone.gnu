set terminal fig color
set multiplot
map="active/summary/testing_Nov-geometry-maps"
set size 0.5,0.7
set origin 0.0,0.0
set title "November 2013" 
set xtics 0,10,50
set style line 1 lw 2
set key samplen 2
set xlabel "Number of features [-]"
set ylabel "Localization error rate [%]" offset 1.5,0
plot [1:49][0:100] \
sprintf("%s-6.txt",map)  using 1:(100-$2*100) lw 2 with lines title "FreMEn - adaptive",\
sprintf("%s-1.txt",map)  using 1:(100-$2*100) lw 2 with lines title "FreMEn - order 1",\
sprintf("%s-2.txt",map)  using 1:(100-$2*100) lw 2 with lines title "FreMEn - order 2",\
sprintf("%s-0.txt",map)  using 1:(100-$2*100) lw 2 with lines title "Static model"         

map="active/summary/testing_Feb-geometry-maps"
set origin 0.5,0.0
set title "February 2014" 
set xlabel "Number of features [-]"
plot [1:49][0:100] \
sprintf("%s-6.txt",map)  using 1:(100-$2*100) lw 2 with lines title "FreMEn - adaptive",\
sprintf("%s-1.txt",map)  using 1:(100-$2*100) lw 2 with lines title "FreMEn - order 1",\
sprintf("%s-2.txt",map)  using 1:(100-$2*100) lw 2 with lines title "FreMEn - order 2",\
sprintf("%s-0.txt",map)  using 1:(100-$2*100) lw 2 with lines title "Static model"        

