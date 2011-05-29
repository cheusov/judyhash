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
template <unsigned M>
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
	size_t operator () (int key) const
	{
		int b1 = key & 255;
		key >>= 8;
		int b2 = key & 255;
		key >>= 8;
		int b3 = key & 255;
		key >>= 8;
		int b4 = key & 255;

		unsigned h = 0;
		h = b1;
		h = h * M ^ b2;
		h = h * M ^ b3;
		h = h * M ^ b4;
		return h;
	}
};

// mixed
struct hashfunc_random {
	size_t operator () (const char *key) const
	{
		return hash_str_random (key);
	}

	size_t operator () (const std::string &key) const
	{
		return hash_str_random (key.c_str ());
	}
};

#endif // _HASH_FUNCS_H_
