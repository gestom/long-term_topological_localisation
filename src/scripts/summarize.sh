rm $1/graphs/report.pdf
rm $1/summary/total.txt
for p in testing;do 
for g in geometry;do 
for m in maps;do 
for i in $(seq 0 6);do 
grep Overall $1/reports/$p-$g-$m*$i.txt|sed s/.*maps.//|sed s/\-.*\://  |sed s/^5\ .*// >$1/summary/$p-$g-$m-$i.txt;
done;
gnuplot -e "map='$1/summary/$p-$g-$m'" scripts/plot.gnu >$1/graphs/$p-$g-$m.fig
fig2dev -L png -m 4 $1/graphs/$p-$g-$m.fig >$1/graphs/$p-$g-$m.png
#display $1/graphs/$p-$g-$m.png
fig2dev -L pdf -m 4 $1/graphs/$p-$g-$m.fig >$1/graphs/$p-$g-$m.pdf
done;
done;
done

w=1
for p in testing;do
echo  -ne "$w "  >>$1/summary/total.txt
for i in $(seq 0 6);do 
	a=$(echo 1-$(grep ^25 $1/summary/$p-geometry-maps-$i.txt |cut -f 2 -d ' ')|bc | awk '{printf "%.2f", $0*100}')
	echo  -ne "$a " >>$1/summary/total.txt
done
echo  >>$1/summary/total.txt
if [ $w == 12 ]; then w=56;fi
if [ $w == 1 ]; then w=12;fi
done

cd $1/graphs
uuencode testing.pdf testing.pdf|mail -s "FREMEN report" tomkrajnik@hotmail.com
cd ..
cd ..
