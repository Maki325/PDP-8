./compile.sh

if [ ! -d "./output" ]
then
  mkdir output
fi

./build/pdp-8 "input/program-1.asm" "output/program-1.bin"
