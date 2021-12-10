mkdir -p exe

gcc master.c -o exe/master
gcc unnamedPipe.c -o exe/unnamedPipe

cd -- "$(find . -iname exe -type d)"

./master