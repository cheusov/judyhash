#include <iostream>
#include <string>
#include <string.h>
#include <map>

#define USE_JUDY_HASH
//#define USE_HASH_MAP
//#define EMPTY_LOOP

#include "judyhash.h"

#ifdef USE_HASH_MAP
#include <hash_map>
#endif

struct hsh_string_hash {
	size_t operator () (const char *key) const
	{
//		const int m = 7U;
//		const int m = 31U;
//		const int m = 33U;
//		const int m = 65599U;
		const size_t m = 2654435789U;

		unsigned h = 0;

		if (*key)
			h = (unsigned char) *key++;
		else
			return h;

		if (*key)
			h = h + ((unsigned char) *key++ << 8);
		else
			return h;

		if (*key)
			h = h + ((unsigned char) *key++ << 16);
		else
			return h;

		if (*key)
			h = h + ((unsigned char) *key++ << 24);
		else
			return h;

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

class dinkumware_hash_traits
:
public hsh_string_hash,
public cmp_string_eq,
public cmp_string_lt
{
public:
	static const size_t bucket_size = 4;
	static const size_t min_buckets = 1024;

	dinkumware_hash_traits ()
	{
	}

	size_t operator () (const char * key) const
	{
		return hsh_string_hash::operator () (key);
	}

	bool operator () (const char * key1, const char *key2) const
	{
		return cmp_string_lt::operator () (key1, key2);
	}
};

#ifdef USE_JUDY_HASH
typedef judyhash_map <
	const char *, int, hsh_string_hash, cmp_string_eq
	> my_hash;
#else
#ifdef USE_HASH_MAP
#ifdef __ICC
typedef std::hash_map <
	const char *, int, dinkumware_hash_traits
	> my_hash;
#else
typedef std::hash_map <
	const char *, int, hsh_string_hash, cmp_string_eq
	> my_hash;
#endif
#else
typedef std::map <const char *, int, cmp_string_lt> my_hash;
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
		my_hash::value_type ("access", 1001),
		my_hash::value_type ("the", 1002),
		my_hash::value_type ("27562356273562036503276502560265", 1003),
		my_hash::value_type ("layout", 1004),
		my_hash::value_type ("layout", 99999999)
	};

	my_hash ht2 (
		init_values,
		init_values + sizeof (init_values)/sizeof (init_values [0]));

#if defined(USE_JUDY_HASH)
	my_hash ht (0, hsh_string_hash (), cmp_string_eq ());
#else
#if defined(USE_HASH_MAP) && !defined(__ICC)
	my_hash ht (0, hsh_string_hash (), cmp_string_eq ());
#else
	my_hash ht;
#endif
#endif

	ht.swap (ht2);

	while (fgets (line, sizeof (line), stdin)){
//		std::pair <my_hash::key_type, my_hash::mapped_type> curr;
		std::pair <my_hash::iterator, bool> curr
#ifndef EMPTY_LOOP
			= ht.insert (
				my_hash::value_type (
					strdup (line), line_count));
#else
		;
		strdup (line);
#endif

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

	std::cout << "duplicates count: " << dups << '\n';

#ifdef USE_JUDY_HASH
	std::cout << "single item count:" << ht.m_debug_info.m_value_count << '\n';
	std::cout << "list item count:  " << ht.m_debug_info.m_list_item_count << '\n';
	std::cout << "list count:       " << ht.m_debug_info.m_list_count << '\n';

	ht.key_eq ();
	ht.hash_funct ();
#endif

	std::cout << "map size: " << ht.size () << '\n';
#if defined(USE_JUDY_HASH) || defined(USE_HASH_MAP)
	std::cout << "max_count=" << ht.max_size () << '\n';
#endif

	my_hash::iterator layout_iterator = ht.find ("layout");
	my_hash::iterator layout_nest_iterator = layout_iterator;
	++layout_nest_iterator;

	layout_nest_iterator = ht.erase (layout_iterator, layout_nest_iterator);
	std::cout << "key after removed \"layout\"=" << (*layout_nest_iterator).first << '\n';
	std::cout << "value after removed \"layout\"=" << (*layout_nest_iterator).second << '\n';

	ht.erase ("apple");
	ht.erase ("record");
	ht.erase ("the");
	for (int i=0; i < sizeof (init_values)/sizeof (init_values [0]); ++i){
		const char *key = init_values [i].first;
		my_hash::iterator found = ht.find (key);
		if (found == ht.end ())
			std::cout << "value[\"" << key << "\"]=(not found)\n";
		else
			std::cout << "value[\"" << key << "\"]=" << (*ht.find (key)).second << "\n";

//		std::cout << "count[\"" << key << "\"]=" << ht.count (key) << "\n";
	}

//	while (1);
	my_hash::iterator beg, end;
	beg = ht.begin ();
	end = ht.end ();

//	for (; !(beg == end); ++beg){
//		std::cout << "key2=`" << (*beg).first << "`\n";
//		std::cout << "value2=" << (*beg).second << "\n";
//	}
}
