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
#include "judy_map_kdcell.h"

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
	> mapl_1;
typedef judy_map_l <
	const char *, int, hsh_string_hash2, cmp_string_eq
	> mapl_2;
typedef judy_map_l <
	const char *, int, hsh_string_hash3, cmp_string_eq,
	my_pool <mapl_1::value_type>
	> mapl_3;
typedef judy_map_l <
	const char *, int, hsh_string_hash1, cmp_string_eq,
	std::allocator <mapl_1::value_type>
	> mapl_4;
typedef judy_map_l <
	const char *, int, hsh_string_hash2, cmp_string_eq,
	boost::fast_pool_allocator <mapl_1::value_type>
	> mapl_5;
typedef judy_map_l <
	const char *, int, hsh_string_hash3, cmp_string_eq,
	boost::fast_pool_allocator <mapl_1::value_type>
	> mapl_6;

typedef std::map <
	std::string, int
	> std_map10;
typedef judy_map_l <
	std::string, int, hsh_string_hash1
	> mapl_11;
typedef judy_map_l <
	std::string, int, hsh_string_hash2
	> mapl_12;
typedef judy_map_l <
	std::string, int, hsh_string_hash3, cmp_string_eq,
	my_pool <mapl_11::value_type>
	> mapl_13;
typedef judy_map_l <
	std::string, int, hsh_string_hash1, cmp_string_eq,
	std::allocator <mapl_11::value_type>
	> mapl_14;
typedef judy_map_l <
	std::string, int, hsh_string_hash2, cmp_string_eq,
	boost::fast_pool_allocator <mapl_11::value_type>
	> mapl_15;
typedef judy_map_l <
	std::string, int, hsh_string_hash3, cmp_string_eq,
	std::allocator <mapl_11::value_type>
	> mapl_16;

typedef std::set <
	const char *, cmp_string_lt
	> std_set20;
typedef judy_set_l <
	const char *, hsh_string_hash1
	> setl_21;
typedef judy_set_l <
	const char *, hsh_string_hash2
	> setl_22;
typedef judy_set_l <
	const char *, hsh_string_hash3, cmp_string_eq,
	my_pool <setl_21::value_type>
	> setl_23;
typedef judy_set_l <
	const char *, hsh_string_hash1, cmp_string_eq,
	std::allocator <setl_21::value_type>
	> setl_24;
typedef judy_set_l <
	const char *, hsh_string_hash2, cmp_string_eq,
	boost::fast_pool_allocator <setl_21::value_type>
	> setl_25;
typedef judy_set_l <
	const char *, hsh_string_hash3, cmp_string_eq,
	std::allocator <setl_21::value_type>
	> setl_26;

typedef std::set <
	std::string, cmp_string_lt
	> setstd_30;
typedef judy_set_l <
	std::string, hsh_string_hash3
	> setl_31;
typedef judy_set_l <
	std::string, hsh_string_hash2
	> setl_32;
typedef judy_set_l <
	std::string, hsh_string_hash3, cmp_string_eq,
	my_pool <setl_31::value_type>
	> setl_33;
typedef judy_set_l <
	std::string, hsh_string_hash1, cmp_string_eq,
	std::allocator <setl_31::value_type>
	> setl_34;
typedef judy_set_l <
	std::string, hsh_string_hash2, cmp_string_eq,
	boost::fast_pool_allocator <setl_31::value_type>
	> setl_35;
typedef judy_set_l <
	std::string, hsh_string_hash3, cmp_string_eq,
	std::allocator <setl_31::value_type>
	> setl_36;

typedef std::set <
	const char *
	> setstd_40;
typedef judy_set_cell <
	const char *
	> setl_41;

typedef std::map <
	const char *, int
	> mapstd_50;
typedef judy_map_kdcell <
	const char *, int
	> mapl_51;

typedef judy_map_m <
	const char *, int, hsh_string_hash1, cmp_string_lt, cmp_string_eq
	> mapm_1;
typedef judy_map_m <
	const char *, int, hsh_string_hash2, cmp_string_lt, cmp_string_eq
	> mapm_2;
typedef judy_map_m <
	const char *, int, hsh_string_hash3, cmp_string_lt, cmp_string_eq,
	my_pool <mapl_1::value_type>
	> mapm_3;
typedef judy_map_m <
	const char *, int, hsh_string_hash1, cmp_string_lt, cmp_string_eq,
	std::allocator <mapl_1::value_type>
	> mapm_4;
typedef judy_map_m <
	const char *, int, hsh_string_hash2, cmp_string_lt, cmp_string_eq,
	boost::fast_pool_allocator <mapl_1::value_type>
	> mapm_5;
typedef judy_map_m <
	const char *, int, hsh_string_hash3, cmp_string_lt, cmp_string_eq,
	boost::fast_pool_allocator <mapl_1::value_type>
	> mapm_6;

typedef judy_map_m <
	std::string, int, hsh_string_hash1
	> mapm_11;
typedef judy_map_m <
	std::string, int, hsh_string_hash2
	> mapm_12;
typedef judy_map_m <
	std::string, int, hsh_string_hash3, cmp_string_lt, cmp_string_eq,
	my_pool <mapl_11::value_type>
	> mapm_13;
typedef judy_map_m <
	std::string, int, hsh_string_hash1, cmp_string_lt, cmp_string_eq,
	std::allocator <mapl_11::value_type>
	> mapm_14;
typedef judy_map_m <
	std::string, int, hsh_string_hash2, cmp_string_lt, cmp_string_eq,
	boost::fast_pool_allocator <mapl_11::value_type>
	> mapm_15;
typedef judy_map_m <
	std::string, int, hsh_string_hash3, cmp_string_lt, cmp_string_eq,
	std::allocator <mapl_11::value_type>
	> mapm_16;

//typedef std::multimap <
//	const char *, int, cmp_string_lt
//	> std_multimap40;

static const mapl_1::value_type init_values [] = {
	mapl_1::value_type ("record", 1000),
	mapl_1::value_type ("access", 1001),
	mapl_1::value_type ("the", 1002),
	mapl_1::value_type ("27562356273562036503276502560265", 1003),
	mapl_1::value_type ("layout", 1004),
	mapl_1::value_type ("apple", 1005),
	mapl_1::value_type ("layout", 99999999)
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

setl_31::value_type
value2key_ (const setl_31::value_type &v)
{
	return v;
}

setl_31::value_type
value2data_ (const setl_31::value_type &)
{
	return setl_31::value_type ("(true)");
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
	if (!sequences_are_equal (b1, e1, b2, e2)){
		std::cerr << "Please, send a bug report to vle@gmx.net.";
		exit (10);
	}
}

template <typename T>
void test_two_sets (std::set <int> &set1, T &set2)
{
	int probs_count = 1000000;
	int print_count = 400;
	int rand_value_count = 100000;

	for (int i=0; i < probs_count; ++i){
		long v = random () % rand_value_count;
		int threshold = int (rand_value_count * i / probs_count);

		if (random () > threshold){
			set1.insert (v);
			set2.insert (v);
		}else{
			set1.erase (v);
			set2.erase (v);
		}

		if (0 == (i % (probs_count / print_count))){
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

template <typename T>
void check_map (
	const std::map <int, int> &m1,
	const T& m2)
{
	std::map <int, int> m3 (m2.begin (), m2.end ());

	check_sequences (m1.begin (), m1.end (), m3.begin (), m3.end ());
}

template <typename T>
void test_two_maps (std::map <int, int> &map1, T &map2)
{
	int probs_count = 1000000;
	int print_count = 400;
	int rand_value_count = 100000;

	for (int i=0; i < probs_count; ++i){
		long v = random () % rand_value_count;
		int threshold = int (rand_value_count * i / probs_count);

		if (random () > threshold){
			map1.insert (std::make_pair (v, i));
			map2.insert (std::make_pair (v, i));
		}else{
			map1.erase (v);
			map2.erase (v);
		}

		if (0 == (i % (probs_count / print_count))){
//			std::cerr << "theshold = " << threshold * 100 / RAND_MAX << "%\n";
			check_map (map1, map2);
		}
	}
	check_map (map1, map2);
}

struct judy_test_hash_eq {
	int operator () (int i) const {
		return i;
	}
};

int main (int argc, const char **argv)
{
	--argc, ++argv;
	srandom (time (NULL));

	// test for constructor
	mapl_6 ht777 (
		init_values,
		init_values + sizeof (init_values)/sizeof (init_values [0]),
		0,
		mapl_6::hasher (),
		mapl_6::key_equal (),
		mapl_6::allocator_type ());

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
		mapl_1 ht_l1;
		test (ht_l1, 1);

//		mapm_1 ht_m1;
//		test (ht_m1, 1);
	}else if (!strcmp (argv [0], "2")){
		// test for constructor
		mapl_2 ht_l2 (0);
		test (ht_l2, 2);

//		mapm_2 ht_m2 (0);
//		test (ht_m2, 2);
	}else if (!strcmp (argv [0], "3")){
		// test for constructor
		mapl_3 ht_l3 (0, mapl_3::hasher ());
		test (ht_l3, 3);

//		mapm_3 ht_m3 (0, mapm_3::hasher ());
//		test (ht_m3, 3);
	}else if (!strcmp (argv [0], "4")){
		// test for constructor
		mapl_4 ht_l4 (0, mapl_4::hasher (), mapl_4::key_equal ());
		test (ht_l4, 4);

//		mapm_4 ht_m4 (0, mapm_4::hasher (), mapm_4::key_equal ());
//		test (ht_m4, 4);
	}else if (!strcmp (argv [0], "5")){
		// test for constructor
		mapl_5 ht_l5 (0, mapl_5::hasher (),
					  mapl_5::key_equal (), mapl_5::allocator_type ());
		test (ht_l5, 5);

//		mapm_5 ht_m5 (0, mapm_5::hasher (),
//					  mapm_5::key_equal (), mapm_5::allocator_type ());
//		test (ht_m5, 5);
	}else if (!strcmp (argv [0], "6")){
		// test for constructor
		mapl_6 ht_l6;
		test (ht_l6, 6);

//		mapm_6 ht_m6;
//		test (ht_m6, 6);


	}else if (!strcmp (argv [0], "10")){
		// test for constructor
		std_map10 ht_l10;
		test (ht_l10, 10);
	}else if (!strcmp (argv [0], "11")){
		// test for constructor
		mapl_11 ht_l11;
		test (ht_l11, 11);

//		mapm_11 ht_m11;
//		test (ht_m11, 11);
	}else if (!strcmp (argv [0], "12")){
		// test for constructor
		mapl_12 ht_l12;
		test (ht_l12, 12);

//		mapm_12 ht_m12;
//		test (ht_m12, 12);
	}else if (!strcmp (argv [0], "13")){
		// test for constructor
		mapl_13 ht_l13;
		test (ht_l13, 13);

//		mapm_13 ht_m13;
//		test (ht_m13, 13);
	}else if (!strcmp (argv [0], "14")){
		// test for constructor
		mapl_14 ht_l14;
		test (ht_l14, 14);

//		mapm_14 ht_m14;
//		test (ht_m14, 14);
	}else if (!strcmp (argv [0], "15")){
		// test for constructor
		mapl_15 ht_l15;
		test (ht_l15, 15);

//		mapm_15 ht_m15;
//		test (ht_m15, 15);
	}else if (!strcmp (argv [0], "16")){
		// test for constructor
		mapl_16 ht_l16;
		test (ht_l16, 16);

//		mapm_16 ht_m16;
//		test (ht_m16, 16);


//	}else if (!strcmp (argv [0], "40")){
//		// test for constructor
//		std_multimap40 ht_l40;
//		test (ht_l40, 40);



	}else if (!strcmp (argv [0], "20")){
		// test for constructor
		std_set20 ht_l20;
		test_set (ht_l20, 20);
	}else if (!strcmp (argv [0], "21")){
		// test for constructor
		setl_21  ht_l21;
		test_set (ht_l21, 21);
	}else if (!strcmp (argv [0], "22")){
		// test for constructor
		setl_22  ht_l22;
		test_set (ht_l22, 22);
	}else if (!strcmp (argv [0], "23")){
		// test for constructor
		setl_23  ht_l23;
		test_set (ht_l23, 23);
	}else if (!strcmp (argv [0], "24")){
		// test for constructor
		setl_24  ht_l24;
		test_set (ht_l24, 24);
	}else if (!strcmp (argv [0], "25")){
		// test for constructor
		setl_25  ht_l25;
		test_set (ht_l25, 25);
	}else if (!strcmp (argv [0], "26")){
		// test for constructor
		setl_26  ht_l26;
		test_set (ht_l26, 26);



	}else if (!strcmp (argv [0], "30")){
		// test for constructor
		setstd_30 ht_l30;
		test_set (ht_l30, 30);
	}else if (!strcmp (argv [0], "31")){
		// test for constructor
		setl_31 ht_l31;
		test_set (ht_l31, 31);
	}else if (!strcmp (argv [0], "32")){
		// test for constructor
		setl_32 ht_l32;
		test_set (ht_l32, 32);
	}else if (!strcmp (argv [0], "33")){
		// test for constructor
		setl_33 ht_l33;
		test_set (ht_l33, 33);
	}else if (!strcmp (argv [0], "34")){
		// test for constructor
		setl_34 ht_l34;
		test_set (ht_l34, 34);
	}else if (!strcmp (argv [0], "35")){
		// test for constructor
		setl_35 ht_l35;
		test_set (ht_l35, 35);
	}else if (!strcmp (argv [0], "36")){
		// test for constructor
		setl_36 ht_l36;
		test_set (ht_l36, 36);




	}else if (!strcmp (argv [0], "40")){
		// test for constructor
		setstd_40 ht_l40;
		test_set (ht_l40, 40);
	}else if (!strcmp (argv [0], "41")){
		// test for constructor
		setl_41 ht_l41;
		test_set (ht_l41, 41);




	}else if (!strcmp (argv [0], "50")){
		// test for constructor
		mapstd_50 ht_l50;
		test (ht_l50, 50);
	}else if (!strcmp (argv [0], "51")){
		// test for constructor
		mapl_51 ht_l51;
		test (ht_l51, 51);




	}else if (!strcmp (argv [0], "101")){
		std::set <int> std_set_int;
		judy_set_cell <int> set_cell_int;
		test_two_sets (std_set_int, set_cell_int);
		std::cout << "(std::set <int> == judy_set_cell <int>) = true\n";

	}else if (!strcmp (argv [0], "102")){
		std::set <int> std_set_int;
		judy_set_l <int, judy_test_hash_eq> set_l_int;
		test_two_sets (std_set_int, set_l_int);
		std::cout << "(std::set <int> == judy_set_l <int>) = true\n";

	}else if (!strcmp (argv [0], "103")){
		std::map <int, int> std_map_int;
		judy_map_kdcell <int, int> map_kdcell_int;
		test_two_maps (std_map_int, map_kdcell_int);
		std::cout << "(std::map <int> == judy_map_kdcell <int, int>) = true\n";

	}else if (!strcmp (argv [0], "104")){
		std::map <int, int> std_map_int;
		judy_map_l <int, int, judy_test_hash_eq> map_l_int;
		test_two_maps (std_map_int, map_l_int);
		std::cout << "(std::map <int> == judy_map_l <int>) = true\n";

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
