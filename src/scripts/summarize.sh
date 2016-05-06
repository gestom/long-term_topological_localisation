REPORTS=$1/reports
SUMMARY=$1/summary

rm -f $SUMMARY/report.pdf
rm -f $SUMMARY/total.txt

if [ ! -e $GRAPHS ]; then mkdir $GRAPHS;fi
if [ ! -e $SUMMARY ]; then mkdir $SUMMARY;fi

for i in $(seq 0 6);do 
grep Overall $REPORTS/*$i.txt|sed s/.*reports.//|sed s/\-.*\://  |sed s/^5\ .*// >$SUMMARY/$i.txt;
done;

gnuplot -e "map='$SUMMARY/'" scripts/plot.gnu >$SUMMARY/error.fig
fig2dev -L png -m 4 $SUMMARY/error.fig >$SUMMARY/error.png
#display $1/graphs/$p-$g-$m.png
fig2dev -L pdf -m 4 $SUMMARY/error.fig >$SUMMARY/error.pdf

#w=1
#echo  -ne "$w "  >>$1/summary/total.txt
#for i in $(seq 0 6);do 
#	a=$(echo 1-$(grep ^25 $1/summary/$p-geometry-maps-$i.txt |cut -f 2 -d ' ')|bc | awk '{printf "%.2f", $0*100}')
#	echo  -ne "$a " >>$1/summary/total.txt
#done
#echo  >>$1/summary/total.txt
#if [ $w == 12 ]; then w=56;fi
#if [ $w == 1 ]; then w=12;fi

cd $GRAPHS
#uuencode testing.pdf testing.pdf|mail -s "FREMEN report" tomkrajnik@hotmail.com
cd ..
cd ..
