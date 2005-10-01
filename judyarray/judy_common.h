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

#ifndef _JUDY_COMMON_H_
#define _JUDY_COMMON_H_

#include "Judy.h"

////////////////////////////////////////////////////////////
///  defining JUDYARRAY_DEBUGINFO
#ifdef JUDYARRAY_NO_DEBUGINFO

#ifdef JUDYARRAY_DEBUGINFO
#undef JUDYARRAY_DEBUGINFO
#endif // JUDYARRAY_DEBUGINFO

#else

#if !defined(JUDYARRAY_DEBUGINFO) && !defined(NDEBUG)
#define JUDYARRAY_DEBUGINFO
#endif // JUDYARRAY_DEBUGINFO && NDEBUG

#endif // JUDYARRAY_NO_DEBUGINFO
////////////////////////////////////////////////////////////

#define __JUDYARRAY_TYPEDEFS(macrosarg_from)                             \
	typedef typename macrosarg_from::key_type        key_type;           \
	typedef typename macrosarg_from::data_type       data_type;          \
	typedef typename macrosarg_from::mapped_type     mapped_type;        \
	typedef typename macrosarg_from::value_type      value_type;         \
	typedef typename macrosarg_from::size_type       size_type;          \
	typedef typename macrosarg_from::difference_type difference_type;    \
    \
	typedef typename macrosarg_from::pointer         pointer;            \
	typedef typename macrosarg_from::const_pointer   const_pointer;      \
	typedef typename macrosarg_from::reference       reference;          \
	typedef typename macrosarg_from::const_reference const_reference;


#endif // _JUDY_COMMON_H_
