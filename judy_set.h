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

#ifndef _JUDY_SET_H_
#define _JUDY_SET_H_

#include "judyarray/judy_common.h"
#include "judyarray/judy_mapset_list.h"
#include "judyarray/judy_mapset_common.h"

//////////////////////////////////////////////////////////////////////

template <typename TKey>
struct __judy_set_traits_base {
	typedef TKey                            key_type;
	typedef TKey                            data_type;
	typedef TKey                            mapped_type;
	typedef TKey                            value_type;
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
		return value;
	}
	static const value_type &key2value (const key_type& key)
	{
		return key;
	}
};


//////////////////////////////////////////////////////////////////////
//////////////////////////     JUDY_SET_L         ////////////////////

template <typename TKey, typename TEqualFunc>
class __judy_list_set
:
public __judy_cmp_traits_list <TKey, void, TEqualFunc, __judy_set_traits_base <TKey> >
{
};


template <
	typename TKey,
	typename THashFunc, // = std::hash <Key>,
	typename TEqualFunc = std::equal_to <TKey>,
	typename TAllocator = std::allocator < TKey> >
class judy_set_l
{
private:
	typedef __judy_base <
		TKey, char, THashFunc, TEqualFunc, TAllocator,
		__judy_list_set <TKey, TEqualFunc> > __impl;
	typedef judy_set_l <
		TKey, THashFunc, TEqualFunc, TAllocator> __this_type;

	__impl m_hash_base;

public:

	__JUDYARRAY_TYPEDEFS(__impl)

// rempping judy_map/judy_set common members
	REMAP_FUNCALLS(__impl, judy_set_l, m_hash_base)

// judy_mem unique members
};

//////////////////////////////////////////////////////////////////////
//////////////////////////     JUDY_SET_S         ////////////////////

#endif // _JUDY_SET_H_
