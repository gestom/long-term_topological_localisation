for p in Nov Feb Dec;do 
echo -ne "$p "
for i in 0 1 2 6;do 
e=$(grep ^50 $1/summary/testing_$p-geometry-maps-$i.txt |cut -f 2 -d ' ')
f=$(printf %02.1f $(echo 100-$e*100|bc))
echo -ne "$f "
done
echo
done

gnuplot ./scripts/plotone.gnu >first.fig
gnuplot ./scripts/plottwo.gnu >second.fig
fig2dev -Lpdf first.fig >first.pdf
fig2dev -Lpdf second.fig >second.pdf
pdftk first.pdf second.pdf cat output report.pdf
