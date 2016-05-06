EXE=../bin/fremen
MAPS=$1/maps
IMAGES=$1/testing
REPORTS=$1/reports

if [ ! -e $REPORTS ];
then 
	mkdir $REPORTS
fi

for numFeatures in $(seq -w 1 1 50) 
do
for order in 0 1 2 6 
do
echo ../bin/fremen test $IMAGES $MAPS $numFeatures $order $REPORTS/$numFeatures-$order.txt 
../bin/fremen test $IMAGES $MAPS $numFeatures $order >$REPORTS/$numFeatures-$order.txt 2>/dev/null
done
done
