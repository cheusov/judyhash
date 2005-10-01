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

#ifndef _JUDY_MAP_H_
#define _JUDY_MAP_H_

#include "judyarray/judy_common.h"
#include "judyarray/judy_mapset_list.h"
#include "judyarray/judy_mapset_common.h"

template <typename TKey, typename TValue>
struct __judy_map_traits_base {
	typedef TKey                            key_type;
	typedef TValue                          data_type;
	typedef TValue                          mapped_type;
#ifdef JUDYARRAY_NO_CONST
	typedef std::pair <      TKey, TValue>  value_type;
#else
	typedef std::pair <const TKey, TValue>  value_type;
#endif

	typedef size_t                          size_type;
	typedef ptrdiff_t                       difference_type;

	// It is not possible to derive 'pointer' and 'reference' types from
	// TAllocator
	typedef value_type                   * pointer;
	typedef value_type             const * const_pointer;
	typedef value_type                   & reference;
	typedef value_type             const & const_reference;

	static const key_type &value2key (const value_type& value)
	{
		return value.first;
	}
	static value_type key2value (const key_type& key)
	{
		return value_type (key, mapped_type ());
	}
};

template <typename TKey, typename TValue, typename TEqualFunc>
class __judy_list_map
:
public __judy_list_traits_base <TKey, TValue, TEqualFunc, __judy_map_traits_base <TKey, TValue> >
{
};

template <
	typename TKey,
	typename TValue,
	typename THashFunc, // = std::hash <Key>,
	typename TEqualFunc = std::equal_to <TKey>,
	typename TAllocator = std::allocator < typename __judy_list_map <TKey, TValue, TEqualFunc>::value_type > >
class judy_map_l
{
private:
	typedef __judy_base <
		TKey, TValue, THashFunc, TEqualFunc, TAllocator,
		__judy_list_map <TKey, TValue, TEqualFunc> > __impl;
	typedef judy_map_l <
		TKey, TValue, THashFunc, TEqualFunc, TAllocator
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

#endif // _JUDY_MAP_H_
