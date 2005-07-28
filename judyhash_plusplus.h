#include <list>
#include <utility>
#include <assert.h>

template <
	typename TKey,
	typename TValue,
	typename THashFunc,
	typename TEqualFunc >
class judyhash_map {

// types
public:
	typedef TKey                                          key_type;
	typedef TValue                                        mapped_type;
	typedef std::pair <const TKey, TValue>                value_type;

	Pvoid_t  m_judy;

//
private:
	THashFunc m_hash_func;

	struct comp_func {
		TEqualFunc m_eq_func;

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

	iterator insert(const value_type& p)
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
				}else{
					value_type *copy = ptr -> m_key_data;
					value_list *lst = ptr -> m_list = new value_list;
					lst -> insert (
						lst -> end (), *copy);
					typename value_list::iterator ret_it
						= lst -> insert (
							lst -> end (), p);

					ptr -> m_integer |= 1;
					return iterator (m_judy, h, (PPvoid_t) ptr,
									 lst -> insert (ret_it, p));
				}
			}else{
				value_list *lst = (value_list *) (ptr -> m_integer & ~1);

				typename value_list::iterator beg, end;
				beg = lst -> begin ();
				end = lst -> end ();

				typename value_list::iterator found = end;

				for (; beg != end; ++beg){
					if (m_comp_func (*beg, p)){
						(*beg).second = p.second;
						return iterator (m_judy, h, (PPvoid_t) ptr, beg);
					}
				}

				return iterator (m_judy, h, (PPvoid_t) ptr,
								 lst -> insert (lst -> end (), p));
			}
		}else{
			ptr -> m_key_data = new value_type (p);
		}

		return iterator (m_judy, h, (PPvoid_t) ptr);
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
