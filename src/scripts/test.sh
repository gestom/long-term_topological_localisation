for p in testing 
do
for g in geometry
do
for m in maps
do
for t in $(seq -w 1 1 50) 
do
for o in 0 1 2 6 
do
echo ../bin/fremen test $p $1/$m $t $o $g
../bin/fremen test $p $1/$m $t $o $g 1>$1/reports/$p-$g-$m-$t-$o.txt 2>/dev/null
done
done
done
done
#curl --data "to=07544266481&message=Phase calc complete&hash=6ed6597d85cd54b066cf1e877a8cf037" http://www.smspi.co.uk/send/
done
