d=$1

function extend_figure
{
	w=$(identify $1 |cut -f 3 -d ' '|cut -f 1 -d x)
	h=$(identify $1 |cut -f 3 -d ' '|cut -f 2 -d x)
	if [ $w -lt 500 ]; then	convert $1 -bordercolor white -border $(((500-$w)/2))x0 $1;fi
	if [ $h -lt 400 ]; then	convert $1 -bordercolor white -border 0x$(((400-$h)/2)) $1;fi
	convert $1 -resize 500x400 $1
	w=$(identify $1 |cut -f 3 -d ' '|cut -f 1 -d x)
	h=$(identify $1 |cut -f 3 -d ' '|cut -f 2 -d x)
	if [ $w -lt 500 ]; then	convert $1 -bordercolor white -border $(((500-$w)/2))x0 $1;fi
	if [ $h -lt 400 ]; then	convert $1 -bordercolor white -border 0x$(((400-$h)/2)) $1;fi
	convert $1 -resize 500x400 $1
}

function create_graph
{
	echo digraph 
	echo { 
	echo node [penwidth="2" fontname=\"palatino bold\"]; 
	echo edge [penwidth="2"]; 
	for m in $(cut -f 1 -d ' ' models.tmp)
	do	
		e=0
		for n in $(cut -f 1 -d ' ' models.tmp)
		do
			if [ $(paste $m.txt $n.txt|tr \\t ' '|cut -f 2,4 -d ' '|./t-test|grep -c higher) == 1 ]
			then
				echo $(grep $n best.txt|cut -d ' ' -f 2,4|sed s/' '/_/|sed s/\_0//) '->' $(grep $m best.txt|cut -d ' ' -f 2,4|sed s/' '/_/|sed s/\_0//) ;
				e=1
			fi
		done
		if [ $e == 0 ]; then echo $(grep $m best.txt|cut -d ' ' -f 2,4|sed s/' '/_/|sed s/\_0//);fi
	done
	echo }
}

REPORTS=../data/$d/reports
rm best.txt
grep -v '#' ../src/models/test_models.txt >models.tmp
cat ../src/models/test_models.txt |tr -d '#' >models.tmp
for model in $(cut -f 1 -d ' ' models.tmp)
do
	errmin=100
	indmin=0
	for order in $(cat models.tmp |grep $model|sed  -e 's/\s\+/\ /g'|cut -f 2-100 -d ' ');
	do
		#err=$(grep Overall $REPORTS/$model\_$order*|cut -f 3,4 -d ' '|awk '{a=a+$1+$2}END{print a}'|sort -nr|tail -n 1)	#TODO this takes into account only the best result. In reality, it should take all results into consideration 
		err=$(grep Overall $REPORTS/$model\_$order*|cut -f 3,4 -d ' '|awk '{a=$1+$2}END{print a}'|sort -nr|tail -n 1)	#TODO this takes into account only the best result. In reality, it should take all results into consideration 
		echo $err
		sm=$(echo $err $errmin|awk '{a=0}($1 > $2){a=1}{print a}')
		if [ $sm == 0 ];
		then
			errmin=$err
			indmin=$order
		fi
	done
	grep Overall $REPORTS/$model\_$indmin* |sed s/.*$model.$indmin.//|sed s/.txt.Overall.//|sort -n|cut -f 1,3,4 -d ' '|awk '{print $1,$2+$3}' >$model.txt
	echo Model $model param $indmin has $errmin error.  >>best.txt
done
create_graph |dot -Tpdf >$d.pdf

convert -density 200 $d.pdf -trim -bordercolor white $d.png 
extend_figure $d.png
cat best.txt |cut -f 2,4 -d ' '|tr ' ' _|sed s/$/.txt/|sed s/^/..\\/results\\/$d\\//
f=0
n=$((1+$(cat models.tmp|grep -c .)));
cp draw_summary_skelet.gnu draw_summary.gnu
for i in $(cut -f 1 -d ' ' models.tmp);
do 
	echo $i
	echo \'$i.txt\' 'using 1:($2*100) with lines title' \'$i\',\\ >>draw_summary.gnu;
	f=$(($f+1))
done
gnuplot draw_summary.gnu >graphs.fig
fig2dev -Lpdf graphs.fig graphs.pdf
convert -density 200 graphs.pdf graphs.png
extend_figure graphs.png 
convert -size 900x450 xc:white \
	-draw 'Image src-over 25,50 500,400 'graphs.png'' \
	-draw 'Image src-over 525,90 375,300 '$d.png'' \
	-pointsize 24 \
	-draw 'Text 20,30 "Performance of temporal models for localisation in changing indoor environments"' \
	-pointsize 16 \
	-gravity North \
	-draw 'Text 0,40 "Arrow A->B means that A performs statistically significantly better that B"' summary.png;
cp summary.png  ../data/$d/results/summary.png
