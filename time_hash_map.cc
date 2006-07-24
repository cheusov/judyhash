// Copyright (c) 2005, Google Inc.
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// ---
// Authors: Sanjay Ghemawat and Craig Silverstein
//
// Time various hash map implementations
//
// See PERFORMANCE for the output of one example run.

#include <google/sparsehash/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern "C" {
#include <time.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif
#ifdef HAVE_SYS_UTSNAME_H
#include <sys/utsname.h>      // for uname()
#endif
}

// The functions that we call on each map, that differ for different types.
// By default each is a noop, but we redefine them for types that need them.

#include <map>
#include <google/sparse_hash_map>
#include <google/dense_hash_map>
#include <judy_map.h>
#include <judy_map_kdcell.h>

// hash_map
#if __GNUC__ >= 3
#undef HASH_NAMESPACE
#define HASH_NAMESPACE __gnu_cxx
#include <ext/hash_map>
#else
#undef HASH_NAMESPACE
#define HASH_NAMESPACE std
#include <ext/hash_map>
#endif

using HASH_NAMESPACE::hash_map;
using GOOGLE_NAMESPACE::sparse_hash_map;
using GOOGLE_NAMESPACE::dense_hash_map;
using STL_NAMESPACE::map;

int func (int a);

struct hash2 {
	unsigned operator () (int n) const {
		return n;
	};
};

struct less2 {
	bool operator () (int a, int b) const {
		return func (a) < func (b);
	};
};

struct equal2 {
	bool operator () (int a, int b) const {
		return func (a) == func (b);
	};
};

// Normally I don't like non-const references, but using them here ensures
// the inlined code ends up as efficient as possible.

template<class MapType> inline void SET_DELETED_KEY(MapType& map, int key) {}
template<class MapType> inline void SET_EMPTY_KEY(MapType& map, int key) {}
template<class MapType> inline void RESIZE(MapType& map, int iters) {}

#define REDEF_SET_DELETED_KEY(Type, Key, Data, Hash, Equal) \
	template<> inline void SET_DELETED_KEY(        \
		Type <Key, Data, Hash, Equal>& m, int key) \
	{                                              \
		m.set_deleted_key(key);                    \
	}

#define REDEF_SET_EMPTY_KEY(Type, Key, Data, Hash, Equal) \
	template<> inline void SET_EMPTY_KEY(          \
		Type <Key, Data, Hash, Equal>& m, int key) \
	{                                              \
		m.set_empty_key(key);                      \
	}

#define REDEF_RESIZE(Type, Key, Data, Hash, Equal)   \
	template<> inline void RESIZE(                   \
		Type <Key, Data, Hash, Equal>& m, int iters) \
	{                                                \
		m.resize(iters);                             \
	}

#define REDEF_SET_DELETED_KEY2(Key, Data, Hash, Equal) \
  REDEF_SET_DELETED_KEY(sparse_hash_map, Key, Data, Hash, Equal) \
  REDEF_SET_DELETED_KEY(dense_hash_map,  Key, Data, Hash, Equal)

#define REDEF_SET_EMPTY_KEY2(Key, Data, Hash, Equal) \
  REDEF_SET_EMPTY_KEY(dense_hash_map, Key, Data, Hash, Equal)

#define REDEF_RESIZE2(Key, Data, Hash, Equal) \
  REDEF_RESIZE(sparse_hash_map, Key, Data, Hash, Equal) \
  REDEF_RESIZE(dense_hash_map,  Key, Data, Hash, Equal) \
  REDEF_RESIZE(hash_map,        Key, Data, Hash, Equal)

REDEF_SET_DELETED_KEY2(int, int, HASH_NAMESPACE::hash <int>, std::equal_to <int> )
REDEF_SET_EMPTY_KEY2(int, int, HASH_NAMESPACE::hash <int>, std::equal_to <int> )
REDEF_RESIZE2(int, int, HASH_NAMESPACE::hash <int>, std::equal_to <int> )

REDEF_SET_DELETED_KEY2(int, int, HASH_NAMESPACE::hash <int>, equal2 )
REDEF_SET_EMPTY_KEY2(int, int, HASH_NAMESPACE::hash <int>, equal2 )
REDEF_RESIZE2(int, int, HASH_NAMESPACE::hash <int>, equal2 )

static const int default_iters = 5000000;

/*
 * Measure resource usage.
 */

class Rusage {
 public:
  /* Start collecting usage */
  Rusage() { Reset(); }

  /* Reset collection */
  void Reset();
    
  /* Show usage */
  double UserTime();

 private:
#ifdef HAVE_SYS_RESOURCE_H
  struct rusage start;
#else
  time_t start_time_t;
#endif
};

inline void Rusage::Reset() {
#ifdef HAVE_SYS_RESOURCE_H
  getrusage(RUSAGE_SELF, &start);
#else
  time(&start_time_t);
#endif
}

inline double Rusage::UserTime() {
#ifdef HAVE_SYS_RESOURCE_H
  struct rusage u;
  
  getrusage(RUSAGE_SELF, &u);
  
  struct timeval result;
  result.tv_sec  = u.ru_utime.tv_sec  - start.ru_utime.tv_sec;
  result.tv_usec = u.ru_utime.tv_usec - start.ru_utime.tv_usec;
  
  return double(result.tv_sec) + double(result.tv_usec) / 1000000.0;
#else
  time_t now;
  time(&now);
  return now - start_time_t;
#endif
}


static void print_uname() {
#ifdef HAVE_SYS_UTSNAME_H
  struct utsname u;
  if (uname(&u) == 0) {
    printf("%s %s %s %s %s\n",
           u.sysname, u.nodename, u.release, u.version, u.machine);
  }
#endif
}

// Generate stamp for this run
static void stamp_run(int iters) {
  time_t now = time(0);
  printf("======\n");
  fflush(stdout);
  print_uname();
  printf("Average over %d iterations\n", iters);
  fflush(stdout);
  // don't need asctime_r/gmtime_r: we're not threaded
  printf("Current time (GMT): %s", asctime(gmtime(&now)));
}

static void report(char const* title, double t, int iters) {
  printf("%-20s %8.02f ns\n",
         title,
         (t * 1000000000.0 / iters));
}

template<class MapType>
static void time_map_grow(int iters) {
  MapType set;
  Rusage t;

  SET_EMPTY_KEY(set, -2);
  t.Reset();
  for (int i = 0; i < iters; i++) {
    set[i] = i+1;
  }
  double ut = t.UserTime();

  report("map_grow", ut, iters);
}

template<class MapType>
static void time_map_grow_predicted(int iters) {
  MapType set;
  Rusage t;

  SET_EMPTY_KEY(set, -2);
  RESIZE(set, iters);
  t.Reset();
  for (int i = 0; i < iters; i++) {
    set[i] = i+1;
  }
  double ut = t.UserTime();

  report("map_predict/grow", ut, iters);
}

template<class MapType>
static void time_map_replace(int iters) {
  MapType set;
  Rusage t;
  int i;

  SET_EMPTY_KEY(set, -2);
  for (i = 0; i < iters; i++) {
    set[i] = i+1;
  }

  t.Reset();
  for (i = 0; i < iters; i++) {
    set[i] = i+1;
  }
  double ut = t.UserTime();

  report("map_replace", ut, iters);
}

template<class MapType>
static void time_map_fetch_base(int iters, int offs, const char *msg) {
  MapType set;
  Rusage t;
  int r;
  int i;

  SET_EMPTY_KEY(set, -2);
  for (i = 0; i < iters; i++) {
    set[i] = i+1;
  }

  r = 1;
  t.Reset();
  for (i = 0; i < iters; i++) {
    r ^= (set.find(offs+i) != set.end());
  }
  double ut = t.UserTime();

  report(msg, ut, iters);
}

template<class MapType>
static void time_map_fetch_present(int iters) {
	time_map_fetch_base <MapType> (iters, 0, "map_fetch(present)");
}

template<class MapType>
static void time_map_fetch_absent(int iters) {
	time_map_fetch_base <MapType> (iters, iters, "map_fetch(absent)");
}

template<class MapType>
static void time_map_remove(int iters) {
  MapType set;
  Rusage t;
  int i;

  SET_EMPTY_KEY(set, -2);
  for (i = 0; i < iters; i++) {
    set[i] = i+1;
  }

  t.Reset();
  SET_DELETED_KEY(set, -1);
  for (i = 0; i < iters; i++) {
    set.erase(i);
  }
  double ut = t.UserTime();

  report("map_remove", ut, iters);
}

template<class MapType>
static void measure_map(int iters) {
  time_map_grow<MapType>(iters);
  time_map_grow_predicted<MapType>(iters);
  time_map_replace<MapType>(iters);
  time_map_fetch_present<MapType>(iters);
  time_map_fetch_absent<MapType>(iters);
  time_map_remove<MapType>(iters);
}

template <class Less, class Equal, class Hash>
void measure_all_maps (int n)
{
  printf("\nSPARSE_HASH_MAP:\n");
  measure_map< sparse_hash_map<int, int, Hash, Equal> >(n);

  printf("\nDENSE_HASH_MAP:\n");
  measure_map< dense_hash_map<int, int, Hash, Equal> >(n);

  printf("\nJUDY_MAP_L:\n");
  measure_map< judy_map_l<int, int, Hash, Equal> >(n);

  printf("\nJUDY_MAP_M:\n");
  measure_map< judy_map_m<int, int, Hash, Less, Equal> >(n);

  printf("\nJUDY_MAP_KDCELL:\n");
  measure_map< judy_map_kdcell<int, int> >(n);

  printf("\nSTANDARD HASH_MAP:\n");
  measure_map< hash_map<int, int, Hash, Equal> >(n);

  printf("\nSTANDARD MAP:\n");
  measure_map< map<int, int, Less> >(n);
}

int main(int argc, char** argv) {
  int iters = default_iters;
  if (argc > 1) {            // first arg is # of iterations
    iters = atoi(argv[1]);
  }

  stamp_run(iters);

#ifndef HAVE_SYS_RESOURCE_H
  printf("\n*** WARNING ***: sys/resources.h was not found, so all times\n"
         "                 reported are wall-clock time, not user time\n");
#endif

//  measure_all_maps <std::less <int>, std::equal_to <int>, std::less <int> > ();
  measure_all_maps <std::less <int>,
                    std::equal_to <int>,
                    HASH_NAMESPACE::hash <int> > (iters);
  measure_all_maps <less2,
                    equal2,
                    HASH_NAMESPACE::hash <int> > (iters);
//  measure_all_maps <less2, equal2, hash2 > (iters);

  return 0;
}
