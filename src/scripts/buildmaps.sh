EXE=../bin/fremen
MAPS=$1/maps
IMAGES=$1/training

if [ ! -e $MAPS ]; then mkdir $MAPS;fi

for i in $(ls $IMAGES/|grep place_*|sed s/place_//)
do
	echo -n Location $i:' ' 
	$EXE build $IMAGES/place_$i $MAPS/place_$i.all
	echo -n feature map created' '
	$EXE reduce $MAPS/place_$i.all 0.0 $MAPS/place_$i.red 
	echo -n and pruned.' ' 
	$EXE recalculate $IMAGES/place_$i $MAPS/place_$i.red $MAPS/place_$i.map
	echo FreMEn feature visibility calculated.
done
