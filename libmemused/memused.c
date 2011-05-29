#include <sys/stat.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned memused (void)
{
#if defined(__NetBSD__) || defined(sun)
	char path [200];
#if defined(__NetBSD__)
	snprintf (path, sizeof (path), "/proc/%d/mem", (int) getpid ());
#else
	snprintf (path, sizeof (path), "/proc/%d/as", (int) getpid ());
#endif
	struct stat sb;
	if (stat (path, &sb)){
		fprintf (stderr, "Cannot obtain amount of occupied memory from %s\n", path);
		return 0;
	}

	return (int) sb.st_size;
#else
#	if defined(__linux)
	char path [200];
	snprintf (path, sizeof (path), "/proc/%d/stat", (int) getpid ());
	FILE *fd = fopen (path, "r");
	if (!fd)
		return 0;

	char buf [20480];
	if (!fgets (buf, sizeof (buf), fd))
		return 0;

	char *p = NULL;
	strtok (buf, " ");
	for (int cnt=22; cnt--; ){
		p = strtok (NULL, " ");
	}
	return strtol (p, NULL, 10);
#	else
	return 0;
#	endif
#endif
}
