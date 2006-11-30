//
// Heavily modified by Aleksey Cheusov <vle@gmx.net>
//

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

#include <google/sparsehash/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/stat.h>

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
#include "judy_map.h"
#include "judy_map_kdcell.h"
#include "hash_funcs.h"

// hash_map
#ifndef _STLP_BEGIN_NAMESPACE
   // not stlport
#  if __GNUC__ >= 3
#    undef HASH_NAMESPACE
#    define HASH_NAMESPACE __gnu_cxx
#    include <ext/hash_map>
#  else
#   ifdef _MSC_VER
#    undef HASH_NAMESPACE
#    define HASH_NAMESPACE stdext
#    include <hash_map>
#   else
#    undef HASH_NAMESPACE
#    define HASH_NAMESPACE std
#    include <ext/hash_map>
#   endif
#  endif
#else
   // stlport
#  undef HASH_NAMESPACE
#  define HASH_NAMESPACE std
#  include <hash_map>
#endif

using HASH_NAMESPACE::hash_map;
using GOOGLE_NAMESPACE::sparse_hash_map;
using GOOGLE_NAMESPACE::dense_hash_map;
using STL_NAMESPACE::map;

// map type for testing
enum {
	mt_sparse,
	mt_dense,
	mt_map,
	mt_hash,
	mt_judy_l,
	mt_judy_m,
	mt_judy_kdcell,
};
int map_type       = -1;

int slowness_level = -1;

unsigned hash_mask = JUDYARRAY_HASH_MASK;

enum {
	ht_identity = 0,
	ht_random   = -1,
};
int hash_func_type = 0;

// external function for slower hash function 
int slow_compare_int (int a, int b, int slowness);

template <class T>
struct hash_ident {
	size_t operator () (int a) const {
		return (size_t) a;
	}
};

struct slow_less {
	bool operator () (int a, int b) const {
		return slow_compare_int (a, b, slowness_level) < 0;
	};
};

struct slow_equal {
	bool operator () (int a, int b) const {
		return slow_compare_int (a, b, slowness_level) == 0;
	};
};

// Normally I don't like non-const references, but using them here ensures
// the inlined code ends up as efficient as possible.

template<class MapType> void SET_DELETED_KEY(MapType& map, int key) {}
template<class MapType> void SET_EMPTY_KEY(MapType& map, int key) {}
template<class MapType> void SET_HASH_MASK(MapType& map, unsigned mask) {}
template<class MapType> void RESIZE(MapType& map, int iters) {}
template<class MapType> void SHOW_DEBUG(const MapType& map) {}

#define REDEF_SET_DELETED_KEY(Type, Key, Data, Hash, Equal) \
    template<> void SET_DELETED_KEY(               \
        Type <Key, Data, Hash, Equal >& m, int key) \
	{                                              \
		m.set_deleted_key(key);                    \
	}

#define REDEF_SET_EMPTY_KEY(Type, Key, Data, Hash, Equal) \
    template<> void SET_EMPTY_KEY(                 \
        Type <Key, Data, Hash, Equal >& m, int key) \
	{                                              \
		m.set_empty_key(key);                      \
	}

#define REDEF_RESIZE(Type, Key, Data, Hash, Equal)   \
    template<> void RESIZE(                          \
        Type <Key, Data, Hash, Equal >& m, int iters) \
	{                                                \
		m.resize(iters);                             \
	}

#define REDEF_SET_HASH_MASK4(Type, Key, Data, Hash, Equal)\
    template<> void SET_HASH_MASK(                       \
        Type <Key, Data, Hash, Equal >& m, unsigned mask) \
	{                                                    \
		m.set_hash_mask(mask);                           \
	}

#define REDEF_SET_HASH_MASK5(Type, Key, Data, Hash, Less, Equal)\
    template<> void SET_HASH_MASK(                       \
        Type <Key, Data, Hash, Less, Equal >& m, unsigned mask) \
	{                                                    \
		m.set_hash_mask(mask);                           \
	}

template <class T>
static void show_debug_judyarray (const T& ht)
{
#ifdef JUDYARRAY_DEBUGINFO
	std::cout << "single item count: "
			  << ht.get_debug_info ().m_value_count << '\n';
	std::cout << "list item count:   "
			  << ht.get_debug_info ().m_list_item_count << '\n';
	std::cout << "list count:        "
			  << ht.get_debug_info ().m_list_count << '\n';
#endif
}

#define REDEF_SHOW_DEBUG5(Type, Key, Data, Hash, Less, Equal)\
    template<> void SHOW_DEBUG(                       \
        const Type <Key, Data, Hash, Less, Equal >& m) \
	{\
		show_debug_judyarray (m);\
	}

#define REDEF_SHOW_DEBUG4(Type, Key, Data, Hash, Equal)\
    template<> void SHOW_DEBUG(                       \
        const Type <Key, Data, Hash, Equal >& m) \
	{\
		show_debug_judyarray (m);\
	}

//
#define REDEF_GLOBAL(Hash, Less, Equal)\
  REDEF_SET_DELETED_KEY(sparse_hash_map, int, int, Hash, Equal)\
  \
  REDEF_SET_DELETED_KEY( dense_hash_map, int, int, Hash, Equal)\
  \
  REDEF_SET_EMPTY_KEY(dense_hash_map, int, int, Hash, Equal)\
  \
  REDEF_RESIZE(sparse_hash_map, int, int, Hash, Equal)      \
  REDEF_RESIZE( dense_hash_map, int, int, Hash, Equal)      \
  REDEF_RESIZE(       hash_map, int, int, Hash, Equal)      \
  \
  REDEF_SET_HASH_MASK4(judy_map_l, int, int, Hash, Equal)   \
  REDEF_SET_HASH_MASK5(judy_map_m, int, int, Hash, Less, Equal)\
  \
  REDEF_SHOW_DEBUG4(judy_map_l, int, int, Hash, Equal)   \
  REDEF_SHOW_DEBUG5(judy_map_m, int, int, Hash, Less, Equal)

//
REDEF_GLOBAL(hash_ident <int>,      std::less <int>, std::equal_to <int>)
REDEF_GLOBAL(hash_ident <int>,      slow_less,       slow_equal)
REDEF_GLOBAL(hashfunc_poly <65599>, std::less <int>, std::equal_to <int>)
REDEF_GLOBAL(hashfunc_poly <65599>, slow_less,       slow_equal)

/*
 * Measure resource usage.
 */

unsigned memory_used ();

class Rusage {
public:
	/* Start collecting usage */
	Rusage() { Reset(); }

	/* Reset collection */
	void Reset();
	void ResetMemory();
	void ResetTime();

	/* Show usage */
	double UserTime();

	/* Memory usage */
	int Memory ();

private:
#ifdef HAVE_SYS_RESOURCE_H
	struct rusage start;
#else
	time_t start_time_t;
#endif

	int start_memory;
};

void Rusage::Reset() {
	ResetMemory ();
	ResetTime ();
}

void Rusage::ResetMemory() {
	start_memory = memory_used ();
}

void Rusage::ResetTime() {
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

int Rusage::Memory () {
	return memory_used () - start_memory;
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

static void report(char const* title, double t, int iters, int memory)
{
	if (memory){
		printf("memory-%-14s %d b\n", title, memory);
	}

	printf("%-20s %8.02f ns\n",
		   title,
		   (t * 1000000000.0 / iters));
}

template<class MapType>
static void add_items_to_map (MapType& m, int iters)
{
	for (int i = 0; i < iters; i++) {
		m[i] = i+1;
	}
}

template<class MapType>
static void time_map_add_absent(int iters) {
	MapType set;
	Rusage t;

	SET_EMPTY_KEY(set, -2);
	SET_HASH_MASK(set, hash_mask);

	t.Reset();
	add_items_to_map(set, iters);
	double ut = t.UserTime();

	report("add-absent", ut, iters, t.Memory ());
	SHOW_DEBUG(set);
}

template<class MapType>
static void time_map_add_absent_predict (int iters) {
	MapType set;
	Rusage t;

	SET_EMPTY_KEY(set, -2);
	SET_HASH_MASK(set, hash_mask);

	t.ResetMemory ();
	RESIZE(set, iters);
	t.ResetTime ();
	add_items_to_map(set, iters);
	double ut = t.UserTime();

//	report("add_absent-predict", ut, iters, t.Memory ()); // internal pool
	report("add-absent-predict", ut, iters, 0);
}

template<class MapType>
static void time_map_add_present(int iters) {
	MapType set;
	Rusage t;
	int i;

	SET_EMPTY_KEY(set, -2);
	SET_HASH_MASK(set, hash_mask);

	add_items_to_map(set, iters);

	t.Reset();
	add_items_to_map(set, iters);
	double ut = t.UserTime();

	report("add-present", ut, iters, 0);
}

template<class MapType>
static void time_map_find_base(int iters, int offs, const char *msg) {
	MapType set;
	Rusage t;
	int r;
	int i;

	SET_EMPTY_KEY(set, -2);
	SET_HASH_MASK(set, hash_mask);

	add_items_to_map(set, iters);

	r = 1;
	t.Reset();

	const typename MapType::iterator e = set.end();
	for (i = 0; i < iters; i++) {
		r ^= (set.find(offs+i) != e);
	}
	double ut = t.UserTime();

	report(msg, ut, iters, 0);
}

template<class MapType>
static void time_map_find_present(int iters) {
	time_map_find_base <MapType> (iters, 0, "find-present");
}

template<class MapType>
static void time_map_find_absent(int iters) {
	time_map_find_base <MapType> (iters, iters, "find-absent");
}

template<class MapType>
static void time_map_remove_base (int iters, int offs, const char *msg) {
	MapType set;
	Rusage t;
	int i;

	SET_EMPTY_KEY(set, -2);
	SET_HASH_MASK(set, hash_mask);

	add_items_to_map(set, iters);

	t.Reset();
	SET_DELETED_KEY(set, -1);
	for (i = 0; i < iters; i++) {
		set.erase(offs+i);
	}
	double ut = t.UserTime();

	report(msg, ut, iters, t.Memory ());
}

template<class MapType>
static void time_map_remove_present(int iters) {
	time_map_remove_base <MapType> (iters, 0, "remove-present");
}

template<class MapType>
static void time_map_remove_absent(int iters) {
	time_map_remove_base <MapType> (iters, iters, "remove-absent");
}

static int iterate_accum = 0;
template<class MapType>
static void time_map_iterate(int iters) {
	MapType set;
	Rusage t;
	int i;

	SET_EMPTY_KEY(set, -2);
	SET_HASH_MASK(set, hash_mask);

	add_items_to_map(set, iters);

	typename MapType::const_iterator beg = set.begin ();
	typename MapType::const_iterator end = set.end ();

	t.Reset();
	for (; beg != end; ++beg) {
		iterate_accum ^= (*beg).first;
	}
	double ut = t.UserTime();

	report("iterate", ut, iters, 0);
}

template<class MapType>
static void measure_map(int iters) {
	time_map_add_absent           <MapType> (iters);
	time_map_add_absent_predict <MapType> (iters);
	time_map_add_present       <MapType> (iters);
	time_map_find_present     <MapType> (iters);
	// sparse_hash_map disabled because its find-absent operation is
	// extreeeemly slow
	if (mt_sparse != map_type){
		time_map_find_absent <MapType> (iters);
		time_map_remove_absent <MapType> (iters);
	}
	time_map_remove_present <MapType> (iters);
	time_map_iterate <MapType> (iters);
	printf ("\n");
}

template <class Less, class Equal, class Hash>
void measure_all_maps (int n)
{
	switch (map_type){
		case mt_sparse:
			printf("sparse_hash_map ( %d iterations , slowness level %d ):\n",
				   n, slowness_level);
			measure_map< sparse_hash_map<int, int, Hash, Equal> >(n);
			break;

		case mt_dense:
			printf("dense_hash_map ( %d iterations , slowness level %d ):\n",
				   n, slowness_level);
			measure_map< dense_hash_map<int, int, Hash, Equal> >(n);
			break;

		case mt_judy_l:
			printf("judy_map_l ( %d iterations , slowness level %d ):\n",
				   n, slowness_level);
			measure_map< judy_map_l<int, int, Hash, Equal> >(n);
			break;

		case mt_judy_m:
			printf("judy_map_m ( %d iterations , slowness level %d ):\n",
				   n, slowness_level);
			measure_map< judy_map_m<int, int, Hash, Less, Equal> >(n);
			break;

		case mt_judy_kdcell:
			printf("judy_map_kdcell ( %d iterations , slowness level %d ):\n",
				   n, slowness_level);
			measure_map< judy_map_kdcell<int, int> >(n);
			break;

		case mt_hash:
			printf("hash_map ( %d iterations , slowness level %d ):\n",
				   n, slowness_level);
			measure_map< hash_map<int, int, Hash, Equal> >(n);
			break;

		case mt_map:
			printf("map ( %d iterations , slowness level %d ):\n",
				   n, slowness_level);
			measure_map< map<int, int, Less> >(n);
			break;
	}
}

static void usage (void)
{
	fprintf (stderr, "\
usage: time_hash_map [OPTIONS]\n\
OPTIONS:\n\
  -h                     this help message\n\
  -n <num>               iteration count\n\
  -s <num>               slowness level\n\
  -a <hash_func_type>    'identity', 'random' or 65599\n\
  -t sparse_hash_map     google's sparse_hash_map<...>\n\
  -t dense_hash_map      google's dense_map<...>\n\
  -t map                 std::map<...>\n\
  -t hash_map            hash_map<...>\n\
  -t judy_map_l          judy_map_l<...>\n\
  -t judy_hash_m         judy_map_m<...>\n\
  -t judy_hash_kd        judy_map_kdcell<...>\n\
  -m <hex_value>         judy_map_[lm].set_hash_mask (hex_value)\n\
");
}

int main(int argc, char** argv)
{
	int iters = -1;
	int c = 0;

	while (c = getopt (argc, argv, "ht:n:s:a:m:"), c != EOF){
		switch (c) {
			case 'h':
				usage ();
				exit (0);
			case 't':
				if      (!strcmp ("sparse_hash_map", optarg))
					map_type = mt_sparse;
				else if (!strcmp ("dense_hash_map", optarg))
					map_type = mt_dense;
				else if (!strcmp ("map", optarg))
					map_type = mt_map;
				else if (!strcmp ("hash_map", optarg))
					map_type = mt_hash;
				else if (!strcmp ("judy_map_l", optarg))
					map_type = mt_judy_l;
				else if (!strcmp ("judy_map_m", optarg))
					map_type = mt_judy_m;
				else if (!strcmp ("judy_map_kdcell", optarg))
					map_type = mt_judy_kdcell;
				else{
					usage ();
					exit (1);
				}
				break;
			case 'n':
				iters = atoi (optarg);
				break;
			case 's':
				slowness_level = atoi (optarg);
				break;
			case 'm':
				hash_mask = strtoul (optarg, NULL, 16);
				break;
			case 'a':
				if (!strcmp (optarg, "identity"))
					hash_func_type = ht_identity;
				else if (!strcmp (optarg, "random"))
					hash_func_type = ht_random;
				else if (!strcmp (optarg, "65599"))
					hash_func_type = 65599;
				else{
					usage ();
					exit (1);
				}

				break;
			default:
				usage ();
				exit (1);
		}
	}

	if (map_type == -1){
		fprintf (stderr, "-t is mandatory option\n");
		exit (1);
	}

	if (slowness_level == -1){
		fprintf (stderr, "-s is mandatory option\n");
		exit (1);
	}

	if (iters == -1){
		fprintf (stderr, "-n is mandatory option\n");
		exit (1);
	}

//	stamp_run(iters);

#ifndef HAVE_SYS_RESOURCE_H
	printf("\n*** WARNING ***: sys/resources.h was not found, so all times\n"
		   "                 reported are wall-clock time, not user time\n");
#endif

	switch (hash_func_type){
		case ht_identity:
			if (!slowness_level){
				measure_all_maps <std::less <int>,
					std::equal_to <int>,
					hash_ident <int> > (iters);
			}else{
				measure_all_maps <slow_less, slow_equal,
					hash_ident <int> > (iters);
			}
			break;
//		case ht_random:
//			if (!slowness_level){
//				measure_all_maps <std::less <int>,
//					std::equal_to <int>,
//					hashfunc_random> (iters);
//			}else{
//				measure_all_maps <slow_less, slow_equal,
//					hashfunc_random> (iters);
//			}
//			break;
		case 65599:
			if (!slowness_level){
				measure_all_maps <std::less <int>,
					std::equal_to <int>,
					hashfunc_poly <65599> > (iters);
			}else{
				measure_all_maps <slow_less, slow_equal,
					hashfunc_poly <65599> > (iters);
			}
			break;
		default:
			abort ();
	}

	return 0;
}
