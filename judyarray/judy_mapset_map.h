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

#ifndef _JUDY_MAPSET_MAP_H_
#define _JUDY_MAPSET_MAP_H_

#include <map>

template <typename TKey, typename TData,
	typename TLessFunc, typename TTraits>
class __judy_cmp_traits_map
:
public TTraits
{
public:
	class compare {
	private:
		TLessFunc m_cmp;

	public:
		bool operator () (typename TTraits::pointer a,
					   typename TTraits::pointer b) const
		{
			return m_cmp (a -> first, b -> first);
		}
	};

	class __pointers_list_type
	:
	public std::map <typename TTraits::pointer, compare>
	{
		typedef std::map <typename TTraits::pointer, compare> inherited;
	public:
		typedef typename __pointers_list_type::pointer pointer;
		typedef typename __pointers_list_type::value_type value_type;
		typedef typename __pointers_list_type::pointer pointer;
		typedef typename __pointers_list_type::iterator iterator;

		__pointers_list_type ()
		{
		}
		~__pointers_list_type ()
		{
		}

		typename std::map <pointer, compare>::iterator find (
			const TKey &key)
		{
//			typename TTraits::value_type tmp (key, TData ());
//			return this -> find (&tmp);
		}

//		std::pair <iterator, bool> insert (const pointer& p)
//		{
//			typename TTraits::value_type tmp (key, TData ());
//			return this -> insert (&tmp);
//		}
	};

	typedef __pointers_list_type pointers_list_type;
};

#endif // _JUDY_MAPSET_LIST_H_
