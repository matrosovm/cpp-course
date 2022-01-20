g++ -c filesystem.cpp && ar -crsv libfilesystem.a filesystem.o

g++ main.cpp -L . -l filesystem

export LIBRARY_PATH=$(pwd)
g++ main.cpp -l filesystem     
OR
g++ -l filesystem main.cpp 
