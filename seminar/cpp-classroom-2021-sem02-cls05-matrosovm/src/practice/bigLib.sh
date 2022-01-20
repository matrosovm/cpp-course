g++ -shared filesystem.cpp -fPIC -o libfilesystem.so
g++ -shared disk_utils.cpp -fPIC -o libdisk_utils.so
g++ -shared bad_lib.cpp -fPIC -o libbad_lib.so

g++ -c big.cpp && ar -vsrc libbig.a big.o
OR
g++ -shared big.cpp -fPIC -o libbig.so

g++ main.cpp -L. -lfilesystem -lbig -lbad_lib -ldisk_utils
