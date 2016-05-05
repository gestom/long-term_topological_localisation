EXE=../bin/fremen
MAPS=$1/maps

for i in $(seq 0 4);
do
$EXE build training/place_$i $MAPS/place_$i.all
$EXE reduce $MAPS/place_$i.all 0.05 $MAPS/place_$i.red 
$EXE recalculate training/place_$i $MAPS/place_$i.red $MAPS/place_$i.map
done
