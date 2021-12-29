# look for the presence of the exe folder created, containing all executables files and move into it
cd -- "$(find . -iname exe -type d)"

# runf the master process
./master
