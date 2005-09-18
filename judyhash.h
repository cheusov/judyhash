#include <list>
//#include <vector>
#include <utility>
#include <assert.h>
#include <algorithm>

#include "Judy.h"

template <
	typename TKey,
	typename TValue,
	typename THashFunc /* = std::hash<Key>*/,
	typename TEqualFunc = std::equal_to <TKey>,
	typename TAllocator = std::allocator< std::pair < TKey, TValue > > >
class judyhash_map {

// types
public:
	typedef TKey                            key_type;
	typedef TValue                          data_type;
	typedef TValue                          mapped_type;
#if 1
	typedef std::pair <TKey, TValue>  value_type;
#else
	typedef std::pair <const TKey, TValue>  value_type;
#endif
	typedef TEqualFunc                      key_equal;
	typedef THashFunc                       hasher;

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	// It is not possible to derive 'pointer' and 'reference' types from
	// TAllocator
	typedef value_type       * pointer;
	typedef value_type const * const_pointer;
	typedef value_type       & reference;
	typedef value_type const & const_reference;

	typedef TAllocator         allocator_type;

//	typedef typename _Mybase::const_iterator const_iterator;

	Pvoid_t  m_judy;
	size_type m_size;

	struct debug_info {
		int m_value_count;
		int m_list_count;
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
	THashFunc m_hash_func;

	TEqualFunc m_eq_func;

//	typedef std::vector <value_type*> value_list;
	typedef std::list <pointer> value_list;

	inline pointer judy_hash_new (const value_type &v)
	{
		pointer p = m_alloc.allocate (1);
		new (p) value_type (v);
		return p;
	}

	allocator_type m_alloc;

//
public:
	judyhash_map (
		size_type n        = 0,
		const hasher& h    = hasher (), 
		const key_equal& k = key_equal (),
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
		size_type          = 0,
		const hasher& h    = hasher (), 
		const key_equal& k = key_equal (),
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
		insert (a.begin (), a.end ());
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
		return ::JudyLCount (m_judy);
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

	union judy_value {
		Word_t        m_integer;
		pointer       m_key_data;
		value_list   *m_list;
	};

	class iterator {
	private:
		Word_t   m_index;
		Pvoid_t  m_value;
		Pvoid_t  m_judy;
		bool     m_end;
		bool     m_inside_list;
		typename value_list::iterator m_list_it;
		typename value_list::iterator m_list_end_it;

//		friend template <
//			typename TKey, typename TValue,
//			typename THashFunc, typename TEqualFunc,
//			typename TAllocator >
		friend class judyhash_map;

		void init_list_it ()
		{
			m_inside_list = false;

			judy_value *ptr = (judy_value *) m_value;
			if (ptr -> m_integer & 1){
				value_list *lst = (value_list *) (ptr -> m_integer & ~1);
				m_inside_list   = true;
				m_list_it       = lst -> begin ();
				m_list_end_it   = lst -> end ();
			}
		}

		void init ()
		{
			m_index = 0;
			m_value = 0;

			m_judy  = 0;
			m_end   = true;

			m_inside_list = false;
		}

	public:
		iterator ()
		{
			init ();
		}

		iterator (const iterator &a)
		{
			m_index       = a.m_index;
			m_value       = a.m_value;
			m_judy        = a.m_judy;
			m_end         = a.m_end;
			m_inside_list = a.m_inside_list;
			m_list_it     = a.m_list_it;
			m_list_end_it = a.m_list_end_it;
		}

		iterator (Pvoid_t judy, Word_t index, Pvoid_t value)
		{
			init ();

			m_index = index;
			m_value = value;

			m_judy  = judy;
			m_end   = false;

			init_list_it ();
		}

		iterator (Pvoid_t judy,
				  Word_t index, Pvoid_t value,
				  typename value_list::iterator it)
		{
			init ();

			m_index = index;
			m_value = value;

			m_judy  = judy;
			m_end   = false;

			m_inside_list = true;

			m_list_it = it;
		}

		iterator (Pvoid_t judy)
		{
			init ();

			m_end = false;

			m_judy = judy;

			m_value = JudyLFirst(m_judy, &m_index, 0);

			if (m_value){
				init_list_it ();
			}else{
				m_end = true;
			}
		}

		reference operator * ()
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

		iterator operator ++ (int)
		{
			iterator ret = *this;
			operator ++ ();
			return ret;
		}

		iterator& operator ++ ()
		{
			if (!m_end){
				if (!m_inside_list || ++m_list_it == m_list_end_it){
					m_value = JudyLNext (m_judy, &m_index, 0);
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

		bool operator == (const iterator& i) const
		{
			if (m_end ^ i.m_end)
				return false;

			if (m_end && i.m_end)
				return true;

			if (m_judy != i.m_judy)
				return false;

			return (m_index == i.m_index);
		}

		void make_end ()
		{
			m_end = true;
		}
	};
	friend class iterator;

	void erase (const key_type& key)
	{
		erase (find (key));
	}

#ifdef JUDYHASH_ERASE_RETURN_IT
	iterator erase (iterator f, iterator l)
#else
	void erase (iterator f, iterator l)
#endif
	{
		while (! (f == l)){
			erase (f++);
		}

#ifdef JUDYHASH_ERASE_RETURN_IT
		return f;
#endif
	}

	void erase (iterator it)
	{
		if (it.m_end)
			return;

		assert (m_judy == it.m_judy);

		judy_value *ptr = (judy_value *) it.m_value;
		if (ptr -> m_integer & 1){
			value_list *lst = (value_list *) (ptr -> m_integer & ~1);

#ifndef NDEBUG
			m_debug_info.m_list_item_count -= 1;
#endif

//			bool is_beg = (m_list_it == lst -> begin ());
			lst -> erase (it.m_list_it);
		}else{

#ifndef NDEBUG
			m_debug_info.m_value_count -= 1;
#endif

			m_alloc.deallocate (ptr -> m_key_data, 1);
			::JudyLDel (&m_judy, it.m_index, 0);
		}
	}

	iterator find (const key_type& key) const
	{
		unsigned long h = m_hash_func (key);
		judy_value *ptr = (judy_value *) ::JudyLGet (m_judy, h, 0);

		if (!ptr || !ptr -> m_integer){
			return iterator ();
		}else{
			if ((ptr -> m_integer & 1) == 0){
				if (m_eq_func (ptr -> m_key_data -> first, key)){
					return iterator (m_judy, h, (PPvoid_t) ptr);
				}else{
					iterator ret (m_judy, h, (PPvoid_t) ptr);
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
						return iterator (m_judy, h, (PPvoid_t) ptr, beg);
					}
				}

				iterator ret (m_judy, h, (PPvoid_t) ptr, end);
				ret.make_end ();
				return ret;
			}
		}
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

	std::pair <iterator, bool> insert(const value_type& p)
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
						(iterator (m_judy, h, (PPvoid_t) ptr),
						 false);
				}else{
					pointer copy = ptr -> m_key_data;
#ifndef NDEBUG
					m_debug_info.m_list_count       += 1;
					m_debug_info.m_list_item_count  += 2;
					m_debug_info.m_value_count      -= 1;
#endif
					value_list *lst = ptr -> m_list = new value_list;

					lst -> insert (
						lst -> end (), copy);

					typename value_list::iterator ret_it
						= lst -> insert (
							lst -> end (), judy_hash_new (p));

					++m_size;

					ptr -> m_integer |= 1;

					return std::make_pair (
						iterator (m_judy, h, (PPvoid_t) ptr, ret_it),
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
							iterator (m_judy, h, (PPvoid_t) ptr, beg),
							false);
					}
				}

				++m_size;

				return std::make_pair (
					iterator (m_judy, h, (PPvoid_t) ptr,
							  lst -> insert (lst -> end (), judy_hash_new (p))),
					true);
			}
		}else{
#ifndef NDEBUG
			m_debug_info.m_value_count += 1;
#endif

			ptr -> m_key_data = judy_hash_new (p);

			++m_size;

			return std::make_pair (
				iterator (m_judy, h, (PPvoid_t) ptr),
				true);
		}
	}

	iterator begin () const
	{
		return iterator (m_judy);
	}

	iterator end () const
	{
		return iterator ();
	}
};
