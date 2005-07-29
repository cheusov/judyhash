#include <iostream>
#include <string>
#include <string.h>
#include <map>

//#define USE_JUDY_HASH
//#define USE_HASH_MAP
//#define EMPTY_LOOP

#include "judyhash.h"

#ifdef USE_HASH_MAP
#include <hash_map>
#endif

struct hsh_string_hash {
	unsigned long operator () (const char *key) const
	{
//		const int m = 7U;
//		const int m = 31U;
//		const int m = 33U;
//		const int m = 65599U;
		const int m = 2654435789U;

		unsigned h = 0;

		while (*key)
			h = h * m + *key++;

		return h ; 
	}
};

struct cmp_string_eq {
	bool operator () (const char *a, const char *b) const
	{
		return !strcmp (a, b);
	}
};

struct cmp_string_lt {
	bool operator () (const char *a, const char *b) const
	{
		return strcmp (a, b) < 0;
	}
};

#ifdef USE_JUDY_HASH
typedef judyhash_map <
	const char *, int, hsh_string_hash, cmp_string_eq
	> my_hash;
#else
#ifdef USE_HASH_MAP
typedef std::hash_map <
	const char *, int//, hsh_string_hash, cmp_string_eq
	> my_hash;
#else
typedef std::map <std::string, int> my_hash;
#endif
#endif

int main ()
{
	char line [2048];
	int line_count = 100;

	int dups = 0;

#ifdef USE_JUDY_HASH
	std::cout << "JudyHash!!!\n";
#else
#ifdef USE_HASH_MAP
	std::cout << "std::hash_map!!!\n";
#else
	std::cout << "std::map!!!\n";
#endif
#endif

	my_hash::value_type init_values [] = {
		my_hash::value_type ("record", 1000),
		my_hash::value_type ("access", 1000),
		my_hash::value_type ("the", 1000),
		my_hash::value_type ("27562356273562036503276502560265", 1000),
		my_hash::value_type ("layout", 1000)
	};

	my_hash ht2 (
		init_values,
		init_values + sizeof (init_values)/sizeof (init_values [0]));

//	my_hash ht = ht2;
	my_hash ht;
	ht.swap (ht2);

	while (fgets (line, sizeof (line), stdin)){
//		std::pair <my_hash::key_type, my_hash::mapped_type> curr;
		std::pair <my_hash::iterator, bool> curr
#ifndef EMPTY_LOOP
			= ht.insert (
				my_hash::value_type (
					strdup (line), line_count))
#endif
			;

		bool new_item = curr.second;
		if (!new_item){
			++dups;
		}

//		std::cout << "new=`" << curr.second << "`\n";
//		std::cout << "key=`" << (*curr.first).first << "`\n";
//		std::cout << "value=" << (*curr.first).second << '\n';
//		std::cout << '\n';

		++line_count;
	}

#ifdef USE_JUDY_HASH
	std::cout << "duplicates count: " << dups << '\n';
	std::cout << "map size: " << ht.size () << '\n';

	std::cout << "single item count:" << ht.m_debug_info.m_value_count << '\n';
	std::cout << "list item count:  " << ht.m_debug_info.m_list_item_count << '\n';
	std::cout << "list count:       " << ht.m_debug_info.m_list_count << '\n';
#endif

//	my_hash::iterator beg, end;
//	beg = ht.begin ();
//	end = ht.end ();

//	for (; !(beg == end); ++beg){
//		std::cout << "key2=`" << (*beg).first << "`\n";
//		std::cout << "value2=" << (*beg).second << "\n";
//	}
}
