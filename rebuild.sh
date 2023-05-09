set -e
now=$(date +'%Y/%m/%d %H:%M:%S')
sed -i "s|std::string build_time.*|std::string build_time = \"$now\";|g" src/global/global_init.cc

rm -rf build;clear;sh -x ./do_cmake.sh
cp cache/boost_1_66_0.tar.bz2_ build/boost/src/boost_1_66_0.tar.bz2
cp start.sh stop.sh build/
cd build
make -j64 #small_cpu make -j4
#make install

