rm -f results.csv

#Test Frequency
echo "Frequency:," >> results.csv
for F0 in {1..2}
do
  for F1 in {1..2}
  do
    for F2 in {1..2}
    do
      mpsim.x -c3 -F0,$F0 -F1,$F1 -F2,$F2
      BUSY=$(grep "Bus" ./stats.txt | grep "busy")
      TRANS=$(grep "Bus" ./stats.txt | grep "transferring")
      echo $F0" "$F1" "$F2,$BUSY,$TRANS, >> results.csv
    done
  done
done
