#include <iostream>
#include <string>
#include <string.h>
#include <map>
//#include <hash_map>

#include "judyhash.h"

struct hsh_string_hash {
	unsigned long operator () (const char *key) const
	{
		unsigned h = 0;

		while (*key)
			h = h * 7 + *key++;

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

#if 1
//typedef std::map <const char *, int, cmp_string_lt> my_hash;
typedef std::map <std::string, int> my_hash;
//typedef std::hash_map <
//	const char *, int, hsh_string_hash, hsh_string_eq
//	> my_hash;
#else
typedef judyhash_map <
	const char *, int, hsh_string_hash, cmp_string_eq
	> my_hash;
#endif

int main ()
{
	char line [2048];
	int line_count = 100;

	int dups = 0;

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
			= ht.insert (
				my_hash::value_type (
					strdup (line), line_count));

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
	std::cout << "map size: " << ht.size () << '\n';

//	my_hash::iterator beg, end;
//	beg = ht.begin ();
//	end = ht.end ();

//	for (; !(beg == end); ++beg){
//		std::cout << "key2=`" << (*beg).first << "`\n";
//		std::cout << "value2=" << (*beg).second << "\n";
//	}
}
