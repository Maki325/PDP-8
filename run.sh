./compile.sh

if [ ! -d "./output" ]
then
  mkdir output
fi

./build/pdp-8 "input/program-1.asm" "output/program-1.bin"
./build/pdp-8 "input/program-1a.asm" "output/program-1a.bin"
./build/pdp-8 "input/program-2.asm" "output/program-2.bin"
