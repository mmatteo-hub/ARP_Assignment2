mkdir -p exe
mkdir -p log

gcc master.c -o exe/master
gcc unnamedPipe.c -o exe/unnamedPipe
gcc producerNpipe.c -o exe/producerNpipe
gcc consumerNpipe.c -o exe/consumerNpipe

cd -- "$(find . -iname exe -type d)"

./master