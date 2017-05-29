d='stromovka'
order=0

function extend_figure
{
	w=$(identify $1 |cut -f 3 -d ' '|cut -f 1 -d x)
	h=$(identify $1 |cut -f 3 -d ' '|cut -f 2 -d x)
	if [ $w -lt 500 ]; then	convert $1 -bordercolor white -border $(((500-$w)/2))x0 $1;fi
	if [ $h -lt 500 ]; then	convert $1 -bordercolor white -border 0x$(((500-$h)/2)) $1;fi
	convert $1 -resize 500x500 $1
	w=$(identify $1 |cut -f 3 -d ' '|cut -f 1 -d x)
	h=$(identify $1 |cut -f 3 -d ' '|cut -f 2 -d x)
	if [ $w -lt 500 ]; then	convert $1 -bordercolor white -border $(((500-$w)/2))x0 $1;fi
	if [ $h -lt 500 ]; then	convert $1 -bordercolor white -border 0x$(((500-$h)/2)) $1;fi
	convert $1 -resize 500x500 $1
}

function create_graph
{
	echo digraph 
	echo { 
	echo node [penwidth="2" fontname=\"palatino bold\"]; 
	echo edge [penwidth="2"];	
	for m in $(cut -f 1 -d ' ' scripts/models.tmp)
	do	
		e=0
		for n in $(cut -f 1 -d ' ' scripts/models.tmp)
		do
			#echo -ne Comparing $m and $n' ';
			if [ $(paste benchmarks/$d/$m.txt benchmarks/$d/$n.txt|tr \\t ' '|cut -f 4,9 -d ' '|./scripts/t-test|grep -c higher) == 1 ]
			then
				echo $n  '->' $m ;
				e=1
			fi
		done
		if [ $e == 0 ]; then echo $m;fi
	done
	echo }
}

EXE=../bin/fremen
MAPS=$d/maps
IMAGES=$d/testing
REPORTS=$d/reports
SUMMARY=$d/summary


cd scripts;make;cd ..

if [ ! -e $MAPS ]; then mkdir $MAPS;fi
if [ ! -e $REPORTS ]; then mkdir $REPORTS;fi
if [ ! -e $SUMMARY ]; then mkdir $SUMMARY;fi

./scripts/buildmaps.sh stromovka

for numFeatures in $(seq -w 1 1 50); 
do
echo $EXE test $IMAGES $MAPS $numFeatures $order 
$EXE test $IMAGES $MAPS $numFeatures $order >$REPORTS/$numFeatures-new.txt 2>/dev/null
done

rm -f $SUMMARY/report.pdf
rm -f $SUMMARY/total.txt
rm benchmarks/*

grep Overall $REPORTS/*-new.txt|sed s/.*reports.//|sed s/\-.*\://  |sed s/^5\ .*// >benchmarks/$d/New_method.txt;

create_graph |dot -Tpdf >benchmarks/$d.pdf
convert -density 200 benchmarks/$d.pdf -trim -bordercolor white benchmarks/$d.png 
extend_figure benchmarks/$d.png
cat best.txt |cut -f 2,4 -d ' '|tr ' ' _|sed s/$/.txt/|sed s/^/..\\/results\\//
cp scripts/draw_summary_skelet.gnu benchmarks/draw_summary.gnu
for i in $(cut -f 1 -d ' ' scripts/models.tmp);
do 
	echo \'benchmarks/$d/$i.txt\' 'using 1:(100-$2*100) lw 2 with line title' \'$i\',\\ >>benchmarks/draw_summary.gnu;
done
gnuplot benchmarks/draw_summary.gnu >benchmarks/graphs.fig
fig2dev -Lpdf benchmarks/graphs.fig benchmarks/graphs.pdf
convert -density 200 benchmarks/graphs.pdf benchmarks/graphs.png
extend_figure benchmarks/graphs.png 
convert -size 1100x600 xc:white \
	-draw 'Image src-over 25,100 500,500 'benchmarks/graphs.png'' \
	-draw 'Image src-over 575,100 500,500 'benchmarks/$d.png'' \
	-pointsize 32 \
	-draw 'Text 100,40 "Performance of temporal models for topological localisation"' \
	-pointsize 18 \
	-gravity North \
	-draw 'Text 0,60 "Arrow A->B means that A performs statistically significantly better that B"' benchmarks/summary.png;
cp benchmarks/summary.png  $d/summary/summary.png
