#include <string.h>
#include <assert.h>
#include <time.h>

#include <iostream>
#include <string>
#include <vector>
#include <string>
//#include <utility>
#include <pool/pool_alloc.hpp>
#include <map>
#include <set>
#include <sstream>

//#define JUDYARRAY_NO_CONST
#include "judy_map.h"
#include "judy_set.h"
#include "judy_set_cell.h"
#include "judy_map_kcell_dcell.h"

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
	size_t operator () (const std::string &key) const
	{
		return operator () (key.c_str ());
	}
};

struct hsh_string_hash2 {
	size_t operator () (const char *) const
	{
		return 0;
	}
	size_t operator () (const std::string &) const
	{
		return 0;
	}
};

struct hsh_string_hash3 {
	size_t operator () (const char *) const
	{
		return 1000;
	}
	size_t operator () (const std::string &) const
	{
		return 1000;
	}
};

struct cmp_string_eq {
	bool operator () (const char *a, const char *b) const
	{
		return !strcmp (a, b);
	}
	bool operator () (const std::string a, const std::string &b) const
	{
		return a == b;
	}
};

struct cmp_string_lt {
	bool operator () (const char *a, const char *b) const
	{
		return strcmp (a, b) < 0;
	}
	bool operator () (const std::string &a, const std::string &b) const
	{
		return a < b;
	}

	template <typename K, typename D>
	bool operator () (
		const std::pair <K, D> &a, const std::pair <K, D> &b)
	{
		return a.first < b.first;
	}
};

typedef std::map <
	const char *, int, cmp_string_lt
	> std_map0;
typedef judy_map_l <
	const char *, int, hsh_string_hash1, cmp_string_eq
	> my_map1;
typedef judy_map_l <
	const char *, int, hsh_string_hash2, cmp_string_eq
	> my_map2;
typedef judy_map_l <
	const char *, int, hsh_string_hash3, cmp_string_eq,
	my_pool <my_map1::value_type>
	> my_map3;
typedef judy_map_l <
	const char *, int, hsh_string_hash1, cmp_string_eq,
	std::allocator <my_map1::value_type>
	> my_map4;
typedef judy_map_l <
	const char *, int, hsh_string_hash2, cmp_string_eq,
	boost::fast_pool_allocator <my_map1::value_type>
	> my_map5;
typedef judy_map_l <
	const char *, int, hsh_string_hash3, cmp_string_eq,
	boost::fast_pool_allocator <my_map1::value_type>
	> my_map6;

typedef std::map <
	std::string, int
	> std_map10;
typedef judy_map_l <
	std::string, int, hsh_string_hash1
	> my_map11;
typedef judy_map_l <
	std::string, int, hsh_string_hash2
	> my_map12;
typedef judy_map_l <
	std::string, int, hsh_string_hash3, cmp_string_eq,
	my_pool <my_map11::value_type>
	> my_map13;
typedef judy_map_l <
	std::string, int, hsh_string_hash1, cmp_string_eq,
	std::allocator <my_map11::value_type>
	> my_map14;
typedef judy_map_l <
	std::string, int, hsh_string_hash2, cmp_string_eq,
	boost::fast_pool_allocator <my_map11::value_type>
	> my_map15;
typedef judy_map_l <
	std::string, int, hsh_string_hash3, cmp_string_eq,
	std::allocator <my_map11::value_type>
	> my_map16;

typedef std::set <
	const char *, cmp_string_lt
	> std_set20;
typedef judy_set_l <
	const char *, hsh_string_hash1
	> my_set21;
typedef judy_set_l <
	const char *, hsh_string_hash2
	> my_set22;
typedef judy_set_l <
	const char *, hsh_string_hash3, cmp_string_eq,
	my_pool <my_set21::value_type>
	> my_set23;
typedef judy_set_l <
	const char *, hsh_string_hash1, cmp_string_eq,
	std::allocator <my_set21::value_type>
	> my_set24;
typedef judy_set_l <
	const char *, hsh_string_hash2, cmp_string_eq,
	boost::fast_pool_allocator <my_set21::value_type>
	> my_set25;
typedef judy_set_l <
	const char *, hsh_string_hash3, cmp_string_eq,
	std::allocator <my_set21::value_type>
	> my_set26;

typedef std::set <
	std::string, cmp_string_lt
	> std_set30;
typedef judy_set_l <
	std::string, hsh_string_hash3
	> my_set31;
typedef judy_set_l <
	std::string, hsh_string_hash2
	> my_set32;
typedef judy_set_l <
	std::string, hsh_string_hash3, cmp_string_eq,
	my_pool <my_set31::value_type>
	> my_set33;
typedef judy_set_l <
	std::string, hsh_string_hash1, cmp_string_eq,
	std::allocator <my_set31::value_type>
	> my_set34;
typedef judy_set_l <
	std::string, hsh_string_hash2, cmp_string_eq,
	boost::fast_pool_allocator <my_set31::value_type>
	> my_set35;
typedef judy_set_l <
	std::string, hsh_string_hash3, cmp_string_eq,
	std::allocator <my_set31::value_type>
	> my_set36;

typedef std::set <
	const char *
	> std_set40;
typedef judy_set_cell <
	const char *
	> my_set41;

typedef std::map <
	const char *, int
	> std_map50;
typedef judy_map_kcell_dcell <
	const char *, int
	> my_map51;

//typedef std::multimap <
//	const char *, int, cmp_string_lt
//	> std_multimap40;

static const my_map1::value_type init_values [] = {
	my_map1::value_type ("record", 1000),
	my_map1::value_type ("access", 1001),
	my_map1::value_type ("the", 1002),
	my_map1::value_type ("27562356273562036503276502560265", 1003),
	my_map1::value_type ("layout", 1004),
	my_map1::value_type ("apple", 1005),
	my_map1::value_type ("layout", 99999999)
};

static char const *const init_values_set [] = {
	"record",
	"access",
	"the",
	"27562356273562036503276502560265",
	"layout",
	"apple",
	"layout"
};

template <typename T>
const typename T::first_type &
value2key_ (const T &v)
{
	return v.first;
}

template <typename T>
const typename T::second_type &
value2data_ (const T &v)
{
	return v.second;
}

my_set31::value_type
value2key_ (const my_set31::value_type &v)
{
	return v;
}

my_set31::value_type
value2data_ (const my_set31::value_type &)
{
	return my_set31::value_type ("(true)");
}

std::string value2key_ (const char *v)
{
	return v;
}

std::string value2data_ (const char *v)
{
	return "(true)";
}

#define ITERATE_OVER(it_t, array, iter)                        \
    for (it_t iter = (array).begin (), iter##_end = (array).end (); \
         !(iter == iter##_end);                                \
         ++iter)

template <typename judyhash_type, typename judyhash_iterator_type>
void print_uni (judyhash_type &ht, const char * name)
{
	typedef std::set <
		typename judyhash_type::value_type,
		cmp_string_lt> vec_type;

	// test for forward_iterator?
	vec_type vec (ht.begin (), ht.end ());

	std::cout << name << ":\n";
	std::cout << "size:" << ht.size () << '\n';
	std::cout << "empty:" << ht.empty () << '\n';
	ITERATE_OVER (typename vec_type::const_iterator, vec, v){
		std::cout << "key=`" << value2key_ (*v) << "` ";
		std::cout << "value=" << value2data_ (*v) << "\n";
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
	std::cout << array_name << "[" << value2key_ (*it) << "]";

	if (ht.end () == it){
		std::cout << " not found\n";
	}else{
		std::cout << "=" << value2data_ (*it) << '\n';
	}

	std::cout << '\n';
}

template <typename T>
void print_insert_ret (const T &i)
{
	if (i.second)
		std::cout << "new item `";
	else
		std::cout << "old item `";

	std::cout << (*i.first).first << "` was inserted";
	std::cout << " value is `" << (*i.first).second << "`\n";
}

template <typename T>
void print_insert_ret_set (const T &i)
{
	if (i.second)
		std::cout << "new item `";
	else
		std::cout << "old item `";

	std::cout << *(i.first) << "` was inserted\n";
}

template <typename judyhash_type>
void test (judyhash_type &ht, int num)
{
	typedef typename judyhash_type::iterator         hash_iterator;
	typedef typename judyhash_type::const_iterator   hash_const_iterator;
	typedef typename judyhash_type::value_type hash_value_type;
	typedef std::pair
		<
		typename judyhash_type::iterator, bool
		> hash_insert_ret_type;

	// initializing 2
	// test for constructor
	judyhash_type ht2 (
		init_values,
		init_values + sizeof (init_values)/sizeof (init_values [0]));
	print_hash_it (ht2, "ht2 initial");

	// test for operator =, erase, insert, operator []
	// test for begin () when container is empty
	judyhash_type ht4;
	print_insert_ret (ht4.insert (hash_value_type ("cool", 10000)));
	print_insert_ret (ht4.insert (hash_value_type ("cool", 10002)));
	print_insert_ret (ht4.insert (hash_value_type ("cool", 10003)));
	print_insert_ret (ht4.insert (hash_value_type ("cool", 10004)));

	ht4 ["cool2"] = 10001;
	print_hash_it (ht4, "h4 before ht4 = ht2");

	// test for count ()
	std::cout << "count(\"cool\")=" << ht.count ("cool") << '\n';

	//
	ht4.clear ();
	print_hash_it (ht4, "h4 is empty");

	ht4 = ht2;

	print_hash_it (ht2, "h2 after ht4(ht2)");
	print_hash_it (ht4, "h4 after ht4(ht2)");
	ht4.erase ("record");
	print_hash_it (ht2, "h2 after ht4.erase (\"record\")");
	print_hash_it (ht4, "h4 after ht4.erase (\"record\")");

	//  test operator = (x, x)
	ht4 = ht4;
	print_hash_it (ht4, "h4 after ht4 = ht4");

	// test for empty ()
	std::cout << "ht4 is empty: " << ht4.empty () << '\n';
	std::cout << "ht2 is empty: " << ht2.empty () << '\n';
	ht.clear ();
	std::cout << "ht4 is empty: " << ht4.empty () << '\n';

	// test for copy-constructor
	judyhash_type ht3 (ht2);
	print_hash_it (ht2, "h2 after ht3(ht2)");
	print_hash_it (ht3, "h3 after ht3(ht2)");
	ht3.erase ("record");
	print_hash_it (ht2, "h2 after ht3.erase (\"record\")");
	print_hash_it (ht3, "h3 after ht3.erase (\"record\")");

	// test for insert (value_type)
	print_insert_ret (ht4.insert (hash_value_type ("cool", 10005)));
	print_insert_ret (ht.insert (hash_value_type ("apple", 7777)));
	print_hash_const_it (ht, "ht initial");

	// swapping 1 and 2
	print_hash_it (ht2, "ht2 before swap");
	print_hash_const_it (ht, "ht before swap");
	ht.swap (ht2);
	print_hash_it (ht2, "ht2 after swap");
	print_hash_const_it (ht, "ht after swap");

	// tests for iterator, find, iterator::operator++(),
	//           iterator::operator*() etc.
	// finding "layout"
	hash_iterator layout_iterator = ht.find ("layout");
	hash_iterator layout_next_iterator;
	print_iterator ("ht", ht, layout_iterator);
//	(*layout_iterator).second = 75;
	print_iterator ("ht_layout_to_75 ", ht, ht.find ("layout"));

	// test for iterator::operator ->
//	layout_iterator -> second = 74;
	print_iterator ("ht_layout_to_74 ", ht, ht.find ("layout"));

	// test for operator []
	ht ["layout"] = 76;
	print_iterator ("ht [\"layout\"]=76 ", ht, ht.find ("layout"));

	// test for count ()
	std::cout << "count(\"layout\")=" << ht.count ("layout") << '\n';

	//
	layout_next_iterator = layout_iterator;
	// operator ++ (int)
	layout_next_iterator++;

	// test for iterators
	std::cout << "distance: "
			  << std::distance (layout_iterator, layout_next_iterator)
			  << '\n';

	// finding "apple", test for 'const_iterator find (k) const'
	hash_const_iterator apple_iterator
		= ((const judyhash_type *)(&ht)) -> find ("apple");

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
	for (size_t i=0; i < sizeof (init_values)/sizeof (init_values [0]); ++i){
		const char *key = init_values [i].first;
		hash_iterator found = ht.find (key);
		if (found == ht.end ())
			std::cout << "value[\"" << key << "\"]=(not found)\n";
		else
			std::cout << "value[\"" << key << "\"]=" << value2data_ (*found) << "\n";
	}

	print_hash_const_it (ht, "ht2 final");
}

template <typename judyhash_type>
void test_set (judyhash_type &ht, int num)
{
	typedef typename judyhash_type::iterator         hash_iterator;
	typedef typename judyhash_type::const_iterator   hash_const_iterator;
	typedef typename judyhash_type::value_type hash_value_type;
	typedef hash_iterator                      hash_insert_ret_type;

	// initializing 2
	// test for constructor
	judyhash_type ht2 (
		init_values_set,
		init_values_set
		+ sizeof (init_values_set)/sizeof (init_values_set [0]));
	print_hash_it (ht2, "ht2 initial");

	// test for operator =, erase, insert, operator []
	// test for begin () when container is empty
	judyhash_type ht4;
	print_insert_ret_set (ht4.insert (hash_value_type ("cool")));
	print_insert_ret_set (ht4.insert (hash_value_type ("cool")));
	print_insert_ret_set (ht4.insert (hash_value_type ("cool")));
	print_insert_ret_set (ht4.insert (hash_value_type ("cool")));

//	ht4 ["cool2"] = 10001;
//	print_hash_it (ht4, "h4 before ht4 = ht2");

	// test for count ()
	std::cout << "count(\"cool\")=" << ht.count ("cool") << '\n';

	//
	ht4.clear ();
	print_hash_it (ht4, "h4 is empty");

	ht4 = ht2;

	print_hash_it (ht2, "h2 after ht4(ht2)");
	print_hash_it (ht4, "h4 after ht4(ht2)");
	ht4.erase ("record");
	print_hash_it (ht2, "h2 after ht4.erase (\"record\")");
	print_hash_it (ht4, "h4 after ht4.erase (\"record\")");

	//  test operator = (x, x)
	ht4 = ht4;
	print_hash_it (ht4, "h4 after ht4 = ht4");

	// test for empty ()
	std::cout << "ht4 is empty: " << ht4.empty () << '\n';
	std::cout << "ht2 is empty: " << ht2.empty () << '\n';
	ht.clear ();
	std::cout << "ht4 is empty: " << ht4.empty () << '\n';

	// test for copy-constructor
	judyhash_type ht3 (ht2);
	print_hash_it (ht2, "h2 after ht3(ht2)");
	print_hash_it (ht3, "h3 after ht3(ht2)");
	ht3.erase ("record");
	print_hash_it (ht2, "h2 after ht3.erase (\"record\")");
	print_hash_it (ht3, "h3 after ht3.erase (\"record\")");

	// test for insert (value_type)
	print_insert_ret_set (ht4.insert (hash_value_type ("cool")));
	print_insert_ret_set (ht.insert (hash_value_type ("apple")));
	print_hash_const_it (ht, "ht initial");

	// swapping 1 and 2
	print_hash_it (ht2, "ht2 before swap");
	print_hash_const_it (ht, "ht before swap");
	ht.swap (ht2);
	print_hash_it (ht2, "ht2 after swap");
	print_hash_const_it (ht, "ht after swap");

	// tests for iterator, find, iterator::operator++(),
	//           iterator::operator*() etc.
	// finding "layout"
	print_hash_const_it (ht, "ht before finding \"layout\"");
	hash_iterator layout_iterator;
	layout_iterator = ht.find ("layout");
	hash_iterator layout_next_iterator;
	print_iterator ("ht", ht, layout_iterator);
//	(*layout_iterator).second = 75;
	print_iterator ("ht_layout_to_75 ", ht, ht.find ("layout"));

	// test for iterator::operator ->
//	layout_iterator -> second = 74;
	print_iterator ("ht_layout_to_74 ", ht, ht.find ("layout"));

	// test for operator []
//	ht ["layout"] = 76;
//	print_iterator ("ht [\"layout\"]=76 ", ht, ht.find ("layout"));

	// test for count ()
	std::cout << "count(\"layout\")=" << ht.count ("layout") << '\n';

	//
	layout_next_iterator = layout_iterator;
	// operator ++ (int)
	layout_next_iterator++;

	// test for iterators
	std::cout << "distance: "
			  << std::distance (layout_iterator, layout_next_iterator)
			  << '\n';

	// finding "apple", test for 'const_iterator find (k) const'
	hash_const_iterator apple_iterator
		= ((const judyhash_type *)(&ht)) -> find ("apple");

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
	for (size_t i=0;
		 i < sizeof (init_values_set)/sizeof (init_values_set [0]);
		 ++i)
	{
		const char *key = init_values [i].first;
		hash_iterator found = ht.find (key);
		if (found == ht.end ())
			std::cout << "value[\"" << key << "\"]=(not found)\n";
		else
			std::cout << "value[\"" << key << "\"]=" << value2data_ (*found) << "\n";
	}

	print_hash_const_it (ht, "ht2 final");
}

#include <ext/hash_set>

template <typename set_type>
void test_set_int (set_type& obj, size_t probs)
{
	time_t time_curr;

	time_t time_start;
	time (&time_start);


	for (int i=0; i < probs; ++i){
		obj.insert (i);
	}

	time (&time_curr);
	std::cout << time_curr - time_start << " seconds\n";
}

template <typename T1, typename T2>
bool sequences_are_equal (T1 b1, T1 e1, T2 b2, T2 e2)
{
	while (b1 != e1 && b2 != e2){
		if (*b1 != *b2)
			return false;

		++b1;
		++b2;
	}

	return (b1 == e1) && (b2 == e2);
}

template <typename T1, typename T2>
void check_sequences (T1 b1, T1 e1, T2 b2, T2 e2)
{
//	std::cerr << "size ()=" << std::distance (b2, e2) << '\n';

	if (!sequences_are_equal (b1, e1, b2, e2)){
		std::cout << "Oooooh :-( Please send a bug report.";
		exit (10);
	}
}

template <typename T1, typename T2>
void test_two_sets (T1 &set1, T2 &set2)
{
	srandom (time (NULL));

	int probs_count = 100000;

	for (int i=0; i < probs_count; ++i){
		long v = random ();
		int threshold = int (((double) RAND_MAX) * i / probs_count);

		if (random () > threshold){
			set1.insert (v);
			set2.insert (v);
		}else{
			set1.erase (v);
			set2.erase (v);
		}

		if (0 == (i % (probs_count / 100))){
//			std::cerr << "theshold = " << threshold * 100 / RAND_MAX << "%\n";
			check_sequences (
				set1.begin (), set1.end (),
				set2.begin (), set2.end ());
		}
	}
	check_sequences (
		set1.begin (), set1.end (),
		set2.begin (), set2.end ());
}

struct judy_test_hash_eq {
	int operator () (int i) const {
		return i;
	}
};

int main (int argc, const char **argv)
{
	--argc, ++argv;

	// test for constructor
	my_map6 ht777 (
		init_values,
		init_values + sizeof (init_values)/sizeof (init_values [0]),
		0,
		my_map6::hasher (),
		my_map6::key_equal (),
		my_map6::allocator_type ());

	//max_count
	std::cout << "max_count=" << ht777.max_size () << '\n';

	std::cout << "single item count:"
			  << ht777.get_debug_info ().m_value_count << '\n';
	std::cout << "list item count:  "
			  << ht777.get_debug_info ().m_list_item_count << '\n';
	std::cout << "list count:       "
			  << ht777.get_debug_info ().m_list_count << '\n';

	print_hash_it (ht777, "ht777");

	if (argc == 0){
		return 10;
	}else if (!strcmp (argv [0], "0")){
		// test for constructor
		std_map0 map0;
		test (map0, 0);
	}else if (!strcmp (argv [0], "1")){
		// test for constructor
		my_map1 ht1;
		test (ht1, 1);
	}else if (!strcmp (argv [0], "2")){
		// test for constructor
		my_map2 ht2 (0);
		test (ht2, 2);
	}else if (!strcmp (argv [0], "3")){
		// test for constructor
		my_map3 ht3 (0, my_map3::hasher ());
		test (ht3, 3);
	}else if (!strcmp (argv [0], "4")){
		// test for constructor
		my_map4 ht4 (0, my_map4::hasher (), my_map4::key_equal ());
		test (ht4, 4);
	}else if (!strcmp (argv [0], "5")){
		// test for constructor
		my_map5 ht5 (0, my_map5::hasher (),
					  my_map5::key_equal (), my_map5::allocator_type ());
		test (ht5, 5);
	}else if (!strcmp (argv [0], "6")){
		// test for constructor
		my_map6 ht6;
		test (ht6, 6);



	}else if (!strcmp (argv [0], "10")){
		// test for constructor
		std_map10 ht10;
		test (ht10, 10);
	}else if (!strcmp (argv [0], "11")){
		// test for constructor
		my_map11 ht11;
		test (ht11, 11);
	}else if (!strcmp (argv [0], "12")){
		// test for constructor
		my_map12 ht12;
		test (ht12, 12);
	}else if (!strcmp (argv [0], "13")){
		// test for constructor
		my_map13 ht13;
		test (ht13, 13);
	}else if (!strcmp (argv [0], "14")){
		// test for constructor
		my_map14 ht14;
		test (ht14, 14);
	}else if (!strcmp (argv [0], "15")){
		// test for constructor
		my_map15 ht15;
		test (ht15, 15);
	}else if (!strcmp (argv [0], "16")){
		// test for constructor
		my_map16 ht16;
		test (ht16, 16);



//	}else if (!strcmp (argv [0], "40")){
//		// test for constructor
//		std_multimap40 ht40;
//		test (ht40, 40);



	}else if (!strcmp (argv [0], "20")){
		// test for constructor
		std_set20 ht20;
		test_set (ht20, 20);
	}else if (!strcmp (argv [0], "21")){
		// test for constructor
		my_set21  ht21;
		test_set (ht21, 21);
	}else if (!strcmp (argv [0], "22")){
		// test for constructor
		my_set22  ht22;
		test_set (ht22, 22);
	}else if (!strcmp (argv [0], "23")){
		// test for constructor
		my_set23  ht23;
		test_set (ht23, 23);
	}else if (!strcmp (argv [0], "24")){
		// test for constructor
		my_set24  ht24;
		test_set (ht24, 24);
	}else if (!strcmp (argv [0], "25")){
		// test for constructor
		my_set25  ht25;
		test_set (ht25, 25);
	}else if (!strcmp (argv [0], "26")){
		// test for constructor
		my_set26  ht26;
		test_set (ht26, 26);



	}else if (!strcmp (argv [0], "30")){
		// test for constructor
		std_set30 ht30;
		test_set (ht30, 30);
	}else if (!strcmp (argv [0], "31")){
		// test for constructor
		my_set31 ht31;
		test_set (ht31, 31);
	}else if (!strcmp (argv [0], "32")){
		// test for constructor
		my_set32 ht32;
		test_set (ht32, 32);
	}else if (!strcmp (argv [0], "33")){
		// test for constructor
		my_set33 ht33;
		test_set (ht33, 33);
	}else if (!strcmp (argv [0], "34")){
		// test for constructor
		my_set34 ht34;
		test_set (ht34, 34);
	}else if (!strcmp (argv [0], "35")){
		// test for constructor
		my_set35 ht35;
		test_set (ht35, 35);
	}else if (!strcmp (argv [0], "36")){
		// test for constructor
		my_set36 ht36;
		test_set (ht36, 36);




	}else if (!strcmp (argv [0], "40")){
		// test for constructor
		std_set40 ht40;
		test_set (ht40, 40);
	}else if (!strcmp (argv [0], "41")){
		// test for constructor
		my_set41 ht41;
		test_set (ht41, 41);




	}else if (!strcmp (argv [0], "50")){
		// test for constructor
		std_map50 ht50;
		test (ht50, 50);
	}else if (!strcmp (argv [0], "51")){
		// test for constructor
		my_map51 ht51;
		test (ht51, 51);




	}else if (!strcmp (argv [0], "100")){
		{
			std::set <int> std_set_int;
			judy_set_cell <int> set_cell_int;
			test_two_sets (std_set_int, set_cell_int);
			std::cout << "(std::set <int> == judy_set_cell <int>) = true\n";
		}

		{
			std::set <int> std_set_int;
			judy_set_l <int, judy_test_hash_eq> set_l_int;
			test_two_sets (std_set_int, set_l_int);
			std::cout << "(std::set <int> == judy_set_l <int>) = true\n";
		}

	}else if (!strcmp (argv [0], "150")){
		if (!argv [1])
			return 55;

		unsigned long probs = strtoul (argv [1], NULL, 10);

		// test for constructor
		std::cout << "judy_set_cell <int>: ";
		judy_set_cell <int> set_judy;
		test_set_int (set_judy, probs);
		set_judy.clear ();

//		std::cout << "std::set <int>: ";
//		std::set <int> set_std;
//		test_set_int (set_std, probs);

		std::cout << "hash <int>: ";
		__gnu_cxx::hash_set <int> set_hash;
		test_set_int (set_hash, probs);
	}else{
		return 11;
	}
}
