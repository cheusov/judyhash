#include <string.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <vector>
#include <string>
//#include <utility>
#include <pool/pool_alloc.hpp>
#include <map>

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

struct hsh_string_hash3 {
	size_t operator () (const std::string &key) const
	{
		hsh_string_hash1 h;
		return h (key.c_str ());
	}
};

struct hsh_string_hash4 {
	size_t operator () (const std::string &key) const
	{
		return 1000;
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

typedef std::map <
	const char *, int, cmp_string_lt
	> std_map0;
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

typedef std::map <
	std::string, int
	> std_map100;
typedef judyhash_map <
	std::string, int, hsh_string_hash3
	> my_hash101;
typedef judyhash_map <
	std::string, int, hsh_string_hash4
	> my_hash102;

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
void print_uni (judyhash_type &ht, const char * name)
{
	typedef std::pair <
		typename judyhash_type::key_type,
		typename judyhash_type::mapped_type> pair_type;
	typedef std::vector <pair_type> vec_type;

	// test for forward_iterator?
	vec_type vec (ht.begin (), ht.end ());

	vec.clear ();

	// test for judyhash_map::const_iterator
	// test for judyhash_map::iterator
	ITERATE_OVER (judyhash_iterator_type, ht, v){
		vec.push_back (*v);
	}

	// actually, pointers are sorted here
	std::sort (vec.begin (), vec.end ());

	std::cout << name << ":\n";
	std::cout << "size:" << ht.size () << '\n';
	ITERATE_OVER (typename vec_type::const_iterator, vec, v){
		std::cout << "key=`" << (*v).first << "` ";
		std::cout << "value=" << (*v).second << "\n";
	}

	std::cout << '\n';
}

template <typename judyhash_type>
void print_hash_it (judyhash_type &ht, const char *name)
{
	print_uni <judyhash_type, typename judyhash_type::iterator>
		(ht, name);
}

template <typename judyhash_type>
void print_hash_const_it (judyhash_type &ht, const char *name)
{
	print_uni <judyhash_type, typename judyhash_type::const_iterator>
		(ht, name);
}

template <typename judyhash_type, typename judyhash_iterator_type>
void print_iterator (
	const char *array_name,
	const judyhash_type& ht,
	const judyhash_iterator_type& it)
{
	std::cout << array_name << "[" << (*it).first << "]";

	if (ht.end () == it){
		std::cout << " not found\n";
	}else{
		std::cout << "=" << (*it).second << '\n';
	}

	std::cout << '\n';
}

template <typename judyhash_type>
void test (judyhash_type &ht, int num)
{
	typedef typename judyhash_type::iterator         hash_iterator;
	typedef typename judyhash_type::const_iterator   hash_const_iterator;
	typedef typename judyhash_type::value_type hash_value_type;

	// initializing 2
	// test for constructor
	judyhash_type ht2 (
		init_values,
		init_values + sizeof (init_values)/sizeof (init_values [0]));
	print_hash_it (ht2, "ht2 initial");

	// test for operator =, erase, insert, operator []
	// test for begin () when container is empty
	judyhash_type ht4;
	ht4.insert (hash_value_type ("cool", 10000));
	ht4 ["cool2"] = 10001;
	print_hash_it (ht4, "h4 before ht4 = ht2");

	ht4.clear ();
	print_hash_it (ht4, "h4 is empty");

	ht4 = ht2;

	print_hash_it (ht2, "h2 after ht4(ht2)");
	print_hash_it (ht4, "h4 after ht4(ht2)");
	ht4.erase ("record");
	print_hash_it (ht2, "h2 after ht4.erase (\"record\")");
	print_hash_it (ht4, "h4 after ht4.erase (\"record\")");

	// test for copy-constructor
	judyhash_type ht3 (ht2);
	print_hash_it (ht2, "h2 after ht3(ht2)");
	print_hash_it (ht3, "h3 after ht3(ht2)");
	ht3.erase ("record");
	print_hash_it (ht2, "h2 after ht3.erase (\"record\")");
	print_hash_it (ht3, "h3 after ht3.erase (\"record\")");

	// test for insert (value_type)
	ht.insert (hash_value_type ("apple", 7777));
	print_hash_const_it (ht, "ht initial");

	// swapping 1 and 2
	ht.swap (ht2);
	print_hash_it (ht2, "ht2 after swap");
	print_hash_const_it (ht, "ht after swap");

	// tests for iterator, find, iterator::operator++(),
	//           iterator::operator*() etc.
	// finding "layout"
	hash_iterator layout_iterator = ht.find ("layout");
	hash_iterator layout_next_iterator;
	print_iterator ("ht", ht, layout_iterator);
	(*layout_iterator).second = 75;
	print_iterator ("ht_layout_to_75 ", ht, ht.find ("layout"));

	// test for iterator::operator ->
	layout_iterator -> second = 74;
	print_iterator ("ht_layout_to_74 ", ht, ht.find ("layout"));

	// test for operator []
	ht ["layout"] = 76;
	print_iterator ("ht [\"layout\"]=76 ", ht, ht.find ("layout"));

	layout_next_iterator = layout_iterator;
	++layout_next_iterator;

	// finding "apple"
	hash_const_iterator apple_iterator = ht.find ("apple");
	hash_const_iterator apple_next_iterator = apple_iterator;
	print_iterator ("ht", ht, apple_iterator);

	// test for (iterator to const_iterator)
	apple_iterator  = layout_iterator;

	// test for erase (x,y)
	// erase found "layout" by iterator
	ht.erase (layout_iterator, layout_next_iterator);
	print_hash_const_it (ht, "ht after erasing \"layout\"");

	// erase "apple", "record", "not a member" and "language"
	// tests for erase (x)
	ht.erase ("apple");
	print_hash_const_it (
		ht, "ht after erasing \"apple\"");

	ht.erase (ht.find ("record"));
	print_hash_const_it (
		ht, "ht after erasing \"record\"");

	ht.erase ("not a member");
	print_hash_const_it (
		ht, "ht after erasing \"not a member\"");

	ht.erase ("language");
	print_hash_const_it (
		ht, "ht after erasing \"language\"");

	// finding predefined words
	for (int i=0; i < sizeof (init_values)/sizeof (init_values [0]); ++i){
		const char *key = init_values [i].first;
		hash_iterator found = ht.find (key);
		if (found == ht.end ())
			std::cout << "value[\"" << key << "\"]=(not found)\n";
		else
			std::cout << "value[\"" << key << "\"]=" << (*found).second << "\n";
	}

	print_hash_const_it (ht, "ht2 final");
}

int main (int argc, const char **argv)
{
	--argc, ++argv;

	// test for constructor
	my_hash6 ht777 (
		init_values,
		init_values + sizeof (init_values)/sizeof (init_values [0]),
		0,
		my_hash6::hasher (),
		my_hash6::key_equal (),
		my_hash6::allocator_type ());

	//max_count
	std::cout << "max_count=" << ht777.max_size () << '\n';

	std::cout << "single item count:"
			  << ht777.m_debug_info.m_value_count << '\n';
	std::cout << "list item count:  "
			  << ht777.m_debug_info.m_list_item_count << '\n';
	std::cout << "list count:       "
			  << ht777.m_debug_info.m_list_count << '\n';

	print_hash_it (ht777, "ht777");

	if (argc == 0){
		return 10;
	}else if (!strcmp (argv [0], "0")){
		// test for constructor
		std_map0 map0;
		test (map0, 0);
	}else if (!strcmp (argv [0], "1")){
		// test for constructor
		my_hash1 ht1;
		test (ht1, 1);
	}else if (!strcmp (argv [0], "2")){
		// test for constructor
		my_hash2 ht2 (0);
		test (ht2, 2);
	}else if (!strcmp (argv [0], "3")){
		// test for constructor
		my_hash3 ht3 (0, my_hash3::hasher ());
		test (ht3, 3);
	}else if (!strcmp (argv [0], "4")){
		// test for constructor
		my_hash4 ht4 (0, my_hash4::hasher (), my_hash4::key_equal ());
		test (ht4, 4);
	}else if (!strcmp (argv [0], "5")){
		// test for constructor
		my_hash5 ht5 (0, my_hash5::hasher (),
					  my_hash5::key_equal (), my_hash5::allocator_type ());
		test (ht5, 5);
	}else if (!strcmp (argv [0], "6")){
		// test for constructor
		my_hash6 ht6;
		test (ht6, 6);
	}else if (!strcmp (argv [0], "100")){
		// test for constructor
		std_map100 ht100;
		test (ht100, 100);
	}else if (!strcmp (argv [0], "101")){
		// test for constructor
		my_hash101 ht101;
		test (ht101, 101);
	}else if (!strcmp (argv [0], "102")){
		// test for constructor
		my_hash102 ht102;
		test (ht102, 102);
	}else{
		return 11;
	}
}
