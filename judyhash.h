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

#include <assert.h>

#include <list>
#include <utility>
#include <set>

#include "Judy.h"

////////////////////////////////////////////////////////////
///  defining JUDYHASH_DEBUGINFO
#ifdef JUDYHASH_NO_DEBUGINFO

#ifdef JUDYHASH_DEBUGINFO
#undef JUDYHASH_DEBUGINFO
#endif // JUDYHASH_DEBUGINFO

#else

#if !defined(JUDYHASH_DEBUGINFO) && !defined(NDEBUG)
#define JUDYHASH_DEBUGINFO
#endif // JUDYHASH_DEBUGINFO && NDEBUG

#endif // JUDYHASH_NO_DEBUGINFO
////////////////////////////////////////////////////////////

template <typename TKey, typename TValue>
struct __judyhash_types {
	typedef TKey                            _key_type;
	typedef TValue                          _data_type;
	typedef TValue                          _mapped_type;
	typedef std::pair <const TKey, TValue>  _value_type;
	typedef size_t                          _size_type;
	typedef ptrdiff_t                       _difference_type;

	// It is not possible to derive 'pointer' and 'reference' types from
	// TAllocator
	typedef _value_type                   * _pointer;
	typedef _value_type             const * _const_pointer;
	typedef _value_type                   & _reference;
	typedef _value_type             const & _const_reference;
};

#define __JUDYHASH_TYPEDEFS                                               \
	typedef typename __base::_key_type                key_type;           \
	typedef typename __base::_data_type               data_type;          \
	typedef typename __base::_mapped_type             mapped_type;        \
	typedef typename __base::_value_type              value_type;         \
	typedef typename __base::_size_type               size_type;          \
	typedef typename __base::_difference_type         difference_type;    \
    \
	typedef typename __base::_pointer                 pointer;            \
	typedef typename __base::_const_pointer           const_pointer;      \
	typedef typename __base::_reference               reference;          \
	typedef typename __base::_const_reference         const_reference;

    // It is not possible to derive 'pointer' and 'reference' types from  \
    // TAllocator                                                         \

template <
	typename TKey,
	typename TValue,
	typename THashFunc /* = std::hash<Key>*/,
	typename TEqualFunc = std::equal_to <TKey>,
	typename TAllocator = std::allocator< std::pair < TKey const, TValue > > >
class judyhash_map : private __judyhash_types <TKey, TValue> {
private:
	typedef __judyhash_types <TKey, TValue> __base;

// types
public:
	typedef TEqualFunc                      key_equal;
	typedef THashFunc                       hasher;
	typedef TAllocator                      allocator_type;

	__JUDYHASH_TYPEDEFS

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
	typedef std::list <pointer> pointers_list_type;
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
	judyhash_map (
		size_type n             = 0,
		const hasher& h         = hasher (), 
		const key_equal& k      = key_equal (),
		const allocator_type& a = allocator_type ())
		:
		m_hash_func (h),
		m_eq_func   (k),
		m_alloc     (a)
	{
		m_judy = 0;
		m_size = 0;
	}

	template <class Tit>
	judyhash_map (
		Tit beg, Tit end,
		size_type               = 0,
		const hasher& h         = hasher (), 
		const key_equal& k      = key_equal (),
		const allocator_type& a = allocator_type ())
		:
		m_hash_func (h),
		m_eq_func   (k),
		m_alloc     (a)
	{
		m_judy = 0;
		m_size = 0;

		insert (beg, end);
	}

	judyhash_map (const judyhash_map& a)
		:
		m_judy (0),
		m_size (0),
		m_hash_func (a.m_hash_func),
		m_eq_func   (a.m_eq_func),
		m_alloc     (a.m_alloc)
	{
		operator = (a);
	}

	~judyhash_map ()
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
#ifdef JUDYHASH_DEBUG
		// Slow implementation which allows better
		// inconsistency checking
		erase (begin (), end ());
#else // JUDYHASH_DEBUG
		// Much faster implementation
		typename allocated_lists_type::iterator f, l;

		f = m_allocated_lists.begin ();
		l = m_allocated_lists.end ();

#ifdef JUDYHASH_DEBUGINFO
		m_debug_info.m_value_count = 0;
#endif

		for (; f != l; ++f){
#ifdef JUDYHASH_DEBUGINFO
			m_debug_info.m_list_count -= 1;
			m_debug_info.m_list_item_count -= (*f) -> size ();
#endif
			delete *f;
		}

		m_size = 0;

		m_allocated_lists.clear ();
		::JudyLFreeArray (&m_judy, 0);
#endif // JUDYHASH_DEBUG
	}

	judyhash_map& operator = (const judyhash_map& a)
	{
		if (this != &a){
			clear ();

			// optimize me!!!
			insert (a.begin (), a.end ());
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

	void swap (judyhash_map& a)
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

	void resize (size_type n)
	{
		// does nothing
	}

	size_type max_size () const
	{
		return size_type (-1);
	}

private:
	union judyhash_union_type {
		Word_t        m_judy_int;
		PWord_t       m_judy_ptr;
		pointer       m_pointer;
		pointers_list_type   *m_list;
	};

public:
	class iterator_base : public __judyhash_types <TKey, TValue> {
	public:
		typedef __judyhash_types <TKey, TValue> __base;
		typedef std::forward_iterator_tag iterator_category;

		__JUDYHASH_TYPEDEFS

	private:
		const judyhash_map    *m_obj;

		Word_t                 m_index;
		judyhash_union_type    m_value;
		bool                   m_end;
		bool                   m_inside_list;

		typename pointers_list_type::iterator m_list_it;
//		typename pointers_list_type::iterator m_list_end_it;

		friend class judyhash_map;

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

	protected:
		reference at () const
		{
			if (m_inside_list){
				return * (*m_list_it);
			}else{
				return * m_value.m_pointer;
			}
		}

	public:
		iterator_base ()
		{
			init ();
		}

		iterator_base (const iterator_base &a)
		{
			init ();
			operator = (a);
		}

		iterator_base (const judyhash_map *obj, Word_t index, Word_t value)
			:
			m_obj (obj),
			m_index (index),
			m_end (false)
		{
			m_value.m_judy_int = value;
			init_list_it ();
		}

		iterator_base (const judyhash_map *obj,
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

		iterator_base (const judyhash_map *obj)
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

		bool operator != (const iterator_base& i) const
		{
			return ! (*this == i);
		}
	};

	class const_iterator;

	class iterator : public iterator_base {
	private:
		// prevent conversion from 'const_iterator' to 'iterator'
		iterator (const const_iterator &a)
		{
			abort ();
		}

	public:
		iterator ()
		{
		}
		iterator (const iterator &a)
			: iterator_base (a)
		{
		}
		explicit iterator (const iterator_base &a)
			: iterator_base (a)
		{
		}
		iterator & operator = (const iterator& a)
		{
			return (iterator &) iterator_base::operator = (a);
		}
		iterator operator ++ (int)
		{
			iterator ret = *this;
			operator ++ ();
			return ret;
		}
		iterator& operator ++ ()
		{
			return (iterator &) iterator_base::operator++ ();
		}
		bool operator == (const iterator& i) const
		{
			return iterator_base::operator == (i);
		}
		pointer operator -> () const
		{
			return &(at ());
		}
	};

	class const_iterator : public iterator_base {
	public:
		const_iterator ()
		{
		}
		const_iterator (const const_iterator &a)
			: iterator_base (a)
		{
		}
		const_iterator (const iterator &a)
			: iterator_base (a)
		{
		}
		explicit const_iterator (const iterator_base &a)
			: iterator_base (a)
		{
		}
		const_iterator & operator = (const const_iterator& a)
		{
			return (const_iterator &) iterator_base::operator = (a);
		}
		const_iterator & operator = (const iterator& a)
		{
			return (const_iterator &) iterator_base::operator = (a);
		}
		const_iterator operator ++ (int)
		{
			const_iterator ret = *this;
			operator ++ ();
			return ret;
		}
		const_iterator& operator ++ ()
		{
			return (const_iterator &) iterator_base::operator++ ();
		}
		bool operator == (const const_iterator& i) const
		{
			return iterator_base::operator == (i);
		}
		const_pointer operator -> () const
		{
			return &(at ());
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
		if (it.m_end){
			// standard says about undefined behaviour in such situations :(
			abort ();

			// I assume, 'return' is much better
			//return;
		}

		assert (this == it.m_obj);

		if (it.m_inside_list){
			assert ((it.m_value.m_judy_int & 1) == 1);

			m_size -= 1;

			m_alloc.deallocate (*it.m_list_it, 1);
#ifdef JUDYHASH_DEBUGINFO
			m_debug_info.m_list_item_count -= 1;
#endif

			pointers_list_type *lst
				= (pointers_list_type *) (it.m_value.m_judy_int & ~1);

			lst -> erase (it.m_list_it);

			if (lst -> empty ()){
				delete lst;
				m_allocated_lists.erase (lst);

				::JudyLDel (&m_judy, it.m_index, 0);
#ifdef JUDYHASH_DEBUGINFO
				m_debug_info.m_list_count -= 1;
#endif
			}
		}else{
			assert ((it.m_value.m_judy_int & 1) == 0);

			m_size -= 1;

			m_alloc.deallocate (it.m_value.m_pointer, 1);
#ifdef JUDYHASH_DEBUGINFO
			m_debug_info.m_value_count -= 1;
#endif

			::JudyLDel (&m_judy, it.m_index, 0);
		}
	}

private:
	iterator find_base (const key_type& key) const
	{
		unsigned long h = m_hash_func (key);
		judyhash_union_type *ptr
			= (judyhash_union_type *) ::JudyLGet (m_judy, h, 0);

		if (!ptr || !ptr -> m_judy_int){
			return iterator ();
		}else{
			judyhash_union_type value;
			value.m_judy_int = ptr -> m_judy_int;

			if ((value.m_judy_int & 1) == 0){
				if (m_eq_func (value.m_pointer -> first, key)){
					return iterator (iterator_base (this, h, value.m_judy_int));
				}else{
					iterator ret (iterator_base (this, h, value.m_judy_int));
					ret.make_end ();
					return ret;
				}
			}else{
				pointers_list_type *lst
					= (pointers_list_type *) (value.m_judy_int & ~1);

				typename pointers_list_type::iterator beg, end;
				beg = lst -> begin ();
				end = lst -> end ();

				typename pointers_list_type::iterator found = end;

				for (; !(beg == end); ++beg){
					if (m_eq_func ((*beg) -> first, key)){
						return iterator (iterator_base
										 (this, h, value.m_judy_int, beg));
					}
				}

				iterator ret (iterator_base (this, h, value.m_judy_int, end));
				ret.make_end ();
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
			!(found == e) && m_eq_func ((*found).first, key);
			++found)
		{
			++c;
		}

		return c;
	}

	mapped_type& operator [] (const key_type& key)
	{
		std::pair <iterator, bool> res = insert (
			value_type (key, mapped_type ()));

		return res.first -> second;
	}

	std::pair <iterator, bool> insert (const value_type& value)
	{
		const TKey &key   = value.first;

		Word_t h = m_hash_func (key);
		judyhash_union_type *ptr
			= (judyhash_union_type *) ::JudyLIns (&m_judy, h, 0);

		assert (ptr);

		if (ptr -> m_judy_int){
			// JudyL cell was already initialized
			if ((ptr -> m_judy_int & 1) == 0){
				if (m_eq_func (
						ptr -> m_pointer -> first, value.first))
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

#ifdef JUDYHASH_DEBUGINFO
					m_debug_info.m_list_count       += 1;
					m_debug_info.m_list_item_count  += 2;
					m_debug_info.m_value_count      -= 1;
#endif

					lst -> insert (
						lst -> end (), copy);

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

				typename pointers_list_type::iterator found = end;

				// Look for 'value' in the list
				for (; !(beg == end); ++beg){
					if (m_eq_func ((*beg) -> first, value.first)){
						// found
						return std::make_pair (
							iterator (iterator_base (
										  this, h, ptr -> m_judy_int, beg)),
							false);
					}
				}

				++m_size;

#ifdef JUDYHASH_DEBUGINFO
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

#ifdef JUDYHASH_DEBUGINFO
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
