#include <list>
#include <utility>
#include <assert.h>
#include <algorithm>

template <
	typename TKey,
	typename TValue,
	typename THashFunc,
	typename TEqualFunc >
class judyhash_map {

// types
public:
	typedef TKey                            key_type;
	typedef TValue                          data_type;
	typedef TValue                          mapped_type;
	typedef std::pair <const TKey, TValue>  value_type;
	typedef TEqualFunc                      key_equal;
	typedef THashFunc                       hasher;

//	typedef _Ty referent_type;
//	typedef typename _Mybase::value_compare value_compare;
//	typedef typename _Mybase::allocator_type allocator_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef value_type       * pointer;
	typedef value_type const * const_pointer;
	typedef value_type       & reference;
	typedef value_type const & const_reference;

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

	struct comp_func {
		TEqualFunc m_eq_func;

		comp_func ()
		{
		}

		comp_func (const key_equal& eq_func)
			: m_eq_func (eq_func)
		{
		}

		bool operator () (const value_type &a, const value_type &b) const
		{
			return m_eq_func (a.first, b.first);
		}
	} m_comp_func;

	typedef std::list <value_type> value_list;

//
public:
	judyhash_map ()
	{
		m_judy = 0;
		m_size = 0;
	}

	template <class Tit>
	judyhash_map (Tit beg, Tit end)
	{
		m_judy = 0;
		m_size = 0;

		insert (beg, end);
	}

	judyhash_map (const judyhash_map& a)
	{
		m_judy       = a.m_judy;
		m_size       = a.m_size;
		m_hash_func  = a.m_hash_func;
		m_debug_info = a.m_debug_info;
	}

	template <class Tit>
	void insert (Tit beg, Tit end)
	{
		while (beg != end){
			insert (*beg);

			++beg;
		}
	}

	void swap (judyhash_map& a)
	{
		std::swap (m_judy, a.m_judy);
		std::swap (m_size, a.m_size);
		std::swap (m_hash_func, a.m_hash_func);
		std::swap (m_debug_info, a.m_debug_info);
	}

//	judyhash_map (const key_equal& eq_func)
//		: m_comp_func (eq_func)
//	{
//		m_judy = 0;
//		m_size = 0;
//	}

//	judy hash_map(const key_compare& eq_func, const allocator_type& alloc)
//		: _Mybase(_Traits, _Al)
//	{
//	}

	size_type size () const
	{
		return m_size;
	}

	union judy_value {
		Word_t        m_integer;
		value_type   *m_key_data;
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

		value_type operator * ()
		{
			if (m_value){
				if (m_inside_list){
					return *m_list_it;
				}else{
					return ** (value_type **) m_value;
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

		value_type& operator ++ ()
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
	};
	friend class iterator;

	std::pair <iterator, bool> insert(const value_type& p)
	{
		const TKey &key   = p.first;
		const TValue &val = p.second;

		unsigned long h = m_hash_func (key);
		judy_value *ptr = (judy_value *) ::JudyLIns (&m_judy, h, 0);

		assert (ptr);

		if (ptr -> m_integer){
			if ((ptr -> m_integer & 1) == 0){
				if (m_comp_func (*ptr -> m_key_data, p)){
					ptr -> m_key_data -> second = p.second;

					return std::make_pair
						(iterator (m_judy, h, (PPvoid_t) ptr),
						 false);
				}else{
					value_type *copy = ptr -> m_key_data;
#ifndef NDEBUG
					m_debug_info.m_list_count       += 1;
					m_debug_info.m_list_item_count  += 2;
					m_debug_info.m_value_count      -= 1;
#endif
					value_list *lst = ptr -> m_list = new value_list;
					lst -> insert (
						lst -> end (), *copy);
					typename value_list::iterator ret_it
						= lst -> insert (
							lst -> end (), p);

					++m_size;

					ptr -> m_integer |= 1;

					return std::make_pair (
						iterator (m_judy, h, (PPvoid_t) ptr,
								  lst -> insert (ret_it, p)),
						true);
				}
			}else{
				value_list *lst = (value_list *) (ptr -> m_integer & ~1);

				typename value_list::iterator beg, end;
				beg = lst -> begin ();
				end = lst -> end ();

				typename value_list::iterator found = end;

				for (; !(beg == end); ++beg){
					if (m_comp_func (*beg, p)){
						(*beg).second = p.second;
						return std::make_pair (
							iterator (m_judy, h, (PPvoid_t) ptr, beg),
							false);
					}
				}

				++m_size;

				return std::make_pair (
					iterator (m_judy, h, (PPvoid_t) ptr,
							  lst -> insert (lst -> end (), p)),
					true);
			}
		}else{
#ifndef NDEBUG
			m_debug_info.m_value_count += 1;
#endif

			ptr -> m_key_data = new value_type (p);

			++m_size;

			return std::make_pair (
				iterator (m_judy, h, (PPvoid_t) ptr),
				true);
		}
	}

	iterator begin ()
	{
		return iterator (m_judy);
	}

	iterator end ()
	{
		return iterator ();
	}
};
