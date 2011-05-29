/*
 * Copyright (c) 2011, Aleksey Cheusov <vle@gmx.net>
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

#ifndef _HASHFUNCS_H_
#define _HASHFUNCS_H_

#ifdef __cplusplus
extern "C" {
#endif

unsigned hash_str_poly31 (const char *key);
unsigned hash_str_poly65k (const char *key);
unsigned hash_str_poly2g (const char *key);

unsigned hash_int_poly31 (int key);
unsigned hash_int_poly65k (int key);
unsigned hash_int_poly2g (int key);

unsigned hash_str_random (const char *key);

#ifdef __cplusplus
};
#endif

#endif // _HASHFUNCS_H_
