/*
 * Copyright (c) 2005, Aleksey Cheusov <vle@gmx.net>
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation.  I make no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied
 * warranty.
 *
 */

#ifndef _HASH_FUNCS_H_
#define _HASH_FUNCS_H_

#include <string>

// multinomial
template <int M>
struct hashfunc_poly {
	size_t operator () (const char *key) const
	{
		size_t h = 0;

		while (*key){
			h = h * M ^ *key++;
		}

		return h; 
	}
	size_t operator () (const std::string &key) const
	{
		return operator () (key.c_str ());
	}
};

extern const unsigned rand_256_array [256];

// mixed
template <int M>
struct hashfunc_mixed {
	size_t operator () (const char *key) const
	{
		unsigned h = 0;

		const unsigned *p=rand_256_array;
		int cnt = 256;
		for (; *key; ++key){
			if (cnt--){
				cnt = 256;
				p=rand_256_array;
			}

			int ch = (unsigned char) *key;
			h = h * M ^ ch ^ *p++;
		}

		return h;
	}

	size_t operator () (const std::string &key) const
	{
		return operator () (key.c_str ());
	}
};

#endif // _HASH_FUNCS_H_
