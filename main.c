#include <search.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

static void strmyupr (char *s)
{
	while (*s){
		*s = toupper (*s);

		++s;
	}
}

typedef struct {
	int count;
	ENTRY items [2000000];
} my_allocator;

static ENTRY *my_allocate (void *allocator_data)
{
	my_allocator *alloc = (my_allocator *) allocator_data;

//	fprintf (stderr, "offs: %d\n", alloc -> count);
	return &alloc -> items [alloc -> count++];
}

int main (int argc, char **argv)
{
	int  dups = 0;
	char word [2000];
	ENTRY entry;
	ENTRY *found_entry = NULL;
	int ret = 0;
	struct hsearch_data hash_tab;
	my_allocator alloc;

	--argc;
	++argv;

	memset (&hash_tab, 0, sizeof (hash_tab));
	if (!hcreate_r (200000, &hash_tab))
		return 11;

//	((hsearch_data_internal *) hash_tab.data) -> m_allocator      = my_allocate;
//	((hsearch_data_internal *) hash_tab.data) -> m_allocator_data = &alloc;

	entry.data = 0;
	int i = 0;
	while (fgets (word, 2000, stdin)) {
		++i;
		word [strlen (word) - 1] = 0;

//		if (i >= 22420506)
//			strdup (word);

//		continue;

		entry.key   = word;
		entry.data  = (void *) 1;
//		strmyupr (entry.data);

		found_entry = NULL;

		ret = hsearch_r (entry, FIND, &found_entry, &hash_tab);
		if (ret){
			++dups;
//			printf ("find='%s' found='%s'\n",
//					word, (char *) found_entry -> data);
		}else if (errno == ESRCH){
			entry.key   = strdup (word);
			entry.data  = NULL;

			ret = hsearch_r (entry, ENTER, &found_entry, &hash_tab);
			if (!ret)
				return 3;
		}else{
			return 4;
		}
	}

	fprintf (stderr, "%lu duplicate lines\n\n",
			 dups);
	fprintf (stderr, "%lu conflicts found\n\n",
			 ((hsearch_data_internal *) hash_tab.data) -> m_conflicts);
	fprintf (stderr, "%lu std::list allocations\n\n",
			 ((hsearch_data_internal *) hash_tab.data) -> m_stdlist_allocs);
	fprintf (stderr, "%lu item allocations\n\n",
			 ((hsearch_data_internal *) hash_tab.data) -> m_item_allocs);
	fprintf (stderr, "%lu push_back count\n\n",
			 ((hsearch_data_internal *) hash_tab.data) -> m_push_back_count);

	while (argc);

	return (0);
}
