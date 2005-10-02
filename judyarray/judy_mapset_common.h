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

#ifndef _JUDY_MAPSET_COMMON_H_
#define _JUDY_MAPSET_COMMON_H_

#include <assert.h>
#include <stddef.h>

#include <list>
#include <utility>
#include <set>

#include "judy_common.h"

template <
	typename TKey,
	typename TValue,
	typename THashFunc,
	typename TEqualFunc,
	typename TAllocator,
	typename TTraits>
class __judy_mapset_base : private TTraits
{
private:
	typedef TTraits __base;

//	enum {multimap_flag = TMultimap_flag};

// types
public:
	typedef TEqualFunc                      key_equal;
	typedef THashFunc                       hasher;
	typedef TAllocator                      allocator_type;

	typedef __judy_mapset_base <TKey, TValue, THashFunc, TEqualFunc, TAllocator, TTraits> __this_type;

	__JUDYARRAY_TYPEDEFS(__base)

	struct debug_info {
		// a number of values (actually, pointer to value)
		// stored in JudyL cells
		int m_value_count;

		// a number of allocated std::list objects
		int m_list_count;

		// a number of values (actually, pointer to value)
		// stored in std::list objects
		int m_list_item_count;

		debug_info ()
		{
			m_value_count     = 0;
			m_list_count      = 0;
			m_list_item_count = 0;
		}

		debug_info (const debug_info &a)
		{
			m_value_count     = a.m_value_count;
			m_list_count      = a.m_list_count;
			m_list_item_count = a.m_list_item_count;
		}
	};

	debug_info m_debug_info;

//
private:
	typedef typename TTraits::pointers_list_type pointers_list_type;
//	typedef std::list <pointer> pointers_list_type;
	typedef std::set <pointers_list_type *> allocated_lists_type;

	Pvoid_t              m_judy;
	size_type            m_size;
	THashFunc            m_hash_func;
	TEqualFunc           m_eq_func;
	allocator_type       m_alloc;
	allocated_lists_type m_allocated_lists;

	inline pointer judy_hash_new (const value_type &v)
	{
		pointer p = m_alloc.allocate (1);
		return new (p) value_type (v);
	}

//
public:
	__judy_mapset_base ()
	{
		m_judy = 0;
		m_size = 0;
	}

	__judy_mapset_base (
		size_type n,
		const hasher& h, 
		const key_equal& k,
		const allocator_type& a)
		:
		m_hash_func (h),
		m_eq_func   (k),
		m_alloc     (a)
	{
		m_judy = 0;
		m_size = 0;
	}

	template <class Tit>
	__judy_mapset_base (
		Tit beg, Tit end,
		size_type,
		const hasher& h, 
		const key_equal& k,
		const allocator_type& a)
		:
		m_hash_func (h),
		m_eq_func   (k),
		m_alloc     (a)
	{
		m_judy = 0;
		m_size = 0;

		insert (beg, end);
	}

	__judy_mapset_base (const __this_type& a)
		:
		m_judy (0),
		m_size (0),
		m_hash_func (a.m_hash_func),
		m_eq_func   (a.m_eq_func),
		m_alloc     (a.m_alloc)
	{
		// optimize me!!!
		insert (a.begin (), a.end ());
	}

	~__judy_mapset_base ()
	{
		clear ();

		assert (!m_debug_info.m_value_count);
		assert (!m_debug_info.m_list_count);
		assert (!m_debug_info.m_list_item_count);
		assert (!m_judy);
		assert (m_allocated_lists.empty ());
	}

	void clear ()
	{
#ifdef JUDYARRAY_DEBUG
		// Slow implementation which allows better
		// inconsistency checking
		erase (begin (), end ());
#else // JUDYARRAY_DEBUG
		// Much faster implementation
		typename allocated_lists_type::iterator f, l;

		f = m_allocated_lists.begin ();
		l = m_allocated_lists.end ();

#ifdef JUDYARRAY_DEBUGINFO
		m_debug_info.m_value_count = 0;
#endif

		for (; f != l; ++f){
#ifdef JUDYARRAY_DEBUGINFO
			m_debug_info.m_list_count -= 1;
			m_debug_info.m_list_item_count -= (*f) -> size ();
#endif
			delete *f;
		}

		m_size = 0;

		m_allocated_lists.clear ();
		::JudyLFreeArray (&m_judy, 0);
#endif // JUDYARRAY_DEBUG
	}

	__judy_mapset_base& operator = (const __this_type& a)
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
		return m_size == 0;
	}

	size_type bucket_count () const
	{
		return m_size;
	}

	void swap (__this_type& a)
	{
		std::swap (m_judy, a.m_judy);
		std::swap (m_size, a.m_size);
		std::swap (m_hash_func, a.m_hash_func);
		std::swap (m_debug_info, a.m_debug_info);
		std::swap (m_alloc, a.m_alloc);
		std::swap (m_allocated_lists, a.m_allocated_lists);
	}

	size_type size () const
	{
		return m_size;
	}

	hasher hash_funct () const
	{
		return m_hash_func;
	}

	key_equal key_eq () const
	{
		return m_eq_func;
	}

	size_type max_size () const
	{
		return size_type (-1);
	}

private:
	union judyarray_union_type {
		Word_t        m_judy_int;
		PWord_t       m_judy_ptr;
		pointer       m_pointer;
		pointers_list_type   *m_list;
	};

	class iterator_base : private TTraits {
	public:
		__JUDYARRAY_TYPEDEFS(TTraits)

//	private:
		const __this_type *m_obj;

		Word_t                 m_index;
		judyarray_union_type    m_value;
		bool                   m_end;
		bool                   m_inside_list;

		typename pointers_list_type::iterator m_list_it;

		void init_list_it ()
		{
			m_inside_list = false;

			if (m_value.m_judy_int & 1){
				pointers_list_type *lst
					= (pointers_list_type *) (m_value.m_judy_int & ~1);

				m_inside_list   = true;
				m_list_it       = lst -> begin ();

				assert (m_list_it != lst -> end ());
			}
		}

		void init ()
		{
			m_obj              = NULL;
			m_index            = 0;
			m_value.m_judy_int = 0;
			m_end              = true;
			m_inside_list      = false;
		}

		void make_end ()
		{
			m_end = true;
		}

//	protected:
		reference at () const
		{
			if (m_inside_list){
				return * (*m_list_it);
			}else{
				return * m_value.m_pointer;
			}
		}

//	public:
		iterator_base ()
		{
			init ();
		}

		iterator_base (const iterator_base &a)
		{
			init ();
			operator = (a);
		}

		iterator_base (const __this_type *obj,
					   Word_t index, Word_t value)
			:
			m_obj (obj),
			m_index (index),
			m_end (false)
		{
			m_value.m_judy_int = value;
			init_list_it ();
		}

		iterator_base (const __this_type *obj,
		               Word_t index, Word_t value,
		               typename pointers_list_type::iterator it)
			:
			m_obj (obj),
			m_index (index),
			m_end (false),
			m_inside_list (true),
			m_list_it (it)
		{
			m_value.m_judy_int = value;
		}

		iterator_base (const __this_type *obj)
		{
			init ();

			m_end = false;

			m_obj = obj;

			m_value.m_judy_ptr
				= (PWord_t) JudyLFirst(m_obj -> m_judy, &m_index, 0);

			if (m_value.m_judy_ptr){
				m_value.m_judy_int = *m_value.m_judy_ptr;
				init_list_it ();
			}else{
				m_end = true;
			}
		}

		iterator_base & operator = (const iterator_base& a)
		{
			if (this == &a)
				return *this;

			m_index       = a.m_index;
			m_value       = a.m_value;
			m_obj         = a.m_obj;
			m_end         = a.m_end;
			m_inside_list = a.m_inside_list;
			m_list_it     = a.m_list_it;

			return *this;
		}

		reference operator * ()
		{
			return at ();
		}
		const_reference operator * () const
		{
			return at ();
		}

		iterator_base& operator ++ ()
		{
			if (m_end)
				abort ();

			bool goto_next_judy_cell = false;

			if (m_inside_list){
				assert ((m_value.m_judy_int & 1) == 1);

				pointers_list_type *lst
					= (pointers_list_type *) (m_value.m_judy_int & ~1);

				goto_next_judy_cell = (lst -> end () == ++m_list_it);
			}else{
				goto_next_judy_cell = true;
			}

			if (goto_next_judy_cell){
				m_value.m_judy_ptr
					= (PWord_t) JudyLNext (m_obj -> m_judy, &m_index, 0);

				if (m_value.m_judy_ptr){
					m_value.m_judy_int = *m_value.m_judy_ptr;
					init_list_it ();
				}else{
					m_end = true;
				}
			}

			return *this;
		}

		bool operator == (const iterator_base& i) const
		{
			if (this == &i)
				return true;

			if (m_end != i.m_end)
				return false;

			if (m_end && i.m_end)
				return true;

			if (m_obj != i.m_obj)
				return false;

			return (m_index       == i.m_index)
				&& (m_inside_list == i.m_inside_list)
				&& (m_list_it     == i.m_list_it);
		}
	};

public:
	class const_iterator;

	class iterator {
	private:
		iterator_base m_it;
		friend class const_iterator;
		friend class __judy_mapset_base;

	public:
		__JUDYARRAY_TYPEDEFS(iterator_base)
		typedef std::forward_iterator_tag iterator_category;

		iterator ()
		{
		}
		iterator (const iterator_base &a)
			: m_it (a)
		{
		}
		iterator (const iterator &a)
			: m_it (a.m_it)
		{
		}
		iterator (const __this_type *obj)
			: m_it (obj)
		{
		}
		iterator & operator = (const iterator& a)
		{
			m_it = a.m_it;
			return *this;
		}
		reference operator * ()
		{
			return m_it.operator * ();
		}
		const_reference operator * () const
		{
			return m_it.operator * ();
		}
		iterator operator ++ (int)
		{
			iterator ret = *this;
			operator ++ ();
			return ret;
		}
		iterator& operator ++ ()
		{
			m_it.operator ++ ();
			return *this;
		}
		bool operator == (const iterator& a) const
		{
			return m_it == a.m_it;
		}
		bool operator != (const iterator& a) const
		{
			return !(m_it == a.m_it);
		}
		pointer operator -> ()
		{
			return &m_it.operator * ();
		}
		const_pointer operator -> () const
		{
			return &m_it.operator * ();
		}
	};

	class const_iterator {
	private:
		iterator_base m_it;
		friend class __judy_mapset_base;

	public:
		__JUDYARRAY_TYPEDEFS(iterator_base)
		typedef std::forward_iterator_tag iterator_category;

		const_iterator ()
		{
		}
		const_iterator (const iterator_base &a)
			: m_it (a)
		{
		}
		const_iterator (const const_iterator &a)
			: m_it (a.m_it)
		{
		}
		const_iterator (const iterator &a)
			: m_it (a.m_it)
		{
		}
		const_iterator (const __this_type *obj)
			: m_it (obj)
		{
		}
		const_iterator & operator = (const const_iterator& a)
		{
			m_it = a.m_it;
			return *this;
		}
		reference operator * ()
		{
			return m_it.operator * ();
		}
		const_reference operator * () const
		{
			return m_it.operator * ();
		}
		const_iterator operator ++ (int)
		{
			const_iterator ret = *this;
			operator ++ ();
			return ret;
		}
		const_iterator& operator ++ ()
		{
			m_it.operator ++ ();
			return *this;
		}
		bool operator == (const const_iterator& a) const
		{
			return m_it == a.m_it;
		}
		bool operator != (const const_iterator& a) const
		{
			return !(m_it == a.m_it);
		}
		pointer operator -> ()
		{
			return &m_it.operator * ();
		}
		const_pointer operator -> () const
		{
			return &m_it.operator * ();
		}
	};

	void erase (const key_type& key)
	{
		iterator e (find (key));
		if (e != end ())
			erase (e);
	}

	void erase (iterator f, iterator l)
	{
		while (f != l){
			erase (f++);
		}
	}

	void erase (iterator it)
	{
		if (it.m_it.m_end){
			// standard says about undefined behaviour in such situations :(
			abort ();

			// I assume, 'return' is much better
			//return;
		}

		assert (this == it.m_it.m_obj);

		if (it.m_it.m_inside_list){
			assert ((it.m_it.m_value.m_judy_int & 1) == 1);

			m_size -= 1;

			m_alloc.deallocate (*it.m_it.m_list_it, 1);
#ifdef JUDYARRAY_DEBUGINFO
			m_debug_info.m_list_item_count -= 1;
#endif

			pointers_list_type *lst
				= (pointers_list_type *) (it.m_it.m_value.m_judy_int & ~1);

			lst -> erase (it.m_it.m_list_it);

			if (lst -> empty ()){
				delete lst;
				m_allocated_lists.erase (lst);

				::JudyLDel (&m_judy, it.m_it.m_index, 0);
#ifdef JUDYARRAY_DEBUGINFO
				m_debug_info.m_list_count -= 1;
#endif
			}
		}else{
			assert ((it.m_it.m_value.m_judy_int & 1) == 0);

			m_size -= 1;

			m_alloc.deallocate (it.m_it.m_value.m_pointer, 1);
#ifdef JUDYARRAY_DEBUGINFO
			m_debug_info.m_value_count -= 1;
#endif

			::JudyLDel (&m_judy, it.m_it.m_index, 0);
		}
	}

private:
	iterator find_base (const key_type& key) const
	{
		unsigned long h = m_hash_func (key);
		judyarray_union_type *ptr
			= (judyarray_union_type *) ::JudyLGet (m_judy, h, 0);

		if (!ptr || !ptr -> m_judy_int){
			return iterator ();
		}else{
			judyarray_union_type value;
			value.m_judy_int = ptr -> m_judy_int;

			if ((value.m_judy_int & 1) == 0){
				if (m_eq_func (value2key (*value.m_pointer), key)){
					return iterator (iterator_base (this, h, value.m_judy_int));
				}else{
					iterator ret (iterator_base (this, h, value.m_judy_int));
					ret.m_it.make_end ();
					return ret;
				}
			}else{
				pointers_list_type *lst
					= (pointers_list_type *) (value.m_judy_int & ~1);
				typename pointers_list_type::iterator end
					= lst -> end ();

#if 1
				typename pointers_list_type::iterator found
					= lst -> find (key);
				if (found != lst -> end ()){
					return iterator (iterator_base
									 (this, h, value.m_judy_int, found));
				}
#else
				typename pointers_list_type::iterator beg;
				beg = lst -> begin ();

				for (; !(beg == end); ++beg){
					if (m_eq_func (value2key (**beg), key)){
						return iterator (iterator_base
										 (this, h, value.m_judy_int, beg));
					}
				}
#endif

				iterator ret (iterator_base (this, h, value.m_judy_int, end));
				ret.m_it.make_end ();
				return ret;
			}
		}
	}

public:
	const_iterator find (const key_type& key) const
	{
		return find_base (key);
	}

	iterator find (const key_type& key)
	{
		return find_base (key);
	}

	size_type count (const key_type& key) const
	{
		const_iterator e = end ();
		size_type c = 0;

		// optimize me!!!
		for (
			const_iterator found = find (key);
			!(found == e) && m_eq_func (value2key (*found), key);
			++found)
		{
			++c;
		}

		return c;
	}

	std::pair <iterator, bool> insert (const value_type& value)
	{
		const TKey &key = value2key (value);

		Word_t h = m_hash_func (key);
		judyarray_union_type *ptr
			= (judyarray_union_type *) ::JudyLIns (&m_judy, h, 0);

		assert (ptr);

		if (ptr -> m_judy_int){
			// JudyL cell was already initialized
			if ((ptr -> m_judy_int & 1) == 0){
				if (m_eq_func (
						value2key (*ptr -> m_pointer), key))
				{
					// JudyL cell equal to 'value'
					return std::make_pair
						(iterator (iterator_base (this, h, ptr -> m_judy_int)),
						 false);
				}else{
					// collision is detected.
					// We create list object here
					pointer copy = ptr -> m_pointer;
					pointers_list_type *lst
						= ptr -> m_list = new pointers_list_type;

					m_allocated_lists.insert (lst);

#ifdef JUDYARRAY_DEBUGINFO
					m_debug_info.m_list_count       += 1;
					m_debug_info.m_list_item_count  += 2;
					m_debug_info.m_value_count      -= 1;
#endif

					lst -> insert (lst -> end (), copy);

					typename pointers_list_type::iterator ret_it
						= lst -> insert (
							lst -> end (), judy_hash_new (value));

					++m_size;

					ptr -> m_judy_int |= 1;

					return std::make_pair (
						iterator (iterator_base (
									  this, h, ptr -> m_judy_int, ret_it)),
						true);
				}
			}else{
				// Collision is detected.
				// List object is already created.
				pointers_list_type *lst
					= (pointers_list_type *) (ptr -> m_judy_int & ~1);

				typename pointers_list_type::iterator beg, end;
				beg = lst -> begin ();
				end = lst -> end ();

				// Look for 'value' in the list
				for (; !(beg == end); ++beg){
					if (m_eq_func (value2key (**beg), key)){
						// found
						return std::make_pair (
							iterator (iterator_base (
										  this, h, ptr -> m_judy_int, beg)),
							false);
					}
				}

				++m_size;

#ifdef JUDYARRAY_DEBUGINFO
				m_debug_info.m_list_item_count += 1;
#endif

				// Add new 'value' to the list
				return std::make_pair (
					iterator (iterator_base (
								  this, h, ptr -> m_judy_int,
								  lst -> insert (lst -> end (),
												 judy_hash_new (value)))),
					true);
			}
		}else{
			// Created JudyL cell is new one.

#ifdef JUDYARRAY_DEBUGINFO
			m_debug_info.m_value_count += 1;
#endif

			ptr -> m_pointer = judy_hash_new (value);

			++m_size;

			return std::make_pair (
				iterator (iterator_base (this, h, ptr -> m_judy_int)),
				true);
		}
	}

	iterator begin ()
	{
		return iterator (iterator_base (this));
	}
	const_iterator begin () const
	{
		return const_iterator (iterator_base (this));
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

// Unfortunately the following code doesn't work.
//            template <typename T>
//            typedef std::pair <T1, char> pair_first_is_char;
// This is why this loooong macros is used here.

#define REMAP_FUNCALLS(macrosarg__base, macrosarg__class_name, macrosarg__to_member)\
		typedef typename macrosarg__base::key_equal      key_equal;\
		typedef typename macrosarg__base::hasher         hasher;\
		typedef typename macrosarg__base::allocator_type allocator_type;\
	\
		typedef typename macrosarg__base::iterator       iterator;\
		typedef typename macrosarg__base::const_iterator const_iterator;\
	\
		typedef typename macrosarg__base::debug_info     debug_info;\
	\
		macrosarg__class_name ()\
		{\
		}\
	\
		macrosarg__class_name (\
			size_type n,\
			const hasher& h         = hasher (), \
			const key_equal& k      = key_equal (),\
			const allocator_type& a = allocator_type ())\
			:\
			macrosarg__to_member (n, h, k, a)\
		{\
		}\
	\
		template <class Tit>\
		macrosarg__class_name (\
			Tit beg, Tit end,\
			size_type n             = 0,\
			const hasher& h         = hasher (), \
			const key_equal& k      = key_equal (),\
			const allocator_type& a = allocator_type ())\
			:\
			macrosarg__to_member (beg, end, n, h, k, a)\
		{\
		}\
	\
		macrosarg__class_name (const __this_type& a)\
			:\
			macrosarg__to_member (a.macrosarg__to_member)\
		{\
		}\
	\
		~macrosarg__class_name ()\
		{\
		}\
	\
		void clear ()\
		{\
			macrosarg__to_member.clear ();\
		}\
	\
		std::pair <iterator, bool> insert (const value_type& value)\
		{\
			return macrosarg__to_member.insert (value);\
		}\
	\
		template <class Tit>\
		void insert (Tit beg, Tit end)\
		{\
			while (beg != end){\
				macrosarg__to_member.insert (*beg);\
	\
				++beg;\
			}\
		}\
	\
		bool empty () const\
		{\
			return macrosarg__to_member.empty ();\
		}\
	\
		size_type bucket_count () const\
		{\
			return macrosarg__to_member.bucket_count ();\
		}\
	\
		void swap (__this_type& a)\
		{\
			macrosarg__to_member.swap (a.macrosarg__to_member);\
		}\
	\
		size_type size () const\
		{\
			return macrosarg__to_member.size ();\
		}\
	\
		hasher hash_funct () const\
		{\
			return macrosarg__to_member.hash_funct ();\
		}\
	\
		key_equal key_eq () const\
		{\
			return macrosarg__to_member.key_eq ();\
		}\
	\
		void resize (size_type n)\
		{\
		}\
	\
		size_type max_size () const\
		{\
			return macrosarg__to_member.max_size ();\
		}\
	\
		void erase (const key_type& key)\
		{\
			macrosarg__to_member.erase (key);\
		}\
	\
		void erase (iterator f, iterator l)\
		{\
			macrosarg__to_member.erase (f, l);\
		}\
	\
		void erase (iterator it)\
		{\
			macrosarg__to_member.erase (it);\
		}\
	\
		const_iterator find (const key_type& key) const\
		{\
			return macrosarg__to_member.find (key);\
		}\
		iterator find (const key_type& key)\
		{\
			return macrosarg__to_member.find (key);\
		}\
	\
		size_type count (const key_type& key) const\
		{\
			return macrosarg__to_member.count (key);\
		}\
	\
		iterator begin ()\
		{\
			return macrosarg__to_member.begin ();\
		}\
		const_iterator begin () const\
		{\
			return macrosarg__to_member.begin ();\
		}\
	\
		iterator end ()\
		{\
			return macrosarg__to_member.end ();\
		}\
		const_iterator end () const\
		{\
			return macrosarg__to_member.end ();\
		}\
	\
		const debug_info &get_debug_info () const\
		{\
			return macrosarg__to_member.m_debug_info;\
		}

#endif // _JUDY_MAPSET_COMMON_H_
