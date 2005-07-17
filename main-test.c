#define _GNU_SOURCE

#include <search.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main (int argc, char **argv)
{
	int  dups = 0;
	char word [2000];
	char data [2000];

	ENTRY entry;
	ENTRY *found_entry = NULL;
	int ret = 0;
	struct hsearch_data hash_tab;

	--argc;
	++argv;

	memset (&hash_tab, 0, sizeof (hash_tab));
	if (!hcreate_r (200000, &hash_tab))
		return 11;

	entry.data = 0;
	while (
		fgets (word, 2000, stdin) &&
		fgets (data, 2000, stdin))
	{
		word [strlen (word) - 1] = 0;

		entry.key   = word;
		entry.data  = (void *) data;
//		strmyupr (entry.data);

		found_entry = NULL;
		ret = hsearch_r (entry, FIND, &found_entry, &hash_tab);
		if (ret){
			++dups;
			printf ("find1='%s' found='%s'\n",
					word, (char *) found_entry -> data);
		}else if (errno == ESRCH){
			entry.key   = strdup (word);
			entry.data  = data;

			ret = hsearch_r (entry, ENTER, &found_entry, &hash_tab);
			if (!ret)
				return 3;

			printf ("find2='%s' found='%s'\n",
					word, (char *) found_entry -> data);
		}else{
			return 4;
		}
	}

	fprintf (stderr, "%lu duplicate lines\n\n",
			 dups);
//	fprintf (stderr, "%lu conflicts found\n\n",
//			 ((hsearch_data_internal *) hash_tab.data) -> m_conflicts);
//	fprintf (stderr, "%lu list allocations\n\n",
//			 ((hsearch_data_internal *) hash_tab.data) -> m_list_allocations);
//	fprintf (stderr, "%lu item allocations\n\n",
//			 ((hsearch_data_internal *) hash_tab.data) -> m_item_allocations);

	while (argc);

	return (0);
}
