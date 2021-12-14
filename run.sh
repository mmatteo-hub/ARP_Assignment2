mkdir -p exe
mkdir -p log

gcc master.c -o exe/master
gcc unnamedPipe.c -o exe/unnamedPipe
gcc producerNpipe.c -o exe/producerNpipe
gcc consumerNpipe.c -o exe/consumerNpipe
gcc producerSocket.c -o exe/producerSocket
gcc consumerSocket.c -o exe/consumerSocket

cd -- "$(find . -iname exe -type d)"

./master
