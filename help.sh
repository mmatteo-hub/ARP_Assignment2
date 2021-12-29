# adding path and file .txt to the help.sh file to show in the shell
cd -- "$(find . -iname src -type d)"

# visualise only 1 .tct per time
less master/master.txt
less unnamedPipe/unnamedPipe.txt
less namedPipe/namedPipe.txt
less socket/socket.txt
less sharedMemory/sharedMemory.txt
