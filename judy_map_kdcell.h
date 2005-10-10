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

#ifndef _JUDY_MAP_KCELL_DCELL_H_
#define _JUDY_MAP_KCELL_DCELL_H_

#include <utility>

#include "judyarray/judy_common.h"

template <
	typename TKey,
	typename TData,
	typename THashFunc  = __judy_always_zero,
	typename TEqualFunc = std::equal_to <TKey>,
	typename TAllocator = std::allocator <std::pair <TKey, TData> > >
//<std::pair <const TKey, judy_reference <TData> > > >
class judy_map_kcell_dcell {
private:
	typedef judy_map_kcell_dcell <TKey, TData, THashFunc, TEqualFunc, TAllocator> __this_type;


	Pvoid_t              m_judy;

	// JudyL(3) tells the following:
	//     JLI() and JLD() reorganize the JudyL array.
	//     Therefore, PValue returned from previous JudyL calls
	//     become invalid and must be re-acquired.
	// This is why this why this member is necessary.
	size_t               m_insdel_count;

// types
public:
	typedef TKey                               key_type;
	typedef TData                              data_type;
	typedef TData                              mapped_type;

	typedef std::pair <const key_type, const data_type>   value_type;

	typedef size_t                    size_type;
	typedef ptrdiff_t                 difference_type;

	typedef THashFunc                 hasher;
	typedef TEqualFunc                key_equal;
	typedef TAllocator                allocator_type;

	// ???
	typedef value_type *              pointer;
	typedef const value_type *        const_pointer;

	typedef value_type                reference;
	typedef const value_type          const_reference;

// functions
public:
	judy_map_kcell_dcell ()
	{
		m_judy         = 0;
		m_insdel_count = 0;
	}

	judy_map_kcell_dcell (
		size_type n,
		const hasher&, 
		const key_equal&,
		const allocator_type&)
	{
		m_judy         = 0;
		m_insdel_count = 0;
	}

	template <class Tit>
	judy_map_kcell_dcell (
		Tit beg, Tit end,
		size_type = 0,
		const hasher& = hasher (), 
		const key_equal& = key_equal (),
		const allocator_type& = allocator_type ())
	{
		m_judy         = 0;
		m_insdel_count = 0;

		insert (beg, end);
	}

	judy_map_kcell_dcell (const __this_type& a)
	{
		m_judy         = 0;
		m_insdel_count = 0;

		insert (a.begin (), a.end ());
	}

	~judy_map_kcell_dcell ()
	{
		clear ();
		assert (m_judy == 0);
	}

	void clear ()
	{
#ifdef JUDYARRAY_DEBUG
		// Slow implementation which allows better
		// inconsistency checking
		erase (begin (), end ());
#else // JUDYARRAY_DEBUG
		// Much faster implementation

		::JudyLFreeArray (&m_judy, 0);
		m_judy = 0;
#endif // JUDYARRAY_DEBUG

		m_insdel_count = 0;
	}

	judy_map_kcell_dcell& operator = (const __this_type& a)
	{
		// exception-less implementation
		if (this != &a){
			clear ();

			__this_type temp (a);

			swap (temp);
		}

		return *this;
	}

	template <class Tit>
	void insert (Tit beg, Tit end)
	{
		while (beg != end){
			insert (*beg);

			++beg;
		}
	}

	bool empty () const
	{
		return m_judy == 0;
	}

	size_type bucket_count () const
	{
		return ::JudyLCount (m_judy, 0, -1, 0);
	}

	void swap (__this_type& a)
	{
		std::swap (m_judy, a.m_judy);
		std::swap (m_insdel_count, a.m_insdel_count);
	}

	size_type size () const
	{
		// this function works sloooowly :(
		return ::JudyLCount (m_judy, 0, -1, 0);
	}

	hasher hash_funct () const
	{
		return hasher ();
	}

	key_equal key_eq () const
	{
		return key_equal ();
	}

	size_type max_size () const
	{
		return size_type (-1);
	}

	class iterator {
	private:
		friend class __this_type;

	public:
		__JUDYARRAY_TYPEDEFS(__this_type);
		typedef std::forward_iterator_tag iterator_category;

		const __this_type *m_obj;

		PWord_t                m_pvalue;
		data_type              m_value;
		Word_t                 m_index;

		size_t                 m_insdel_count;
		bool                   m_end;

		iterator ()
		{
			m_obj    = NULL;

			m_pvalue = NULL;
			m_value  = 0;
			m_index  = 0;

			m_insdel_count = 0;
			m_end          = true;
		}

		iterator (const __this_type *obj, Word_t index, PWord_t pvalue)
		{
			assert (pvalue);

			m_obj    = obj;

			m_pvalue = pvalue;
			m_value  = *pvalue;
			m_index  = index;

			m_insdel_count = obj -> m_insdel_count;
			m_end          = false;
		}

		iterator (const __this_type *obj)
		{
			m_obj    = obj;
			m_pvalue = NULL;
			m_value  = 0;
			m_index  = 0;
			m_insdel_count = obj -> m_insdel_count;
			m_end          = false;

			m_pvalue = (PWord_t) ::JudyLFirst (m_obj -> m_judy, &m_index, 0);

			if (m_pvalue){
				m_value  = * (data_type *) m_pvalue;
			}else{
				m_end = true;
			}
		}

		iterator & operator = (const iterator& a)
		{
			if (this == &a)
				return *this;

			m_obj         = a.m_obj;
			m_pvalue      = a.m_pvalue;
			m_value       = a.m_value;
			m_index       = a.m_index;
			m_insdel_count = a.m_insdel_count;
			m_end          = a.m_end;

			return *this;
		}

		reference operator * ()
		{
			assert (!m_end);

			// object was already clear'ed
			assert (m_obj -> m_insdel_count >= m_insdel_count);

			//
			if (m_obj -> m_insdel_count == m_insdel_count){
				return value_type ((key_type) m_index, m_value);
			}else{
				m_pvalue = (PWord_t) ::JudyLGet (m_obj -> m_judy, m_index, 0);

				assert (m_pvalue); // key was already removed

				//
				m_value = *m_pvalue;
				m_insdel_count = m_obj -> m_insdel_count;

				return value_type ((key_type) m_index, m_value);
			}
		}

		const_reference operator * () const
		{
			assert (!m_end);

			// object was already clear'ed
			assert (m_obj -> m_insdel_count >= m_insdel_count);

			//
			if (m_obj -> m_insdel_count == m_insdel_count){
				return value_type ((key_type) m_index, m_value);
			}else{
				data_type *p = (data_type *) ::JudyLGet (m_obj -> m_judy, m_index, 0);

				assert (p); // key was already removed

				return value_type ((key_type) m_index, *p);
			}
		}

		iterator& operator ++ ()
		{
			assert (!m_end);

			m_pvalue = (PWord_t) ::JudyLNext (m_obj -> m_judy, &m_index, 0);

			if (m_pvalue){
				m_value = * (data_type *) m_pvalue;
			}else{
				m_end = true;
			}

			return *this;
		}

		iterator operator ++ (int)
		{
			iterator ret = *this;
			operator ++ ();
			return ret;
		}

		bool operator == (const iterator& a) const
		{
			if (m_end && a.m_end)
				return true;

			assert (!m_obj || !a.m_obj || m_obj == a.m_obj);

			if (m_end != a.m_end)
				return false;

			return m_index == a.m_index;
		}
		bool operator != (const iterator& a) const
		{
			return !operator == (a);
		}
	};
	typedef iterator const_iterator;

	void erase (key_type key)
	{
		if (JudyLDel (&m_judy, (Word_t) key, 0))
			++m_insdel_count;
	}

	void erase (iterator f, iterator l)
	{
		while (f != l){
			erase (f++);
		}
	}

	void erase (iterator it)
	{
		if (!it.m_end){
			erase ((key_type) it.m_index);
		}else{
			// Does nothing here.
			// IMHO, it is much better than "undefined behaviour"
		}
	}

	const_iterator find (key_type key) const
	{
		PWord_t p = (PWord_t) ::JudyLGet (m_judy, (Word_t) key, 0);

		if (p){
			return iterator (this, (Word_t) key, p);
		}else{
			return iterator ();
		}
	}

	iterator find (key_type key)
	{
		PWord_t p = (PWord_t) ::JudyLGet (m_judy, (Word_t) key, 0);

		if (p){
			return iterator (this, (Word_t) key, p);
		}else{
			return iterator ();
		}
	}

	size_type count (const key_type& key) const
	{
		return ::JudyLCount (m_judy, (Word_t) key, (Word_t) key, 0);
	}

	data_type &operator [] (key_type key)
	{
		PWord_t p = (PWord_t) ::JudyLIns (&m_judy, (Word_t) key, 0);
		assert (p);

		++m_insdel_count;

		return * (data_type *) p;
	}

	std::pair <iterator, bool> insert (const value_type &v)
	{
		// Unfortunately JudyLIns doesn't return value
		// which tells whether inserted value is new or not.
		// So, operator [] can work twice (approximately) faster.
		PWord_t p = (PWord_t) ::JudyLGet (m_judy, (Word_t) v.first, 0);

		if (p == NULL){
			p = (PWord_t) ::JudyLIns (&m_judy, (Word_t) v.first, 0);
			assert (p);
			*p = (Word_t) v.second;

			++m_insdel_count;

			return std::make_pair (iterator (this, (Word_t) v.first, p), true);
		}else{
			return std::make_pair (iterator (this, (Word_t) v.first, p), false);
		}
	}

public:
	iterator begin ()
	{
		return iterator (this);
	}
	const_iterator begin () const
	{
		return const_iterator (this);
	}

	iterator end ()
	{
		return iterator ();
	}
	const_iterator end () const
	{
		return const_iterator ();
	}
};

#endif // _JUDY_MAP_KCELL_DCELL_H_
