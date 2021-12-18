# unzip src.zip file into the path inserted
unzip -q -d $1/ src.zip

# create 2 folder to organise the files
mkdir $1/exe
mkdir $1/log

# compile .c files, from src folder, and put them into exc folder created
gcc $1/src/master/master.c -o $1/exe/master
gcc $1/src/namedPipe/producerNpipe.c -o $1/exe/producerNpipe
gcc $1/src/namedPipe/consumerNpipe.c -o $1/exe/consumerNpipe
gcc $1/src/sharedMemory/producerSharedmemory.c -o $1/exe/producerSharedmemory
gcc $1/src/sharedMemory/consumerSharedmemory.c -o $1/exe/consumerSharedmemory
gcc $1/src/socket/producerSocket.c -o $1/exe/producerSocket
gcc $1/src/socket/consumerSocket.c -o $1/exe/consumerSocket
gcc $1/src/unnamedPipe/unnamedPipe.c -o $1/exe/unnamedPipe

# printing on the shell
echo installation succeded
