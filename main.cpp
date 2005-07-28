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
			h = h * 33 + *key++;

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

#if 0
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
	my_hash ht;

	std::string line;
	int line_count = 100;
	while (std::getline (std::cin, line)){
//		std::pair <my_hash::key_type, my_hash::mapped_type> curr;
		my_hash::iterator curr
			= ht.insert (
				my_hash::value_type (
					strdup (line.c_str ()), line_count));

		std::cout << "key=`" << (*curr).first << "`\n";
		std::cout << "value=" << (*curr).second << '\n';
		std::cout << '\n';

		++line_count;
	}

	my_hash::iterator beg, end;
	beg = ht.begin ();
	end = ht.end ();

	for (; !(beg == end); ++beg){
		std::cout << "key2=`" << (*beg).first << "`\n";
		std::cout << "value2=" << (*beg).second << "\n";
	}
}
