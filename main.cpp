//#include <sys/time.h>
//#include <time.h>
#include <string.h>
#include <assert.h>

#include <iostream>
#include <string>
#include <vector>
#include <pool/pool_alloc.hpp>

#if !defined(USE_GOOGLE_DENSE_MAP) && !defined(USE_STD_MAP) && !defined(USE_JUDY_HASH) && !defined(USE_HASH_MAP) && !defined(EMPTY_LOOP)
#error Opps
#endif

#ifdef EMPTY_LOOP
#define USE_STD_MAP
#endif

#include "judy_map.h"

#ifdef USE_GOOGLE_DENSE_MAP
#include <google/dense_hash_map>
#endif

#include <map>

#if defined(__GNUC__) && (__GNUC__ >= 3) && !defined(_STLP_CONFIG_H)
#include <ext/hash_map>
#define NMSPC __gnu_cxx
#else
#if defined(__INTEL_COMPILER)
#include <ext/hash_map>
#define NMSPC __gnu_cxx
#else
#include <hash_map>
#define NMSPC std
#endif
#endif

template<class T>
inline void resize (T& map, int iters)
{
}

#ifdef USE_GOOGLE_DENSE_MAP
template <
	typename TKey,
	typename TValue,
	typename THashFunc /* = std::hash<Key>*/,
	typename TEqualFunc = std::equal_to <TKey>,
	typename TAllocator = std::allocator < std::pair < TKey const, TValue > > >
class google_dense_hash_map : public google::dense_hash_map
	 <TKey, TValue, THashFunc, TEqualFunc, TAllocator>
{
private:
	typedef google::dense_hash_map
	<TKey, TValue, THashFunc, TEqualFunc, TAllocator> __base;

public:
	google_dense_hash_map ()
	{
		set_deleted_key ("\1");
		set_empty_key ("\2");
	}

	template <class Tit>
	google_dense_hash_map (
		Tit beg, Tit end)
		:
		__base (beg, end)
	{
		set_deleted_key ("\1");
		set_empty_key ("\2");
	}

	google_dense_hash_map (
		size_type n             = 0,
		const hasher& h         = hasher (), 
		const key_equal& k      = key_equal ())
		:
		__base (n, h, k)
	{
		set_deleted_key ("\1");
		set_empty_key ("\2");
	}

	~google_dense_hash_map ()
	{
	}
};
#endif

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

#ifdef TYPE_CHAR_PTR
typedef const char * my_type;
#else
typedef std::string my_type;
#endif

struct hsh_string_hash {
	size_t operator () (const std::string &key) const
	{
		return operator () (key.c_str ());
	}

	size_t operator () (const char *key) const
	{
#if 1
		static const unsigned char rand_256_array [] = {
			0xa8, 0x68, 0x0b, 0x60, 0xf6, 0xe4, 0xeb, 0x2c,
			0x86, 0xc3, 0xfb, 0xbd, 0x9e, 0x0d, 0x86, 0x9b,
			0x75, 0x55, 0xc1, 0x9e, 0x19, 0x7b, 0x1f, 0xc8,
			0x43, 0x0e, 0x3d, 0x4a, 0x80, 0xa7, 0xc1, 0x06,
			0x31, 0xbf, 0x9b, 0xbc, 0xdd, 0x81, 0x33, 0xd3,
			0x73, 0x0a, 0x87, 0xf9, 0xa2, 0xeb, 0x9a, 0xe7,
			0xcd, 0x59, 0x2c, 0xe6, 0x1d, 0x3d, 0xdb, 0x75,
			0xf9, 0x89, 0x86, 0x63, 0xa4, 0x51, 0x23, 0xa7,
			0xbf, 0x22, 0xf5, 0xd8, 0xbd, 0x5b, 0x25, 0xbd,
			0x5c, 0x9d, 0x64, 0xe9, 0x9b, 0x32, 0xdf, 0x3d,
			0x6f, 0xfe, 0xd9, 0xf6, 0x32, 0x66, 0x5e, 0xe7,
			0x03, 0x27, 0x31, 0x16, 0x62, 0x89, 0xff, 0x55,
			0x00, 0x1d, 0xb8, 0x87, 0x56, 0x84, 0xec, 0xe7,
			0x25, 0x60, 0xa0, 0xf2, 0xdf, 0xf3, 0x73, 0xf7,
			0xa8, 0xd0, 0x97, 0x90, 0x8a, 0x52, 0x99, 0xc5,
			0x9b, 0xac, 0xad, 0xf7, 0x91, 0x61, 0xbd, 0x05,
			0xf1, 0x56, 0x38, 0xcb, 0xba, 0x40, 0x0f, 0xb2,
			0x6b, 0xfb, 0x4b, 0xc2, 0xf0, 0xca, 0x15, 0xcd,
			0x5a, 0xd6, 0xb0, 0x24, 0x8b, 0xcf, 0x55, 0x4a,
			0xa1, 0xc3, 0xf6, 0x9b, 0xd9, 0x48, 0x80, 0x96,
			0xf2, 0x64, 0xdf, 0x4b, 0x97, 0x5d, 0x84, 0x2f,
			0x70, 0x12, 0x0d, 0x40, 0x77, 0x08, 0x58, 0x3d,
			0xee, 0xd3, 0x27, 0xf8, 0x82, 0xeb, 0x5e, 0x1a,
			0x09, 0x95, 0x89, 0x1f, 0xab, 0xf9, 0x0d, 0x34,
			0x66, 0x60, 0x46, 0xca, 0xe7, 0x52, 0xb5, 0x73,
			0xbc, 0x02, 0x26, 0x26, 0x93, 0x54, 0x89, 0xbd,
			0x9b, 0x6f, 0x2f, 0xd9, 0x51, 0xd0, 0xe3, 0xba,
			0x1d, 0xb6, 0xbc, 0x7a, 0xa5, 0x57, 0x50, 0x84,
			0xd0, 0x0f, 0xdf, 0x6d, 0xe2, 0xa6, 0xff, 0x51,
			0x73, 0x33, 0xd2, 0x2d, 0x61, 0x1a, 0xc3, 0xde,
			0xa3, 0x91, 0x0a, 0x46, 0x45, 0xed, 0x48, 0x1a,
			0xd8, 0x7a, 0x2a, 0x20, 0xe8, 0xe8, 0x4c, 0x0c
		};

		unsigned h = 0;
		const unsigned char *p=rand_256_array;
		while (*key){
			h = h * 31 ^ *key++ ^ *p;
			++p;
			if (p == rand_256_array + 256){
				p = rand_256_array;
			}
		}

		return h;// & 0x7FFFFF;
#else
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
#endif
	}
};

struct cmp_string_eq {
	bool operator () (char const *s1, char const *s2) const
	{
		if (s1 == s2)
			return true;
		else if (!!s1 ^ !!s2)
			return false;
		else if (s1 == (const char *)1)
			return false;
		else if (s1 == (const char *)2)
			return false;
		else if (s2 == (const char *)1)
			return false;
		else if (s2 == (const char *)2)
			return false;
		else
			return strcmp(s1, s2) == 0;
	}
	bool operator () (const std::string &s1, const std::string &s2) const
	{
		return operator () (s1.c_str (), s2.c_str ());
	}
};

struct cmp_string_lt {
	bool operator () (char const *s1, char const *s2) const
	{
		if (s1 == s2)
			return false;
		else if (s1 && !s2)
			return true;
		else if (!s1 && s2)
			return false;
		else
			return strcmp(s1, s2) < 0;
	}
	bool operator () (std::string const &s1, std::string const &s2) const
	{
		return operator () (s1.c_str (), s2.c_str ());
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

	size_t operator () (my_type const &key) const
	{
		return hsh_string_hash::operator () (key);
	}

	bool operator () (my_type const &key1, my_type const &key2) const
	{
		return cmp_string_lt::operator () (key1, key2);
	}
};

//typedef my_pool < std::pair <my_type const, int> > test_allocator_type;
//typedef std::allocator < std::pair <my_type const, int> > test_allocator_type;
typedef boost::fast_pool_allocator < std::pair <my_type const, int> > test_allocator_type;

#ifdef USE_GOOGLE_DENSE_MAP
typedef google_dense_hash_map <
	my_type, int, hsh_string_hash, cmp_string_eq, test_allocator_type
	> my_hash;

template <> void resize (my_hash& m, int n)
{
	m.resize (n);
}
#endif

#ifdef USE_JUDY_HASH
typedef judy_map_l <
	my_type, int, hsh_string_hash, cmp_string_eq, test_allocator_type
	> my_hash;

template <> void resize (my_hash& m, int n)
{
	m.resize (n);
}
#endif // USE_JUDY_HASH

#ifdef USE_HASH_MAP
//#ifdef __INTEL_COMPILER
//typedef std::hash_map <
//	my_type, int, dinkumware_hash_traits, test_allocator_type
//	> my_hash;
//#else // !__INTEL_COMPILER
typedef NMSPC::hash_map <
	my_type, int, hsh_string_hash, cmp_string_eq, test_allocator_type
	> my_hash;
//#endif // __INTEL_COMPILER
template <> void resize (my_hash& m, int n)
{
	m.resize (n);
}
#endif // USE_HASH_MAP

#ifdef USE_STD_MAP
typedef std::map <my_type, int, cmp_string_lt> my_hash;
#endif

void usage ()
{
	std::cerr << "usage: judyarray <num> < <filename>";
}

int main (int argc, const char **argv)
{
//	struct timeval tv_begin, tv_end;
//	memset (&tv_begin, 0, sizeof (tv_begin));
//	memset (&tv_end,   0, sizeof (tv_end));

//	if (gettimeofday (&tv_begin, NULL)){
//		std::cerr << strerror (errno) << '\n';
//		return 9;
//	}

	char line [2048];

	int dups = 0;

	--argc, ++argv;

	if (argc == 0){
		usage ();
		return 1;
	}

	unsigned long threshold = strtoul (argv [0], NULL, 10);
	std::cout << "threshold: " << threshold << '\n';

#ifdef USE_JUDY_HASH
	std::cout << "JudyArray: ";
#endif
#ifdef USE_HASH_MAP
	std::cout << "hash_map: ";
#endif
#ifdef USE_STD_MAP
	std::cout << "std::map: ";
#endif
#ifdef USE_GOOGLE_DENSE_MAP
	std::cout << "google::dense_hash_map: ";
#endif

#if defined(USE_JUDY_HASH) || (defined(USE_HASH_MAP) && !defined(__ICC)) || defined(USE_GOOGLE_DENSE_MAP)
	my_hash ht (0, hsh_string_hash (), cmp_string_eq ());
#else
	my_hash ht;
#endif

	unsigned long line_count = 0;
	while (fgets (line, sizeof (line), stdin)){
		++line_count;

#ifndef EMPTY_LOOP
		if ((line_count % threshold) == 0){
			ht.clear ();
			resize (ht, threshold);
		}
#endif

		char *NL = strchr (line, '\n');
		if (NL)
			*NL = 0;

#ifndef EMPTY_LOOP
#ifdef TYPE_CHAR_PTR
		std::pair <my_hash::iterator, bool> curr = ht.insert (
			my_hash::value_type (strdup (line), line_count));
#else
		std::pair <my_hash::iterator, bool> curr = ht.insert (
			my_hash::value_type (line, line_count));
#endif

		bool new_item = curr.second;
		if (!new_item){
			++dups;
		}
#else
#ifdef TYPE_CHAR_PTR
		strdup (line);
#endif
#endif
	}

	std::cout << "duplicates count: " << dups << '\n';

#ifdef USE_JUDY_HASH
	std::cout << "single item count:" << ht.get_debug_info ().m_value_count << '\n';
	std::cout << "list item count:  " << ht.get_debug_info ().m_list_item_count << '\n';
	std::cout << "list count:       " << ht.get_debug_info ().m_list_count << '\n';

	ht.key_eq ();
	ht.hash_funct ();
#endif

#if 0
	my_hash::iterator beg, end;
	beg = ht.begin ();
	end = ht.end ();

//	for (; !(beg == end); ++beg){
//		std::cout << "key2=`" << (*beg).first << "`\n";
//		std::cout << "value2=" << (*beg).second << "\n";
//	}
#endif

//	if (gettimeofday (&tv_end, NULL)){
//		std::cerr << strerror (errno) << '\n';
//		return 9;
//	}

//	std::cerr << tv_end.tv_usec - tv_begin.tv_usec << " microseconds\n";
//	std::cerr << tv_end.tv_sec - tv_begin.tv_sec << " seconds\n";
}
