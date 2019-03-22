#!/bin/sh

make test

tests=(1 2 3 4 5 6)

for i in "${tests[@]}"
do
  ./t$i > "myout"
  ./r$i > "answer"
  DIFF=`diff -U 0 myout answer | grep ^@ | wc -l`
  if [ $DIFF == 1 ]
  then
    echo "Passed test $i"
  else
    echo "Failed test $i"
  fi
done

./tpa4c > "myout"
./rpa4c > "answer"
DIFF=`diff -U 0 myout answer | grep ^@ | wc -l`
if [ $DIFF == 1 ]
then
  echo "Passed pa4c test"
else
  echo "Failed pa4c test"
fi

