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

#ifndef _JUDY_FUNCS_WRAPPERS_H_
#define _JUDY_FUNCS_WRAPPERS_H_

#include <Judy.h>
#include <stdexcept>

inline void judyl_freearray (Pvoid_t &array)
{
	::JudyLFreeArray (&array, NULL);
}

inline PPvoid_t judyl_first (Pcvoid_t array, Word_t *index)
{
	return ::JudyLFirst (array, index, NULL);
}

inline PPvoid_t judyl_next (Pcvoid_t array, Word_t *index)
{
	return ::JudyLNext (array, index, NULL);
}

inline int judyl_del (Pvoid_t &array, Word_t index)
{
	return ::JudyLDel (&array, index, NULL);
}

inline PPvoid_t judyl_get (Pvoid_t array, Word_t index)
{
	return ::JudyLGet (array, index, NULL);
}

inline PPvoid_t judyl_ins (Pvoid_t &array, Word_t index)
{
	return ::JudyLIns (&array, index, NULL);
}

inline Word_t judyl_count (Pvoid_t array, Word_t f, Word_t l)
{
	return ::JudyLCount (array, f, l, NULL);
}

#endif // _JUDY_FUNCS_WRAPPERS_H_
