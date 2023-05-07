#rm -rf build;clear;sh -x ./do_cmake.sh

set -e
now=$(date +'%Y/%m/%d %H:%M:%S')
sed -i "s|std::string build_time.*|std::string build_time = \"$now\";|g" src/global/global_init.cc

cd build
make -j64
#make install
