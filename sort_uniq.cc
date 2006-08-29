#include <string>
#include <iostream>
#include <map>
#include <hash_map>
#include <google/sparse_hash_map>
#include <google/dense_hash_map>

#include "judy_map.h"
#include "hash_funcs.h"

judy_map_l <std::string, int, hashfunc_poly <65599> > map_judy_l;
judy_map_m <std::string, int, hashfunc_poly <65599> > map_judy_m;
std::map <std::string, int> map_map;
google::sparse_hash_map <std::string, int, hashfunc_poly <65599> > map_sparse;
google::dense_hash_map <std::string, int, hashfunc_poly <65599> > map_dense;
std::hash_map <std::string, int, hashfunc_poly <65599> > map_hash;

template <typename T>
static void add (T& m, const std::string &line)
{
	++m [line];
}

template <typename T>
static void print_all (T& m)
{
	typename T::const_iterator beg = m.begin ();
	typename T::const_iterator end = m.end ();
	for (; !(beg == end); ++beg){
		std::cout << (*beg).second << ' ' << (*beg).first << '\n';
	}
}

template <typename T>
static void do_main (T& m)
{
	std::string line;

	while (std::getline (std::cin, line)){
		add (m, line);
	}

	print_all (m);
}

unsigned memory_used ();

int main ()
{
	unsigned mem = memory_used ();

	map_dense.set_empty_key ("\n");

//	do_main (map_hash);
//	do_main (map_map);
//	do_main (map_judy_l);
//	do_main (map_judy_m);
//	do_main (map_sparse);
	do_main (map_dense);

	std::cerr << memory_used () - mem << " bytes\n";
	return 0;
}
