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

#ifndef _JUDY_MAPSET_LIST_H_
#define _JUDY_MAPSET_LIST_H_

#include <list>

template <typename TKey, typename TData,
	typename TEqualFunc, typename TTraits>
class __judy_cmp_traits_list
:
public TTraits
{
public:
	class __pointers_list_type
	: public std::list <typename TTraits::pointer>
	{
	private:
		TEqualFunc m_eq_func;
	public:
		typedef typename TTraits::pointer pointer;

		__pointers_list_type ()
		{
		}
		~__pointers_list_type ()
		{
		}
		typename std::list <pointer>::iterator find (
			const TKey &key)
		{
			typename std::list <pointer>::iterator beg
				= std::list <pointer>::begin ();
			typename std::list <pointer>::iterator end
				= std::list <pointer>::end ();

			for (; !(beg == end); ++beg){
				if (m_eq_func (TTraits::value2key (**beg), key)){
					return beg;
				}
			}

			return std::list <pointer>::end ();
		}
	};

	typedef __pointers_list_type pointers_list_type;
};

#endif // _JUDY_MAPSET_LIST_H_
