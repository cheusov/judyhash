#include "judyhash.h"

#include <assert.h>
#include <errno.h>
#include <string.h>

#include <list>
#include <algorithm>

//#define MULT1 3U
//#define MULT1 7U
//#define MULT1 31U
//#define MULT1 33U
//#define MULT1 65599U
#define MULT1 2654435789U

#define MULT2 (MULT1*MULT1)
#define MULT3 (MULT2*MULT1)

#if 1
unsigned long hsh_string_hash (const char *key)
{
	register unsigned h = 0 ; 

	while (*key)
		h = h * MULT1 + *key++ ; 

	return h ; 
}
#else

// Zobrist
unsigned long hsh_string_hash (const char *key)
{
	static const unsigned long multiplyers [] = {
		3985730824,
		2379127401,
		588924680,
		2966272510,
		2253240334,
		2916101190,
		1230155606,
		2439554122,
		3558490022,
		3272059162,
		1273375956,
		1849317937,
		485481804,
		597095348,
		3080212372,
		1489879919,
		3511471284,
		185519581,
		2288457303,
		2185871248,
		2831638102,
		3819017460,
		3878593660,
		2300488227,
		2944999867,
		877127176,
		3142383701,
		3032312908,
		1376345957,
		1177632216,
		1204543471,
		2756263844,
		839113342,
		546729481,
		3657860487,
		251725051,
		1656760305,
		561171492,
		3827866746,
		2535256929,
		2434602859,
		2332799757,
		997972649,
		3431885100,
		7995786,
		3140113785,
		2255172080,
		3180197018,
		1731420065,
		2411638234,
		254455901,
		3480079525,
		1753870199,
		3816400702,
		167550548,
		909619996,
		1375893030,
		1651193561,
		2895344098,
		3909537387,
		2594633239,
		2287747656,
		2574625185,
		2060221887,

		746568883,
		178453983,
		186449769,
		3326563555,
		1581735635,
		761932652,
		2493352717,
		904990951,
		1159446852,
		639526378,
		2393396577,
		2209797279,
		2377347829,
		3286967826,
		662860855,
		2314054417,
		1209398514,
		1118935900,
		3713569141,
		2001316796,
		575941980,
		2636163867,
		111613543,
		2483117024,
		2409667548,
		2021591364,
		674048297,
		2102937721,
		3897413946,
		645766852,
		987379357,
		59576200,
		2421894566,
		644511640,
		1932127310,
		2265256523,
		3889929208,
		2344033050,
		3801286261,
		26911254,
		1551720370,
		2082849499,
		3707616140,
		3111131004,
		593864565,
		1405035252,
		2904411188,
		3266695251,
		2746860764,
		3147507210,
		1923825030,
		3476217869,
		2646195992,
		643349809,
		1771830775,
		1441484649,
		3019375782,
		307172844,
		3415186423,
		3861123112,
		2842316731,
		258113209,
		6709443,
		753278328,
		931732312,
		1118182082,
		444745635,
		2026481271,
		2788413924,
		1281766640,
		2186757593,
		3346204446,
		3985730824,
		2379127401,
		588924680,
		2966272510,
		2253240334,
		2916101190,
		1230155606,
		2439554122,
		3558490022,
		3272059162,
		1273375956,
		1849317937,
		485481804,
		597095348,
		3080212372,
		1489879919,
		3511471284,
		185519581,
		2288457303,
		2185871248,
		2831638102,
		3819017460,
		3878593660,
		2300488227,
		2944999867,
		877127176,
		3142383701,
		3032312908,
		1376345957,
		1177632216,
		1204543471,
		2756263844,
		839113342,
		546729481,
		3657860487,
		251725051,
		1656760305,
		561171492,
		3827866746,
		2574727508,
		1722234719,
		3646059749,
		3122277618,
		1768473609,
		2411823420,
		183654194,
		1625138844,
		644514626,
		951687470,
		366873892,
		227997003,
		3070313736,
		3328426945,
		3335136389,
		88414715,
		1020147027,
		2138329110,
		2583074747,
		609556017,
		3397969944,
		679736582,
		2866494176,
		2212698622,
		2198429445,
		577556846,
		1166481527,
		132754035,
		2385994370,
		1302095559,
		2532251165,
		971805286,
		530295308,
		3802354471,
		1075730428,
		2925048365,
		3410530170,
		7625518,
		3087837891,
		577717811,
		89189093,
		274708675,
		2563165980,
		749037227,
		3580675329,
		3399692790,
		3278286449,
		1578774674,
		523774542,
		1400901718,
		543285418,
		3575598327,
		951944284,
		2129576501,
		3334119975,
		2090383817,
		2929497159,
		3476226642,
		3070313736,
		3328426945,
		3335136389,
		88414715,
		1020147027,
		2138329110,
		2583074747,
		609556017,
		3397969944,
		679736582,
		2866494176,
		2212698622,
		2198429445,
		577556846,
		1166481527,
		3070313736,
		3328426945,
		3335136389,
		88414715,
		1020147027,
		2138329110,
		2583074747,
	};

	const unsigned long *multiplyer = multiplyers;

	unsigned long h      = 0;
	assert (key);

	int i=0;
	while (*key) {
		//		h += ((unsigned char)*key++) * (*multiplyer++);
		h ^= multiplyers [((unsigned char)*key++ + i) & 0xFF];
		++i;

		//		if (multiplyer == multiplyers + sizeof (multiplyers)/sizeof (multiplyers[0]))
		//			multiplyer == multiplyers;
	}

	return h;
}
#endif

static ENTRY *judyhash_allocate (void *allocator_data)
{
	return new ENTRY;
}

int judyhash_hcreate_r (size_t nel, struct hsearch_data *tab)
{
	hsearch_data_internal *tab_real = NULL;

	assert (tab);

	tab -> data = malloc (sizeof (hsearch_data_internal));
	if (!tab -> data)
		return 0;

	tab_real = (hsearch_data_internal *) tab -> data;
	memset (tab_real, 0, sizeof (*tab_real));

	tab_real -> m_allocator      = judyhash_allocate;
	tab_real -> m_allocator_data = NULL;

	return 1;
}

typedef std::pair <const char *, void *> pair_key_data_t;

struct list_data_t : public ENTRY {
	list_data_t (char *a, void *b)
	{
		key = a;
		data = b;
	}

	list_data_t ()
	{
		key  = NULL;
		data = NULL;
	}

	bool operator == (const list_data_t& a) const
	{
		return !strcmp (a.key, key);
	}
};

typedef std::list <list_data_t> list_str;
typedef list_str::iterator list_str__it;

typedef union {
	PWord_t       m_judy_ptr;
	PWord_t       m_judy_ptr_ptr;
	Word_t        m_integer;
	ENTRY        *m_data_ptr;
	list_str     *m_list_ptr;
	ENTRY       **m_data_ptr_ptr;
	list_str    **m_list_ptr_ptr;
} judyhash_ptr;

int judyhash_hsearch_r (
	ENTRY item,
	ACTION action,
	ENTRY **ret,
	struct hsearch_data *tab)
{
	hsearch_data_internal *tab_real = (hsearch_data_internal *) (tab -> data);

	Word_t hash_key = (Word_t) hsh_string_hash (item.key);

	switch (action){
		case FIND:
			{
				// look for hash key in the Judy array
				judyhash_ptr ptr;
				ptr.m_judy_ptr = (PWord_t) JudyLGet (tab_real -> m_judy1, hash_key, 0);

				if (!ptr.m_judy_ptr){
					// hash key was not found
					errno = ESRCH;
					return 0;
				}

				ptr.m_integer = *ptr.m_judy_ptr;

//				return 1;
				if ((ptr.m_integer & 1) == 0){
					// found hash key is assigned
					// with the single (key,data) pair

					if (!strcmp (ptr.m_data_ptr -> key, item.key)){
						*ret = ptr.m_data_ptr;
						return 1;
					}else{
						errno = ESRCH;
						return 0;
					}
				}else{
					// found hash key is assigned
					// with list of (key,data) pairs
					ptr.m_integer = ptr.m_integer & ~1;

					list_data_t pair_key_data (item.key, NULL);

					list_str__it end = ptr.m_list_ptr -> end ();
					list_str__it found = std::find (
						ptr.m_list_ptr -> begin (), end, pair_key_data);

					if (found == end){
						errno = ESRCH;
						return 0;
					}

					*ret = &(*found);
					return 1;
				}
			}
		case ENTER:
			{
				judyhash_ptr ptr;
				ptr.m_judy_ptr = (PWord_t) JudyLIns (&tab_real -> m_judy1, hash_key, 0);
				if (!ptr.m_judy_ptr)
					return 0;

//				return 1;
				if (!*ptr.m_judy_ptr){
					++tab_real -> m_item_allocs;

					*ret = (*tab_real -> m_allocator) (tab_real -> m_allocator_data);
					(*ret) -> key  = item.key;
					(*ret) -> data = item.data;

					*ptr.m_data_ptr_ptr = *ret;

					return 1;
				}else if ((*ptr.m_judy_ptr & 1) == 0){
					if (!strcmp ((*ptr.m_data_ptr_ptr) -> key, item.key)){
						(*ptr.m_data_ptr_ptr) -> data = item.data;
						*ret = *ptr.m_data_ptr_ptr;
						return 1;
					}else{
						list_data_t *key_data = (list_data_t *) *ptr.m_data_ptr_ptr;

						++tab_real -> m_stdlist_allocs;
						*ptr.m_list_ptr_ptr = new list_str;

						++tab_real -> m_push_back_count;
						(*ptr.m_list_ptr_ptr) -> push_back (*key_data);

						++tab_real -> m_push_back_count;
						(*ptr.m_list_ptr_ptr) -> push_back (list_data_t (item.key, item.data));

						*ret = &(*ptr.m_list_ptr_ptr) -> back ();

						*ptr.m_judy_ptr |= 1;
						return 1;
					}
				}else{
					list_str *res = (list_str *) (*ptr.m_judy_ptr & ~1);

					list_data_t pair_key_data (item.key, item.data);
					list_str__it end = res -> end ();
					list_str__it found = std::find (res -> begin (), end, pair_key_data);
					if (found == end){
						++tab_real -> m_push_back_count;
						res -> push_back (pair_key_data);

						*ret = &res -> back ();
						return 1;
					}else{
						(*found).data = item.data;

						*ret = &(*found);
						return 1;
					}
				}
			}
	}

	return 0;
}

void judyhash_hdestroy_r (struct hsearch_data *tab)
{
	hsearch_data_internal *tab_real = (hsearch_data_internal *) (tab -> data);

	assert (tab);
	assert (tab -> data);

	JudyLFreeArray (&tab_real -> m_judy1, 0);
	JudyLFreeArray (&tab_real -> m_judy2, 0);

	free (tab -> data);
}
