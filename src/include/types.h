// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*- 
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2004-2006 Sage Weil <sage@newdream.net>
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software 
 * Foundation.  See file COPYING.
 * 
 */
#ifndef CEPH_TYPES_H
#define CEPH_TYPES_H

// this is needed for ceph_fs to compile in userland
#include "int_types.h"
#include "byteorder.h"

#include "uuid.h"

#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>

#ifndef __FILENAME__
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif
#ifndef __FFL__
#define __FFL__ "" << __func__ << " " << __FILENAME__ << ":" << __LINE__
#endif

// <macro hackery>
// temporarily remap __le* to ceph_le* for benefit of shared kernel/userland headers
#define __le16 ceph_le16
#define __le32 ceph_le32
#define __le64 ceph_le64
#include "ceph_fs.h"
#include "ceph_frag.h"
#include "rbd_types.h"
#undef __le16
#undef __le32
#undef __le64
// </macro hackery>


#ifdef __cplusplus
#ifndef _BACKWARD_BACKWARD_WARNING_H
#define _BACKWARD_BACKWARD_WARNING_H   // make gcc 4.3 shut up about hash_*
#endif
#endif

extern "C" {
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "statlite.h"
}

#include <string>
#include <list>
#include <set>
#include <map>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

#include "include/unordered_map.h"

#include "object.h"
#include "intarith.h"

#include "acconfig.h"

#include "assert.h"

// DARWIN compatibility
#ifdef DARWIN
typedef long long loff_t;
typedef long long off64_t;
#define O_DIRECT 00040000
#endif

// FreeBSD compatibility
#ifdef __FreeBSD__
typedef off_t loff_t;
typedef off_t off64_t;
#endif

#if defined(__sun) || defined(_AIX)
typedef off_t loff_t;
#endif


// -- io helpers --

// Forward declare all the I/O helpers so strict ADL can find them in
// the case of containers of containers. I'm tempted to abstract this
// stuff using template templates like I did for denc.

template<class A, class B>
inline ostream& operator<<(ostream&out, const pair<A,B>& v);
template<class A, class Alloc>
inline ostream& operator<<(ostream& out, const vector<A,Alloc>& v);
template<class A, class Comp, class Alloc>
inline ostream& operator<<(ostream& out, const deque<A,Alloc>& v);
template<class A, class B, class C>
inline ostream& operator<<(ostream&out, const boost::tuple<A, B, C> &t);
template<class A, class Alloc>
inline ostream& operator<<(ostream& out, const list<A,Alloc>& ilist);
template<class A, class Comp, class Alloc>
inline ostream& operator<<(ostream& out, const set<A, Comp, Alloc>& iset);
template<class A, class Comp, class Alloc>
inline ostream& operator<<(ostream& out, const multiset<A,Comp,Alloc>& iset);
template<class A, class B, class Comp, class Alloc>
inline ostream& operator<<(ostream& out, const map<A,B,Comp,Alloc>& m);
template<class A, class B, class Comp, class Alloc>
inline ostream& operator<<(ostream& out, const multimap<A,B,Comp,Alloc>& m);

template<class A, class B>
inline ostream& operator<<(ostream& out, const pair<A,B>& v) {
  return out << v.first << "," << v.second;
}

template<class A, class Alloc>
inline ostream& operator<<(ostream& out, const vector<A,Alloc>& v) {
  out << "[";
  for (auto p = v.begin(); p != v.end(); ++p) {
    if (p != v.begin()) out << ",";
    out << *p;
  }
  out << "]";
  return out;
}
template<class A, class Alloc>
inline ostream& operator<<(ostream& out, const deque<A,Alloc>& v) {
  out << "<";
  for (auto p = v.begin(); p != v.end(); ++p) {
    if (p != v.begin()) out << ",";
    out << *p;
  }
  out << ">";
  return out;
}

template<class A, class B, class C>
inline ostream& operator<<(ostream&out, const boost::tuple<A, B, C> &t) {
  out << boost::get<0>(t) <<"," << boost::get<1>(t) << "," << boost::get<2>(t);
  return out;
}

template<class A, class Alloc>
inline ostream& operator<<(ostream& out, const list<A,Alloc>& ilist) {
  for (auto it = ilist.begin();
       it != ilist.end();
       ++it) {
    if (it != ilist.begin()) out << ",";
    out << *it;
  }
  return out;
}

template<class A, class Comp, class Alloc>
inline ostream& operator<<(ostream& out, const set<A, Comp, Alloc>& iset) {
  for (auto it = iset.begin();
       it != iset.end();
       ++it) {
    if (it != iset.begin()) out << ",";
    out << *it;
  }
  return out;
}

template<class A, class Comp, class Alloc>
inline ostream& operator<<(ostream& out, const multiset<A,Comp,Alloc>& iset) {
  for (auto it = iset.begin();
       it != iset.end();
       ++it) {
    if (it != iset.begin()) out << ",";
    out << *it;
  }
  return out;
}

template<class A, class B, class Comp, class Alloc>
inline ostream& operator<<(ostream& out, const map<A,B,Comp,Alloc>& m)
{
  out << "{";
  for (auto it = m.begin();
       it != m.end();
       ++it) {
    if (it != m.begin()) out << ",";
    out << it->first << "=" << it->second;
  }
  out << "}";
  return out;
}

template<class A, class B, class Comp, class Alloc>
inline ostream& operator<<(ostream& out, const multimap<A,B,Comp,Alloc>& m)
{
  out << "{{";
  for (auto it = m.begin();
       it != m.end();
       ++it) {
    if (it != m.begin()) out << ",";
    out << it->first << "=" << it->second;
  }
  out << "}}";
  return out;
}




/*
 * comparators for stl containers
 */
// for ceph::unordered_map:
//   ceph::unordered_map<const char*, long, hash<const char*>, eqstr> vals;
struct eqstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) == 0;
  }
};

// for set, map
struct ltstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};


namespace ceph {
  class Formatter;
}

#include "encoding.h"

WRITE_RAW_ENCODER(ceph_fsid)
WRITE_RAW_ENCODER(ceph_file_layout)
WRITE_RAW_ENCODER(ceph_dir_layout)
WRITE_RAW_ENCODER(ceph_mds_session_head)
WRITE_RAW_ENCODER(ceph_mds_request_head_legacy)
WRITE_RAW_ENCODER(ceph_mds_request_head)
WRITE_RAW_ENCODER(ceph_mds_request_release)
WRITE_RAW_ENCODER(ceph_filelock)
WRITE_RAW_ENCODER(ceph_mds_caps_head)
WRITE_RAW_ENCODER(ceph_mds_caps_body_legacy)
WRITE_RAW_ENCODER(ceph_mds_cap_peer)
WRITE_RAW_ENCODER(ceph_mds_cap_release)
WRITE_RAW_ENCODER(ceph_mds_cap_item)
WRITE_RAW_ENCODER(ceph_mds_lease)
WRITE_RAW_ENCODER(ceph_mds_snap_head)
WRITE_RAW_ENCODER(ceph_mds_snap_realm)
WRITE_RAW_ENCODER(ceph_mds_reply_head)
WRITE_RAW_ENCODER(ceph_mds_reply_cap)
WRITE_RAW_ENCODER(ceph_mds_cap_reconnect)
WRITE_RAW_ENCODER(ceph_mds_snaprealm_reconnect)
WRITE_RAW_ENCODER(ceph_frag_tree_split)
WRITE_RAW_ENCODER(ceph_osd_reply_head)
WRITE_RAW_ENCODER(ceph_osd_op)
WRITE_RAW_ENCODER(ceph_msg_header)
WRITE_RAW_ENCODER(ceph_msg_footer)
WRITE_RAW_ENCODER(ceph_msg_footer_old)
WRITE_RAW_ENCODER(ceph_mon_subscribe_item)

WRITE_RAW_ENCODER(ceph_mon_statfs)
WRITE_RAW_ENCODER(ceph_mon_statfs_reply)

// ----------------------
// some basic types

// NOTE: these must match ceph_fs.h typedefs
typedef uint64_t ceph_tid_t; // transaction id
typedef uint64_t version_t;
typedef __u32 epoch_t;       // map epoch  (32bits -> 13 epochs/second for 10 years)

// --------------------------------------
// identify individual mount clients by 64bit value

struct client_t {
  int64_t v;

  // cppcheck-suppress noExplicitConstructor
  client_t(int64_t _v = -2) : v(_v) {}
  
  void encode(bufferlist& bl) const {
    ::encode(v, bl);
  }
  void decode(bufferlist::iterator& bl) {
    ::decode(v, bl);
  }
};
WRITE_CLASS_ENCODER(client_t)

static inline bool operator==(const client_t& l, const client_t& r) { return l.v == r.v; }
static inline bool operator!=(const client_t& l, const client_t& r) { return l.v != r.v; }
static inline bool operator<(const client_t& l, const client_t& r) { return l.v < r.v; }
static inline bool operator<=(const client_t& l, const client_t& r) { return l.v <= r.v; }
static inline bool operator>(const client_t& l, const client_t& r) { return l.v > r.v; }
static inline bool operator>=(const client_t& l, const client_t& r) { return l.v >= r.v; }

static inline bool operator>=(const client_t& l, int64_t o) { return l.v >= o; }
static inline bool operator<(const client_t& l, int64_t o) { return l.v < o; }

inline ostream& operator<<(ostream& out, const client_t& c) {
  return out << c.v;
}



// --

namespace {
  inline ostream& format_u(ostream& out, const uint64_t v, const uint64_t n,
      const int index, const uint64_t mult, const char* u)
  {
    char buffer[32];

    if (index == 0) {
      (void) snprintf(buffer, sizeof(buffer), "%" PRId64 "%s", n, u);
    } else if ((v % mult) == 0) {
      // If this is an even multiple of the base, always display
      // without any decimal fraction.
      (void) snprintf(buffer, sizeof(buffer), "%" PRId64 "%s", n, u);
    } else {
      // We want to choose a precision that reflects the best choice
      // for fitting in 5 characters.  This can get rather tricky when
      // we have numbers that are very close to an order of magnitude.
      // For example, when displaying 10239 (which is really 9.999K),
      // we want only a single place of precision for 10.0K.  We could
      // develop some complex heuristics for this, but it's much
      // easier just to try each combination in turn.
      int i;
      for (i = 2; i >= 0; i--) {
        if (snprintf(buffer, sizeof(buffer), "%.*f%s", i,
          static_cast<double>(v) / mult, u) <= 7)
          break;
      }
    }

    return out << buffer;
  }
}

/*
 * Use this struct to pretty print values that should be formatted with a
 * decimal unit prefix (the classic SI units). No actual unit will be added.
 */
struct si_u_t {
  uint64_t v;
  explicit si_u_t(uint64_t _v) : v(_v) {};
};

inline ostream& operator<<(ostream& out, const si_u_t& b)
{
  uint64_t n = b.v;
  int index = 0;
  uint64_t mult = 1;
  const char* u[] = {"", "k", "M", "G", "T", "P", "E"};

  while (n >= 1000 && index < 7) {
    n /= 1000;
    index++;
    mult *= 1000;
  }

  return format_u(out, b.v, n, index, mult, u[index]);
}

/*
 * Use this struct to pretty print values that should be formatted with a
 * binary unit prefix (IEC units). Since binary unit prefixes are to be used for
 * "multiples of units in data processing, data transmission, and digital
 * information" (so bits and bytes) and so far bits are not printed, the unit
 * "B" for "byte" is added besides the multiplier.
 */
struct byte_u_t {
  uint64_t v;
  explicit byte_u_t(uint64_t _v) : v(_v) {};
};

inline ostream& operator<<(ostream& out, const byte_u_t& b)
{
  uint64_t n = b.v;
  int index = 0;
  const char* u[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB"};

  while (n >= 1024 && index < 7) {
    n /= 1024;
    index++;
  }

  return format_u(out, b.v, n, index, 1ULL << (10 * index), u[index]);
}

inline ostream& operator<<(ostream& out, const ceph_mon_subscribe_item& i)
{
  return out << i.start
	     << ((i.flags & CEPH_SUBSCRIBE_ONETIME) ? "" : "+");
}

struct weightf_t {
  float v;
  // cppcheck-suppress noExplicitConstructor
  weightf_t(float _v) : v(_v) {}
};

inline ostream& operator<<(ostream& out, const weightf_t& w)
{
  if (w.v < -0.01) {
    return out << "-";
  } else if (w.v < 0.000001) {
    return out << "0";
  } else {
    std::streamsize p = out.precision();
    return out << std::fixed << std::setprecision(5) << w.v << std::setprecision(p);
  }
}

struct shard_id_t {
  int8_t id;

  shard_id_t() : id(0) {}
  explicit shard_id_t(int8_t _id) : id(_id) {}

  operator int8_t() const { return id; }

  const static shard_id_t NO_SHARD;

  void encode(bufferlist &bl) const {
    ::encode(id, bl);
  }
  void decode(bufferlist::iterator &bl) {
    ::decode(id, bl);
  }
};
WRITE_CLASS_ENCODER(shard_id_t)
WRITE_EQ_OPERATORS_1(shard_id_t, id)
WRITE_CMP_OPERATORS_1(shard_id_t, id)
ostream &operator<<(ostream &lhs, const shard_id_t &rhs);

#if defined(__sun) || defined(_AIX) || defined(DARWIN) || defined(__FreeBSD__)
__s32  ceph_to_hostos_errno(__s32 e);
__s32  hostos_to_ceph_errno(__s32 e);
#else
#define  ceph_to_hostos_errno(e) (e)
#define  hostos_to_ceph_errno(e) (e)
#endif

struct errorcode32_t {
  int32_t code;

  errorcode32_t() : code(0) {}
  // cppcheck-suppress noExplicitConstructor
  errorcode32_t(int32_t i) : code(i) {}

  operator int() const  { return code; }
  int* operator&()      { return &code; }
  int operator==(int i) { return code == i; }
  int operator>(int i)  { return code > i; }
  int operator>=(int i) { return code >= i; }
  int operator<(int i)  { return code < i; }
  int operator<=(int i) { return code <= i; }

  void encode(bufferlist &bl) const {
    __s32 newcode = hostos_to_ceph_errno(code);
    ::encode(newcode, bl);
  }
  void decode(bufferlist::iterator &bl) {
    ::decode(code, bl);
    code = ceph_to_hostos_errno(code);
  }
};
WRITE_CLASS_ENCODER(errorcode32_t)
WRITE_EQ_OPERATORS_1(errorcode32_t, code)
WRITE_CMP_OPERATORS_1(errorcode32_t, code)


#endif
