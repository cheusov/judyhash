#include <string>
#include <iostream>
#include <map>
#include <tr1/unordered_map>
#include <sparsehash/sparse_hash_map>
#include <sparsehash/dense_hash_map>

#include "judy_map.h"
#include "hashfuncs.h"
#include "memused.h"

struct hash_string_poly65k {
	unsigned operator () (const std::string &v) const {
		return hash_str_poly65k (v.c_str ());
	};
};

judy_map_l <std::string, int, hash_string_poly65k > map_judy_l;
judy_map_m <std::string, int, hash_string_poly65k > map_judy_m;
std::map <std::string, int> map_map;
google::sparse_hash_map <std::string, int, hash_string_poly65k > map_sparse;
google::dense_hash_map <std::string, int, hash_string_poly65k > map_dense;
std::tr1::unordered_map <std::string, int, hash_string_poly65k > map_hash;

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

int main ()
{
	unsigned mem = memused ();

	map_dense.set_empty_key ("\n");

//	do_main (map_hash);
//	do_main (map_map);
//	do_main (map_judy_l);
//	do_main (map_judy_m);
//	do_main (map_sparse);
	do_main (map_dense);

	std::cerr << memused () - mem << " bytes\n";
	return 0;
}
