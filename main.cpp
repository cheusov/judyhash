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

#include "judyhash.h"

#ifdef USE_GOOGLE_DENSE_MAP
#include <google/dense_hash_map>

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

#ifdef USE_HASH_MAP
#if ((__GNUC__ >= 3) && !defined(_STLP_CONFIG_H))
#include <ext/hash_map>
#else
#if defined(__INTEL_COMPILER)
#include <ext/hash_map>
#else
#include <hash_map>
#endif
#endif
#endif

#ifdef USE_STD_MAP
#include <map>
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

//typedef const char * my_type;
typedef std::string my_type;

struct hsh_string_hash {
	size_t operator () (const std::string &key) const
	{
		return operator () (key.c_str ());
	}

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
#endif

#ifdef USE_JUDY_HASH
typedef judyhash_map <
	my_type, int, hsh_string_hash, cmp_string_eq, test_allocator_type
	> my_hash;
#endif // USE_JUDY_HASH

#ifdef USE_HASH_MAP
#ifdef __INTEL_COMPILER
//typedef std::hash_map <
//	my_type, int, dinkumware_hash_traits, test_allocator_type
//	> my_hash;
typedef __gnu_cxx::hash_map <
	my_type, int, hsh_string_hash, cmp_string_eq, test_allocator_type
	> my_hash;
#else // !__INTEL_COMPILER
#if __GNUC__ >= 3 && !defined(_STLP_CONFIG_H)
typedef __gnu_cxx::hash_map <
	my_type, int, hsh_string_hash, cmp_string_eq, test_allocator_type
	> my_hash;
#else // __GNUC__ < 3
typedef std::hash_map <
	my_type, int, hsh_string_hash, cmp_string_eq, test_allocator_type
	> my_hash;
#endif // __GNUC__ >= 3
#endif // __INTEL_COMPILER
#endif // USE_HASH_MAP

#ifdef USE_STD_MAP
typedef std::map <my_type, int, cmp_string_lt> my_hash;
#endif

void usage ()
{
	std::cerr << "usage: judyhash <num> < <filename>";
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
	std::cout << "JudyHash: ";
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
		}
#endif

		char *NL = strchr (line, '\n');
		if (NL)
			*NL = 0;

#ifndef EMPTY_LOOP
		std::pair <my_hash::iterator, bool> curr = ht.insert (
			my_hash::value_type (line, line_count));
//				strdup (line), line_count));

		bool new_item = curr.second;
		if (!new_item){
			++dups;
		}
#else
//		strdup (line);
#endif
	}

	std::cout << "duplicates count: " << dups << '\n';

#ifdef USE_JUDY_HASH
	std::cout << "single item count:" << ht.m_debug_info.m_value_count << '\n';
	std::cout << "list item count:  " << ht.m_debug_info.m_list_item_count << '\n';
	std::cout << "list count:       " << ht.m_debug_info.m_list_count << '\n';

	ht.key_eq ();
	ht.hash_funct ();
#endif

#if 0
	my_hash::iterator beg, end;
	beg = ht.begin ();
	end = ht.end ();

	for (; !(beg == end); ++beg){
		std::cout << "key2=`" << (*beg).first << "`\n";
		std::cout << "value2=" << (*beg).second << "\n";
	}
#endif

//	if (gettimeofday (&tv_end, NULL)){
//		std::cerr << strerror (errno) << '\n';
//		return 9;
//	}

//	std::cerr << tv_end.tv_usec - tv_begin.tv_usec << " microseconds\n";
//	std::cerr << tv_end.tv_sec - tv_begin.tv_sec << " seconds\n";
}
