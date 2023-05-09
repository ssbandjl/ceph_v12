
[ `hostname` == "centos7" ] && {
  export PATH=/home/xb/project/stor/ceph/ceph/build/bin:$PATH
  echo "centos7, path:$PATH"
  return 0
}
export PATH=/home/xb/project/stor/ceph/ceph/build/bin:$PATH
