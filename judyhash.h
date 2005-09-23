#include <list>
#include <utility>
#include <assert.h>
#include <algorithm>

#include "Judy.h"

#if !defined(JUDYHASH_DEBUGINFO) && !defined(NDEBUG)
#define JUDYHASH_DEBUGINFO
#endif

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

#define __JUDYHASH_REDEFINE_TYPEDEFS                                      \
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

	__JUDYHASH_REDEFINE_TYPEDEFS

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
	Pvoid_t  m_judy;
	size_type m_size;

	THashFunc m_hash_func;

	TEqualFunc m_eq_func;

//	typedef std::vector <value_type*> value_list;
	typedef std::list <pointer> value_list;

	inline pointer judy_hash_new (const value_type &v)
	{
		pointer p = m_alloc.allocate (1);
		return new (p) value_type (v);
	}

	allocator_type m_alloc;

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

#ifdef JUDYHASH_DEBUGINFO
		assert (!m_debug_info.m_value_count);
		assert (!m_debug_info.m_list_count);
		assert (!m_debug_info.m_list_item_count);
#endif
	}

	void clear ()
	{
		// optimize me!!!
		erase (begin (), end ());
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
	union judy_value {
		Word_t        m_integer;
		pointer       m_key_data;
		value_list   *m_list;
	};

public:
	class iterator_base : public __judyhash_types <TKey, TValue> {
	public:
		typedef __judyhash_types <TKey, TValue> __base;
		typedef std::forward_iterator_tag iterator_category;

		__JUDYHASH_REDEFINE_TYPEDEFS

	private:
		const judyhash_map *m_obj;

		Word_t        m_index;
		Pvoid_t       m_value;
		bool          m_end;
		bool          m_inside_list;

		typename value_list::iterator m_list_it;
//		typename value_list::iterator m_list_end_it;

		friend class judyhash_map;

		void init_list_it ()
		{
			m_inside_list = false;

			judy_value *ptr = (judy_value *) m_value;
			if (ptr -> m_integer & 1){
				value_list *lst = (value_list *) (ptr -> m_integer & ~1);
				m_inside_list   = true;
				m_list_it       = lst -> begin ();

				assert (m_list_it != lst -> end ());
			}
		}

		void init ()
		{
			m_index = 0;
			m_value = 0;

			m_obj   = NULL;
			m_end   = true;

			m_inside_list = false;
		}

		void make_end ()
		{
			m_end = true;
		}

	protected:
		reference at () const
		{
			if (m_value){
				if (m_inside_list){
					return * (*m_list_it);
				}else{
					return ** (pointer *) m_value;
				}
			}else{
				throw 123;
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

		iterator_base (const judyhash_map *obj, Word_t index, Pvoid_t value)
		{
			init ();

			m_index = index;
			m_value = value;

			m_obj  = obj;
			m_end  = false;

			init_list_it ();
		}

		iterator_base (const judyhash_map *obj,
					   Word_t index, Pvoid_t value,
					   typename value_list::iterator it)
		{
			init ();

			m_index = index;
			m_value = value;

			m_obj   = obj;
			m_end   = false;

			m_inside_list = true;

			m_list_it = it;
		}

		iterator_base (const judyhash_map *obj)
		{
			init ();

			m_end = false;

			m_obj = obj;

			m_value = JudyLFirst(m_obj -> m_judy, &m_index, 0);

			if (m_value){
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
//			m_list_end_it = a.m_list_end_it;

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

		iterator_base operator ++ (int)
		{
			iterator_base ret = *this;
			operator ++ ();
			return ret;
		}

		iterator_base& operator ++ ()
		{
			if (!m_end){
				bool goto_next_judy_cell = false;

				if (m_inside_list){
					judy_value *ptr = (judy_value *) m_value;
					assert ((ptr -> m_integer & 1) == 1);

					value_list *lst = (value_list *) (ptr -> m_integer & ~1);

					goto_next_judy_cell = (lst -> end () == ++m_list_it);
				}else{
					goto_next_judy_cell = true;
				}

				if (goto_next_judy_cell){
					m_value = JudyLNext (m_obj -> m_judy, &m_index, 0);

					if (m_value){
						init_list_it ();
					}else{
						m_end = true;
					}
				}
			}else{
				throw 123;
			}

			return *this;
		}

		bool operator == (const iterator_base& i) const
		{
			if (this == &i)
				return true;

			if (m_end ^ i.m_end)
				return false;

			if (m_end && i.m_end)
				return true;

			if (m_obj != i.m_obj)
				return false;

			return (m_index       == i.m_index)
				&& (m_value       == i.m_value)
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
		iterator (const const_iterator &a)
		{
			abort ();
		}

		friend class judyhash_map;

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
	private:
		friend class judyhash_map;
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
			// It seems that standard containers work this way :(
			abort ();
		}

		assert (this == it.m_obj);

		judy_value *ptr = (judy_value *) it.m_value;

		if (it.m_inside_list){
			assert ((ptr -> m_integer & 1) == 1);

			m_size -= 1;

			m_alloc.deallocate (*it.m_list_it, 1);
#ifdef JUDYHASH_DEBUGINFO
			m_debug_info.m_list_item_count -= 1;
#endif

			value_list *lst = (value_list *) (ptr -> m_integer & ~1);
			lst -> erase (it.m_list_it);

			if (lst -> empty ()){
				delete lst;
				::JudyLDel (&m_judy, it.m_index, 0);
#ifdef JUDYHASH_DEBUGINFO
				m_debug_info.m_list_count -= 1;
#endif
			}
		}else{
			assert ((ptr -> m_integer & 1) == 0);

			m_size -= 1;

			m_alloc.deallocate (ptr -> m_key_data, 1);
			::JudyLDel (&m_judy, it.m_index, 0);

#ifdef JUDYHASH_DEBUGINFO
			m_debug_info.m_value_count -= 1;
#endif
		}
	}

private:
	iterator find_base (const key_type& key) const
	{
		unsigned long h = m_hash_func (key);
		judy_value *ptr = (judy_value *) ::JudyLGet (m_judy, h, 0);

		if (!ptr || !ptr -> m_integer){
			return iterator ();
		}else{
			if ((ptr -> m_integer & 1) == 0){
				if (m_eq_func (ptr -> m_key_data -> first, key)){
					return iterator (iterator_base
									 (this, h, (PPvoid_t) ptr));
				}else{
					iterator ret (iterator_base (this, h, (PPvoid_t) ptr));
					ret.make_end ();
					return ret;
				}
			}else{
				value_list *lst = (value_list *) (ptr -> m_integer & ~1);

				typename value_list::iterator beg, end;
				beg = lst -> begin ();
				end = lst -> end ();

				typename value_list::iterator found = end;

				for (; !(beg == end); ++beg){
					if (m_eq_func ((*beg) -> first, key)){
						return iterator (iterator_base
										 (this, h, (PPvoid_t) ptr, beg));
					}
				}

				iterator ret (iterator_base (this, h, (PPvoid_t) ptr, end));
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
		iterator e = end ();
		size_type c = 0;

		for (
			iterator found = find (key);
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

	std::pair <iterator, bool> insert (const value_type& p)
	{
		const TKey &key   = p.first;
//		const TValue &val = p.second;

		Word_t h = m_hash_func (key);
		judy_value *ptr = NULL;
		ptr = (judy_value *) ::JudyLIns (&m_judy, h, 0);

		assert (ptr);

		if (ptr -> m_integer){
			if ((ptr -> m_integer & 1) == 0){
				if (m_eq_func (
						ptr -> m_key_data -> first, p.first))
				{
//					ptr -> m_key_data -> second = p.second;

					return std::make_pair
						(iterator (iterator_base (this, h, (PPvoid_t) ptr)),
						 false);
				}else{
					pointer copy = ptr -> m_key_data;
					value_list *lst = ptr -> m_list = new value_list;
#ifdef JUDYHASH_DEBUGINFO
					m_debug_info.m_list_count       += 1;
					m_debug_info.m_list_item_count  += 2;
					m_debug_info.m_value_count      -= 1;
#endif

					lst -> insert (
						lst -> end (), copy);

					typename value_list::iterator ret_it
						= lst -> insert (
							lst -> end (), judy_hash_new (p));

					++m_size;

					ptr -> m_integer |= 1;

					return std::make_pair (
						iterator (iterator_base (
									  this, h, (PPvoid_t) ptr, ret_it)),
						true);
				}
			}else{
				value_list *lst = (value_list *) (ptr -> m_integer & ~1);

				typename value_list::iterator beg, end;
				beg = lst -> begin ();
				end = lst -> end ();

				typename value_list::iterator found = end;

				for (; !(beg == end); ++beg){
					if (m_eq_func ((*beg) -> first, p.first)){
//						(*beg) -> second = p.second;
						return std::make_pair (
							iterator (iterator_base (
										  this, h, (PPvoid_t) ptr, beg)),
							false);
					}
				}

				++m_size;

#ifdef JUDYHASH_DEBUGINFO
				m_debug_info.m_list_item_count += 1;
#endif

				return std::make_pair (
					iterator (iterator_base (
								  this, h, (PPvoid_t) ptr,
								  lst -> insert (lst -> end (),
												 judy_hash_new (p)))),
					true);
			}
		}else{
#ifdef JUDYHASH_DEBUGINFO
			m_debug_info.m_value_count += 1;
#endif

			ptr -> m_key_data = judy_hash_new (p);

			++m_size;

			return std::make_pair (
				iterator (iterator_base (this, h, (PPvoid_t) ptr)),
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
