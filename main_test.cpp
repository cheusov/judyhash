#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <assert.h>
//#include <utility>
#include <pool/pool_alloc.hpp>

#include "judyhash.h"

template <typename T>
class my_pool {
private:
	std::vector <T*> m_blocks;
	int m_it_size;
	int m_curr;
public:
	typedef const T* const_pointer;
	typedef const T& const_reference;
	typedef int difference_type;
	typedef T* pointer;
	typedef T& reference;
	typedef size_t size_type;

public:
	my_pool ()
	{
		m_it_size = 300000;
		m_curr    = 0;
	}

	~my_pool ()
	{
	}

	pointer allocate (size_t s)
	{
		assert (s == 1);

		if (m_curr == m_it_size || m_blocks.empty ()){
			m_blocks.push_back (new T [m_it_size]);
			m_curr = 0;
		}

		return m_blocks.back () + (m_curr++);
	}

	void deallocate (pointer ptr, size_type count)
	{
		assert (count == 1);
	}
};

typedef my_pool < std::pair <const char *const, int> > test_allocator_type1;
//typedef std::allocator < std::pair <const char *const, int> > test_allocator_type2;
typedef boost::fast_pool_allocator < std::pair <const char *const, int> > test_allocator_type2;

struct hsh_string_hash1 {
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

struct hsh_string_hash2 {
	size_t operator () (const char *key) const
	{
		return 0;
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

typedef judyhash_map <
	const char *, int, hsh_string_hash1, cmp_string_eq
	> my_hash1;
typedef judyhash_map <
	const char *, int, hsh_string_hash2, cmp_string_eq
	> my_hash2;
typedef judyhash_map <
	const char *, int, hsh_string_hash1, cmp_string_eq, test_allocator_type1
	> my_hash3;
typedef judyhash_map <
	const char *, int, hsh_string_hash2, cmp_string_eq, test_allocator_type1
	> my_hash4;
typedef judyhash_map <
	const char *, int, hsh_string_hash1, cmp_string_eq, test_allocator_type2
	> my_hash5;
typedef judyhash_map <
	const char *, int, hsh_string_hash2, cmp_string_eq, test_allocator_type2
	> my_hash6;

static const my_hash1::value_type init_values [] = {
	my_hash1::value_type ("record", 1000),
	my_hash1::value_type ("access", 1001),
	my_hash1::value_type ("the", 1002),
	my_hash1::value_type ("27562356273562036503276502560265", 1003),
	my_hash1::value_type ("layout", 1004),
	my_hash1::value_type ("apple", 1005),
	my_hash1::value_type ("layout", 99999999)
};

#define ITERATE_OVER(it_t, array, iter)                        \
    for (it_t iter = (array).begin (), iter##_end = (array).end (); \
         !(iter == iter##_end);                                \
         ++iter)

template <typename judyhash_type, typename judyhash_iterator_type>
void print_uni (judyhash_type &ht, int num)
{
	typedef std::pair <
		typename judyhash_type::key_type,
		typename judyhash_type::data_type> pair_type;
	typedef std::vector <pair_type> vec_type;

	vec_type vec;

	ITERATE_OVER (judyhash_iterator_type, ht, v){
		vec.push_back (*v);
	}

	std::sort (vec.begin (), vec.end ());

	ITERATE_OVER (typename vec_type::const_iterator, vec, v){
		std::cout << "key=`" << (*v).first << "` ";
		std::cout << "value=" << (*v).second << "\n";
	}
	std::cout << '\n';
}

template <typename judyhash_type>
void print_hash_it (judyhash_type &ht, int num)
{
	print_uni <judyhash_type, typename judyhash_type::iterator>
		(ht, num);
}

template <typename judyhash_type>
void print_hash_const_it (judyhash_type &ht, int num)
{
	print_uni <judyhash_type, typename judyhash_type::const_iterator>
		(ht, num);
}

template <typename judyhash_type>
void test (judyhash_type &ht, int num)
{
	typedef typename judyhash_type::iterator         hash_iterator;
	typedef typename judyhash_type::const_iterator   hash_const_iterator;
	typedef typename judyhash_type::value_type hash_value_type;

	// initializing 2
	judyhash_type ht2 (
		init_values,
		init_values + sizeof (init_values)/sizeof (init_values [0]));
	print_hash_it (ht2, num);

	// initializing 1
	ht.insert (hash_value_type ("apple", 7777));

	// swapping 1 and 2
	ht.swap (ht2);
	print_hash_it (ht2, num);
	print_hash_const_it (ht, num);

	// size and count
	std::cout << "map size: " << ht.size () << '\n';
	std::cout << "max_count=" << ht.max_size () << '\n';

	// finding "layout"
	hash_iterator layout_iterator = ht.find ("layout");
	hash_iterator layout_next_iterator;
	layout_next_iterator = layout_iterator;
	std::cout << " `" << (*layout_next_iterator).first << "` found\n";
	++layout_next_iterator;

	// finding "apple"
	hash_const_iterator apple_iterator = ht.find ("apple");
	hash_const_iterator apple_next_iterator = apple_iterator;
	std::cout << " `" << (*apple_next_iterator).first << "` found\n";

	// erase found "layout" by iterator
//	ht.erase (layout_iterator);
	ht.erase (layout_iterator, layout_next_iterator);
	apple_iterator  = layout_iterator;

	// erase "apple", "record", "the" and "language"
	ht.erase ("apple");
	ht.erase ("record");
	ht.erase ("the");
	ht.erase ("language");

	// finding predefined words
	for (int i=0; i < sizeof (init_values)/sizeof (init_values [0]); ++i){
		const char *key = init_values [i].first;
		hash_iterator found = ht.find (key);
		if (found == ht.end ())
			std::cout << "value[\"" << key << "\"]=(not found)\n";
		else
			std::cout << "value[\"" << key << "\"]=" << (*found).second << "\n";
	}

	print_hash_const_it (ht, num);
}

int main (int argc, const char **argv)
{
	--argc, ++argv;

	if (argc == 0){
		return 10;
	}if (!strcmp (argv [0], "1")){
		my_hash1 ht1;
		test (ht1, 1);
	}else if (!strcmp (argv [0], "2")){
		my_hash2 ht2;
		test (ht2, 2);
	}else if (!strcmp (argv [0], "3")){
		my_hash3 ht3;
		test (ht3, 3);
	}else if (!strcmp (argv [0], "4")){
		my_hash4 ht4;
		test (ht4, 4);
	}else if (!strcmp (argv [0], "5")){
		my_hash5 ht5;
		test (ht5, 5);
	}else if (!strcmp (argv [0], "6")){
		my_hash6 ht6;
		test (ht6, 6);
	}else{
		return 11;
	}
}
