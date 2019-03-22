#!/bin/sh

make test

tests=(1 2 3 4 5 6)

for i in "${tests[@]}"
do
  echo TEST CASE $i DIFFERENCE
  echo
  ./t$i > "myout"
  ./r$i > "answer"
  diff myout answer
  echo
done

echo TEST CASE pa4c DIFFERENCE
echo
./tpa4c > "myout"
./rpa4c > "answer"
diff myout answer

