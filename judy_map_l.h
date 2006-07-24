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

#ifndef _JUDY_MAP_L_H_
#define _JUDY_MAP_L_H_

#include "judyarray/judy_common.h"
#include "judyarray/judy_mapset_list.h"
#include "judyarray/judy_mapset_common.h"
#include "judyarray/judy_map_common.h"

template <typename TKey, typename TData, typename TEqualFunc>
class __judy_map_traits_list
:
public __judy_cmp_traits_list <TKey, TData, TEqualFunc, __judy_map_traits_base <TKey, TData> >
{
};

template <
	typename TKey,
	typename TData,
	typename THashFunc, // = std::hash <Key>,
	typename TEqualFunc = std::equal_to <TKey>,
	typename TAllocator = std::allocator < typename __judy_map_traits_list <TKey, TData, TEqualFunc>::value_type > >
class judy_map_l
{
private:
	typedef __judy_mapset_base <
		TKey, TData, THashFunc, TEqualFunc, TAllocator,
		__judy_map_traits_list <TKey, TData, TEqualFunc> > __impl;
	typedef judy_map_l <
		TKey, TData, THashFunc, TEqualFunc, TAllocator
		> __this_type;

	__impl m_hash_base;

public:

	__JUDYARRAY_TYPEDEFS(__impl)

// rempping judy_map/judy_set common members
	REMAP_FUNCALLS(__impl, judy_map_l, m_hash_base)

// judy_mem unique members
	mapped_type& operator [] (const key_type& key)
	{
		std::pair <iterator, bool> res = insert (
			value_type (key, mapped_type ()));

		return res.first -> second;
	}
};

#endif // _JUDY_MAP_L_H_
