g++ -shared disk_utils.cpp -o libdisk_utils.so

export LD_LIBRARY_PATH=$(pwd):$LD_LIBRARY_PATH
g++ main.cpp -lfilesystem -ldisk_utils

# чтобы посмотреть, где ищется либа
LD_DEBUG=libs ldd a.out
