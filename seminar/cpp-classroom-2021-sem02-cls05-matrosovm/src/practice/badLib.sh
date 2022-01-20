g++ -shared -fPIC big.cpp -o libbig.so
g++ -shared -fPIC disk_utils.cpp -o libdisk_utils.so
g++ -shared -fPIC filesystem.cpp -o libfilesystem.so 

g++ -shared -fshort-enums -fPIC bad_lib.cpp -o libbad_lib.so

g++ main.cpp -L. -lfilesystem -lbig -lbad_lib -ldisk_utils


