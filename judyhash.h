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
#include <stddef.h>

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
struct __judyhash_map_traits {
	typedef TKey                            key_type;
	typedef TValue                          data_type;
	typedef TValue                          mapped_type;
#ifdef JUDYHASH_NO_CONST
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

	const key_type &value2key (const value_type& value) const
	{
		return value.first;
	}
};

template <typename TKey>
struct __judyhash_set_traits {
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

	const key_type &value2key (const value_type& value) const
	{
		return value;
	}
};

#define __JUDYHASH_TYPEDEFS                                               \
	typedef typename __base::key_type                key_type;           \
	typedef typename __base::data_type               data_type;          \
	typedef typename __base::mapped_type             mapped_type;        \
	typedef typename __base::value_type              value_type;         \
	typedef typename __base::size_type               size_type;          \
	typedef typename __base::difference_type         difference_type;    \
    \
	typedef typename __base::pointer                 pointer;            \
	typedef typename __base::const_pointer           const_pointer;      \
	typedef typename __base::reference               reference;          \
	typedef typename __base::const_reference         const_reference;

    // It is not possible to derive 'pointer' and 'reference' types from  \
    // TAllocator                                                         \

template <typename TKey, typename TEqualFunc>
class __judyhash_list_map : public std::list <TKey *>
{
private:
	TEqualFunc m_eq_func;
public:
	__judyhash_list_map ()
	{
	}
	~__judyhash_list_map ()
	{
	}
	typename std::list <TKey *>::iterator find (
		const typename TKey::first_type &key)
	{
		typename std::list <TKey *>::iterator beg
			= std::list <TKey *>::begin ();
		typename std::list <TKey *>::iterator end
			= std::list <TKey *>::end ();

		for (; !(beg == end); ++beg){
			if (m_eq_func ((**beg).first, key)){
				return beg;
			}
		}
	}
};

template <typename TKey, typename TEqualFunc>
class __judyhash_list_set : public std::list <TKey *>
{
private:
	TEqualFunc m_eq_func;
public:
	__judyhash_list_set ()
	{
	}
	~__judyhash_list_set ()
	{
	}
	typename std::list <TKey *>::iterator find (const TKey &key)
	{
		typename std::list <TKey *>::iterator beg
			= std::list <TKey *>::begin ();
		typename std::list <TKey *>::iterator end
			= std::list <TKey *>::end ();

		for (; !(beg == end); ++beg){
			if (m_eq_func (**beg, key)){
				return beg;
			}
		}
	}
};

template <
	typename TKey,
	typename TValue,
	typename THashFunc,
	typename TEqualFunc,
	typename TAllocator,
	typename TList,
	typename TTraits>
class __judyhash_map : private TTraits
{
private:
	typedef TTraits __base;

//	enum {multimap_flag = TMultimap_flag};

// types
public:
	typedef TEqualFunc                      key_equal;
	typedef THashFunc                       hasher;
	typedef TAllocator                      allocator_type;

	typedef __judyhash_map <TKey, TValue, THashFunc, TEqualFunc, TAllocator, TList, TTraits> __this_type;

	__JUDYHASH_TYPEDEFS

	struct debug_info {
		// a number of values (actually, pointer to value)
		// stored in JudyL cells
		int m_value_count;

		// a number of allocated TList objects
		int m_list_count;

		// a number of values (actually, pointer to value)
		// stored in TList objects
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
	typedef TList pointers_list_type;
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
	__judyhash_map ()
	{
		m_judy = 0;
		m_size = 0;
	}

	__judyhash_map (
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
	__judyhash_map (
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

	__judyhash_map (const __this_type& a)
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

	~__judyhash_map ()
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

	__judyhash_map& operator = (const __this_type& a)
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

	class iterator_base : private TTraits {
	public:
		__JUDYHASH_TYPEDEFS

//	private:
		const __this_type *m_obj;

		Word_t                 m_index;
		judyhash_union_type    m_value;
		bool                   m_end;
		bool                   m_inside_list;

		typename pointers_list_type::iterator m_list_it;
//		typename pointers_list_type::iterator m_list_end_it;

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

	class iterator : private TTraits {
	private:
		iterator_base m_it;
		friend class const_iterator;
		friend class __judyhash_map;

	public:
		__JUDYHASH_TYPEDEFS

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

	class const_iterator : private TTraits {
	private:
		iterator_base m_it;
		friend class __judyhash_map;

	public:
		__JUDYHASH_TYPEDEFS

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
#ifdef JUDYHASH_DEBUGINFO
			m_debug_info.m_list_item_count -= 1;
#endif

			pointers_list_type *lst
				= (pointers_list_type *) (it.m_it.m_value.m_judy_int & ~1);

			lst -> erase (it.m_it.m_list_it);

			if (lst -> empty ()){
				delete lst;
				m_allocated_lists.erase (lst);

				::JudyLDel (&m_judy, it.m_it.m_index, 0);
#ifdef JUDYHASH_DEBUGINFO
				m_debug_info.m_list_count -= 1;
#endif
			}
		}else{
			assert ((it.m_it.m_value.m_judy_int & 1) == 0);

			m_size -= 1;

			m_alloc.deallocate (it.m_it.m_value.m_pointer, 1);
#ifdef JUDYHASH_DEBUGINFO
			m_debug_info.m_value_count -= 1;
#endif

			::JudyLDel (&m_judy, it.m_it.m_index, 0);
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

	mapped_type& operator [] (const key_type& key)
	{
		std::pair <iterator, bool> res = insert (
			value_type (key, mapped_type ()));

		return res.first -> second;
	}

	std::pair <iterator, bool> insert (const value_type& value)
	{
		const TKey &key = value2key (value);

		Word_t h = m_hash_func (key);
		judyhash_union_type *ptr
			= (judyhash_union_type *) ::JudyLIns (&m_judy, h, 0);

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

#ifdef JUDYHASH_DEBUGINFO
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

template <
	typename TKey,
	typename TValue,
	typename THashFunc, // = std::hash <Key>,
	typename TEqualFunc = std::equal_to <TKey>,
	typename TAllocator = std::allocator < typename __judyhash_map_traits <TKey, TValue>::value_type > >
class judyhash_map
:
public __judyhash_map <
	TKey, TValue, THashFunc, TEqualFunc, TAllocator,
	__judyhash_list_map
		<typename __judyhash_map_traits <TKey, TValue>::value_type, TEqualFunc>,
	__judyhash_map_traits <TKey, TValue> >
{
private:
	typedef __judyhash_map <
		TKey, TValue, THashFunc, TEqualFunc, TAllocator,
		__judyhash_list_map
			<typename __judyhash_map_traits <TKey, TValue>::value_type, TEqualFunc>,
		__judyhash_map_traits <TKey, TValue> > __base;
	typedef judyhash_map <
		TKey, TValue, THashFunc, TEqualFunc, TAllocator
		> __this_type;

public:
	typedef typename __base::key_equal               key_equal;
	typedef typename __base::hasher                  hasher;
	typedef typename __base::allocator_type          allocator_type;

	__JUDYHASH_TYPEDEFS

	judyhash_map ()
	{
	}

	judyhash_map (
		size_type n,
		const hasher& h         = hasher (), 
		const key_equal& k      = key_equal (),
		const allocator_type& a = allocator_type ())
		:
		__base (n, h, k, a)
	{
	}

	template <class Tit>
	judyhash_map (
		Tit beg, Tit end,
		size_type n             = 0,
		const hasher& h         = hasher (), 
		const key_equal& k      = key_equal (),
		const allocator_type& a = allocator_type ())
		:
		__base (beg, end, n, h, k, a)
	{
	}

	judyhash_map (const __this_type& a)
		:
		__base (a)
	{
	}

	~judyhash_map ()
	{
	}
};

template <
	typename TKey,
	typename THashFunc, // = std::hash <Key>,
	typename TEqualFunc = std::equal_to <TKey>,
	typename TAllocator = std::allocator < TKey> >
class judyhash_set
:
public __judyhash_map <
	TKey, char, THashFunc, TEqualFunc, TAllocator,
	__judyhash_list_set
		<typename __judyhash_set_traits <TKey>::value_type, TEqualFunc>,
	__judyhash_set_traits <TKey> >
{
private:
	typedef __judyhash_map <
		TKey, char, THashFunc, TEqualFunc, TAllocator,
		__judyhash_list_set
			<typename __judyhash_set_traits <TKey>::value_type, TEqualFunc>,
		__judyhash_set_traits <TKey> > __base;
	typedef judyhash_set <
		TKey, THashFunc, TEqualFunc, TAllocator> __this_type;

public:
	typedef typename __base::key_equal               key_equal;
	typedef typename __base::hasher                  hasher;
	typedef typename __base::allocator_type          allocator_type;

	__JUDYHASH_TYPEDEFS

	judyhash_set ()
	{
	}

	judyhash_set (
		size_type n,
		const hasher& h         = hasher (), 
		const key_equal& k      = key_equal (),
		const allocator_type& a = allocator_type ())
		:
		__base (n, h, k, a)
	{
	}

	template <class Tit>
	judyhash_set (
		Tit beg, Tit end,
		size_type n             = 0,
		const hasher& h         = hasher (), 
		const key_equal& k      = key_equal (),
		const allocator_type& a = allocator_type ())
		:
		__base (beg, end, n, h, k, a)
	{
	}

	judyhash_set (const __this_type& a)
		:
		__base (a)
	{
	}

	~judyhash_set ()
	{
	}
};
