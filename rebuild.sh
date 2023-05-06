rm -rf build;clear;sh -x ./do_cmake.sh
cp cache/boost_1_66_0.tar.bz2 /home/xb/project/stor/ceph/ceph/build/boost/src/boost_1_66_0.tar.bz2
cd build
make -j64
#make install
