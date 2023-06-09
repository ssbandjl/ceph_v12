#!/usr/bin/env bash
set -x
git submodule update --init --recursive
if test -e build; then
    echo 'build dir already exists; rm -rf build and re-run'
    exit 1
fi

ARGS=""
if which ccache ; then
    echo "enabling ccache"
    ARGS="$ARGS -DWITH_CCACHE=ON"
fi

mkdir build
cd build
ARGS+=" -DWITH_LTTNG=ON"
ARGS+=" -DWITH_BLKIN=ON"
ARGS+=" -DWITH_EVENTTRACE=OFF"
ARGS+=" -DWITH_OSD_INSTRUMENT_FUNCTIONS=OFF"
echo -e "cmd:cmake -DBOOST_J=$(nproc) $ARGS $@ .."
cmake -DBOOST_J=$(nproc) $ARGS "$@" ..

# minimal config to find plugins
cat <<EOF > ceph.conf
plugin dir = lib
erasure code dir = lib
EOF

# give vstart a (hopefully) unique mon port to start with
echo $(( RANDOM % 1000 + 40000 )) > .ceph_port

echo done.
