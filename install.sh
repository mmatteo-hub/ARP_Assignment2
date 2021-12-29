# unzip src.zip file into the path inserted by the user
unzip -q -d $1/ src.zip

# create 2 folder to organise the files
mkdir -p $1/exe
mkdir -p $1/log

# compile .c files, from src folder, and put them into exc folder created
gcc $1/src/master/master.c -o $1/exe/master
gcc $1/src/namedPipe/producerNpipe.c -o $1/exe/producerNpipe
gcc $1/src/namedPipe/consumerNpipe.c -o $1/exe/consumerNpipe
gcc -pthread $1/src/sharedMemory/producerSharedmemory.c -o $1/exe/producerSharedmemory -lrt
gcc -pthread $1/src/sharedMemory/consumerSharedmemory.c -o $1/exe/consumerSharedmemory -lrt
gcc $1/src/socket/producerSocket.c -o $1/exe/producerSocket
gcc $1/src/socket/consumerSocket.c -o $1/exe/consumerSocket
gcc $1/src/unnamedPipe/unnamedPipe.c -o $1/exe/unnamedPipe

# printing on the shell the success of the installation
echo installation succeded
