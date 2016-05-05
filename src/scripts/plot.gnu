set terminal fig color
set title "Localization results - Noveber 2013" 
set xlabel "Number of image features selected [-]"
set ylabel "Localization error rate [%]"
plot [0:49][0:99] \
sprintf("%s-0.txt",map)  using 1:(100-$2*100) with lines title "Static model",\
sprintf("%s-1.txt",map)  using 1:(100-$2*100) with lines title "FreMEn - order 1",\
sprintf("%s-2.txt",map)  using 1:(100-$2*100) with lines title "FreMEn - order 2",\
sprintf("%s-6.txt",map)  using 1:(100-$2*100) with lines title "FreMEn - adaptive"
#sprintf("%s-3.txt",map)  using 1:(1-$2) with lines title "Spectral order 3",\
#sprintf("%s-4.txt",map)  using 1:(1-$2) with lines title "Spectral order 4",\
#sprintf("%s-5.txt",map)  using 1:(1-$2) with lines title "Spectral order 5",\
