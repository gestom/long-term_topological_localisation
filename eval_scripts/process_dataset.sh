d=$1

EXE=../bin/fremen
MAPS=../data/$d/maps
IMAGES=../data/$d/testing
TRAINING=../data/$d/training
REPORTS=../data/$d/reports
SUMMARY=../data/$d/summary

make

if [ ! -e $MAPS ]; then mkdir $MAPS;fi
if [ ! -e $REPORTS ]; then mkdir $REPORTS;fi
if [ ! -e $SUMMARY ]; then mkdir $SUMMARY;fi

for i in $(ls $IMAGES/|grep place_*|sed s/place_//)
do
	echo -n Location $i:' ' 
#	$EXE build $TRAINING/place_$i $MAPS/place_$i.all
	echo -n feature map created' '
	$EXE reduce $MAPS/place_$i.all 0.05 $MAPS/place_$i.red 
	echo and pruned.' ' 
done

grep -v '#' ../src/models/test_models.txt >models.tmp

for model in $(cut -f 1 -d ' ' models.tmp)
do	
for order in $(cat models.tmp |grep $model|sed  -e 's/\s\+/\ /g'|cut -f 2-100 -d ' ');
do
for i in $(ls $IMAGES/|grep place_*|sed s/place_//)
do
echo $EXE recalculate $TRAINING/place_$i $MAPS/place_$i.red $MAPS/place_$i.map $model $order
$EXE recalculate $TRAINING/place_$i $MAPS/place_$i.red $MAPS/place_$i.map $model $order
done
for numFeatures in $(seq -w 1 1 50); 
do
echo $EXE test $IMAGES $MAPS $numFeatures $order $model 
$EXE test $IMAGES $MAPS $numFeatures $order $model >$REPORTS/$model\_$order\_$numFeatures.txt 2>/dev/null
done
done
done
