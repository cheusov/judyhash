//#include <sys/time.h>
//#include <time.h>
#include <string.h>
#include <assert.h>

#include <iostream>
#include <string>
#include <vector>
//#include <pool/pool_alloc.hpp>

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
		static const unsigned rand_256_array [] = {
			0xfd40ef73, 0x37b28def, 0xc2c58c4b, 0x1f695396,
			0xb42e5906, 0x3e8ac47b, 0x181fddd7, 0x044084f9,
			0x808c4c57, 0x8674f381, 0xeff56f58, 0x879d8ff3,
			0x8eb6a732, 0xf6aa47ff, 0xd75c273f, 0x86775557,
			0xf99117e4, 0x49003568, 0xd3331fa3, 0x59d39c95,
			0x711a4e0c, 0xee2b3d4a, 0xabbbd6a1, 0x966680c7,
			0x3c167915, 0x45e789f2, 0x436147db, 0xc8792bb7,
			0x9cbf3727, 0xcb62d840, 0x992b813a, 0x66daa431,
			0x04932cac, 0xa13a8948, 0xe999483f, 0x3ff64428,
			0x70245740, 0x84bbfdb0, 0x5456c181, 0x8d5c2cc4,
			0x79094721, 0xe271c027, 0x4c46a46a, 0xa41500ab,
			0xba4f3345, 0xe8604635, 0x91d8d99e, 0xee5d9203,
			0x2634518e, 0x959f6120, 0x9a776b22, 0xfd7bc362,
			0xcc8a878f, 0x0af20740, 0xd16a2753, 0xa7f2e911,
			0xb425c449, 0x8e7faad6, 0xe8b288e2, 0x978dee01,
			0x70e1c6f6, 0xe747f9d5, 0xc702b929, 0xc8462850,
			0x61a6caa6, 0xf211a412, 0xecfd3eb0, 0x928cb3bf,
			0xd0418ed0, 0x28aa100d, 0x4afbce36, 0x278afb09,
			0x66c9e7fe, 0x887bd3a6, 0xd2a8f130, 0x1d7972c5,
			0xd63c98d5, 0x6f6accb4, 0x652b1544, 0xb69015cc,
			0x6503f99f, 0x850c257b, 0xe4ca19fe, 0xd1471987,
			0x738924c1, 0x95558227, 0x98160429, 0xd969b68f,
			0x6efe8081, 0x10bcfd14, 0x26b5c821, 0xccebea16,
			0xc143fbb0, 0x50d59067, 0xfc015f51, 0x19a67921,
			0x83bb71ec, 0x97cd8cbe, 0x1b3ecb15, 0xbfc1da48,
			0x010aae37, 0x2e9080f4, 0xba368b0e, 0xe4dae282,
			0x05fbd3ce, 0x71bc8cd5, 0x327d2e15, 0x34c79e9c,
			0xfb303206, 0xe4ecd8c3, 0x282b8aeb, 0x9ea1d2e2,
			0xaf49560f, 0x8d2212c7, 0x8f501de5, 0x73be8c06,
			0xf8463a16, 0x049084a1, 0x69635326, 0x4ae79f71,
			0x4c2da09b, 0xca30b705, 0xa944dbf8, 0x91fa48b2,
			0xaa3c9c71, 0xd27f80fb, 0x4e4a8b7b, 0xd0588268,
			0x30a1fd64, 0x0cb7a269, 0x25935567, 0xb4e7027e,
			0xbd3b4e2f, 0x35364cd1, 0xbd0d6e8c, 0x75fe9419,
			0x6fe135c3, 0x028a9bbb, 0x78212de7, 0xf86c2c77,
			0xc381f5d6, 0x4669f262, 0x8352666a, 0x6b7a5a86,
			0xf4b0fd7e, 0xa55bb8fb, 0x8dbdfb85, 0x07c20a31,
			0x3909a0ff, 0x295702a6, 0x7613a8d7, 0xe1ee3730,
			0x66174ad5, 0x70e443e8, 0x3ca5699a, 0xfe44df2f,
			0xc2d78601, 0xce14426d, 0x6297a844, 0xe5ce7876,
			0x8457c4b7, 0xf706777d, 0x9c0d6670, 0xbb00a11c,
			0x90041efe, 0x4b962c0a, 0xb616033e, 0x71320b8c,
			0xd518d62c, 0x27ae39ee, 0x18033552, 0x913b5ed5,
			0xccd84ed7, 0xb272dc0b, 0xd1adc5bd, 0x9c992e67,
			0x4d2444c9, 0x2f65fcf6, 0x7c76cb21, 0x8627caf6,
			0x83e92f47, 0x8b5a7655, 0xe4bd749b, 0x23ba5473,
			0x6f9b35d4, 0x8e7d11fc, 0xe14ba84d, 0x36fbc1ac,
			0x34cba098, 0x6cbc6b7a, 0xb4e3ac46, 0x2cc67f7f,
			0x01135c24, 0x8b18a4d9, 0x39c9d3c3, 0x2d5250d9,
			0x45aa5771, 0x5d2ad6fc, 0x6a8ce9f1, 0xa6f6cff0,
			0x7d2efc48, 0x732056ec, 0x8b0eb168, 0xa674e849,
			0x3fe7ea60, 0xc89879c3, 0x650a5709, 0x86bfa7b6,
			0xc6e8d4a8, 0x5d56a841, 0xc13c9398, 0x9bba26bf,
			0x4e9f9a98, 0x02693d12, 0x598dbfb6, 0x27193676,
			0x5a987872, 0x480fcbf5, 0x088c83d7, 0x854d0369,
			0x423bcd3b, 0xf465f9c2, 0x17941c6f, 0xc7048321,
			0x1f117a14, 0xeedf234c, 0x19ebda99, 0x92b49be0,
			0x9bcd10f5, 0xbb2624f7, 0x156a818e, 0xd9a1153f,
			0xf466fcde, 0x9f43ab95, 0xedf2caed, 0x0d3ceceb,
			0x457ee5a5, 0xf5e4da52, 0x8a3998ce, 0x19d53758,
			0x93f524cf, 0x10a8cf70, 0xb624d190, 0x210a0741,
			0x02b5fd3c, 0xd1f81cc9, 0xc55f7fcf, 0xb55eeae9,
			0x61c05df2, 0x836307b3, 0xc9ea8a6c, 0xeeb9cad2,
		};

		unsigned h = 0;
		const unsigned *p=rand_256_array;
		while (*key){
			h = h * 31 ^ *key++ ^ *p;
			++p;
			if (p == rand_256_array + 256){
				p = rand_256_array;
			}
		}

		return h;// & 0x0FFFFFF;
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
typedef std::allocator < std::pair <my_type const, int> > test_allocator_type;
//typedef boost::fast_pool_allocator < std::pair <my_type const, int> > test_allocator_type;

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
typedef judy_map_m <
	my_type, int, hsh_string_hash, cmp_string_lt, cmp_string_eq, test_allocator_type
//	my_type, int, hsh_string_hash, cmp_string_eq, test_allocator_type
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
