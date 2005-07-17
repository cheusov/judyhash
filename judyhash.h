
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stddef.h>

#include <Judy.h>

	typedef struct entry {
		char *key;
		void *data;
	} ENTRY;

	typedef enum {
		FIND,
		ENTER
	} ACTION;

	struct hsearch_data {
		void *data;
	};

	typedef struct {
		Pvoid_t  m_judy1;
		Pvoid_t  m_judy2;

		int      m_conflicts;

		int      m_item_allocs;
		int      m_stdlist_allocs;
		int      m_push_back_count;

		void *   m_allocator_data;
		ENTRY *  (*m_allocator) (void *);
	} hsearch_data_internal;

// hcreate_r!
#define hcreate_r judyhash_hcreate_r
	extern int judyhash_hcreate_r (size_t nel, struct hsearch_data *tab);

// hsearch_r!
#define hsearch_r judyhash_hsearch_r
	extern int judyhash_hsearch_r (
		ENTRY item,
		ACTION action,
		ENTRY **ret,
		struct hsearch_data *tab);

// hdestroy_r!
#define hdestroy_r judyhash_hdestroy_r
	extern void judyhash_hdestroy_r (struct hsearch_data *tab);
#ifdef __cplusplus
}
#endif
