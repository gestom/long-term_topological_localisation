set terminal fig color
set multiplot
map="adaptive/summary/testing_Nov-geometry-maps"
set size 0.5,0.8
set origin 0.0,0.0
set title "Indoor environment" 
set xtics 0,10,50
set xlabel "Number of features [-]"
set ylabel "Localization error rate [%]"
plot [0:49][0:100] \
sprintf("%s-1.txt",map)  using 1:(100-$2*100) with lines title "FreMEn - order 1",\
sprintf("%s-2.txt",map)  using 1:(100-$2*100) with lines title "FreMEn - order 2",\
sprintf("%s-0.txt",map)  using 1:(100-$2*100) with lines title "Static model"         

map="latest/summary/testing-geometry-maps"
set size 0.5,0.8
set origin 0.5,0.0
set title "Outdoor environment" 
set xlabel "Number of features [-]"
unset ylabel
#unset ytics
plot [0:49][0:100] \
sprintf("%s-1.txt",map)  using 1:(100-$2*100) with lines title "FreMEn - order 1",\
sprintf("%s-2.txt",map)  using 1:(100-$2*100) with lines title "FreMEn - order 2",\
sprintf("%s-0.txt",map)  using 1:(100-$2*100) with lines title "Static model"        
