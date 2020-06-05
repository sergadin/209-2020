

#ifndef UTILITY_H_
#define UTILITY_H_

struct sEx
{
	int code;
	const char* msg;
	sEx(int c, const char* m):code(c), msg(m){}
};

template<typename F, typename S> struct pair
{
	F _first;
	S _second;
	pair(){};
	pair(F first, S second)
	{
		_first = first;
		_second = second;
	}
};


template<typename T> class queuelim
{
private:
	struct node
	{
		T _data;
		node* _next;
		node* _prev;
		node(){}
		node(T& data){_data = data;}
	};
	node* _head;
	node* _tail;
	unsigned int _size;

public:
	class iterator
	{
	public:
		iterator(node* n);
		iterator operator++(int i);
		iterator& operator++();
		iterator operator--(int i);
		iterator& operator--();
		bool operator==(const iterator& i);
		bool operator!=(const iterator& i);
		T* operator*();
	private:
		node* _n;
	};

	queuelim();
	queuelim(const queuelim& cpy);
	iterator begin();
	iterator end();
	iterator operator[](int i);
	const queuelim& operator=(const queuelim& cpy);
	unsigned int size();
	T pop();
	int push(T data);
	int clear();
	~queuelim();

};

template<typename T> queuelim<T>::queuelim()
{
	_head=_tail=nullptr;
	_size = 0;
}

template<typename T>  typename queuelim<T>::iterator queuelim<T>::begin()
{
	return iterator(_head);
}

template<typename T>  typename queuelim<T>::iterator queuelim<T>::end()
{
	return iterator(nullptr);
}

template<typename T>  typename queuelim<T>::iterator queuelim<T>::operator[](int i)
{
	iterator it = begin();
	if (i >= _size)
		return iterator(nullptr);
	for(int j = 0; j < i; j++)
	{
		it++;
	}
	return it;
}

template<typename T> unsigned int queuelim<T>::size()
{
	return _size;
}

template<typename T> T queuelim<T>::pop()
{
	T ret;
	node temp;
	if(_head == nullptr)
		throw sEx(-5, "poped an empty queue");
	else
	{
		temp = *_head;
		ret = temp._data;
	}
	if(_tail == _head)
	{
		delete _head;
		_head = nullptr;
		_tail = nullptr;
	}
	else
	{
		delete _head;
		_head = temp._next;
	}
	_size--;
	return ret;
}

template<typename T>  int queuelim<T>::push(T data)
{
	if(_head == nullptr)
	{
		_head = new node(data);
		if (!_head)
			throw sEx(-3, "out of memory");
		_head->_next = nullptr;
		_head->_prev = nullptr;
		_tail = _head;
	}
	else
	{
		_tail->_next = new node(data);
		if (!_tail->_next)
			throw sEx(-3, "out of memory");
		(_tail->_next)->_prev = _tail;
		_tail = _tail->_next;
		_tail->_next = nullptr;
	}
	return ++_size;
}

template<typename T> int queuelim<T>::clear()
{
	for( ; _size > 0; pop());
	return 0;
}

template<typename T> queuelim<T>::~queuelim()
{
	clear();
}



template<typename T> queuelim<T>::iterator::iterator(node* n)
{
	_n = n;
}

template<typename T> typename queuelim<T>::iterator queuelim<T>::iterator::operator++(int i)
{
	iterator ret = *this;
	if(_n)
		_n = _n->_next;
	else
		_n = nullptr;
	return ret;
}

template<typename T> typename queuelim<T>::iterator& queuelim<T>::iterator::operator++()
{
	if(_n)
		_n = _n->_next;
	else
		_n = nullptr;
	return *this;
}

template<typename T> typename queuelim<T>::iterator queuelim<T>::iterator::operator--(int i)
{
	iterator ret = *this;
	if(_n)
		_n = _n->_prev;
	else
		_n = nullptr;
	return ret;
}

template<typename T> typename queuelim<T>::iterator& queuelim<T>::iterator::operator--()
{
	if(_n)
		_n = _n->_prev;
	else
		_n = nullptr;
	return *this;
}

template<typename T> bool queuelim<T>::iterator::operator==(const iterator& i)
{
	return(_n == i._n);
}

template<typename T> bool queuelim<T>::iterator::operator!=(const iterator& i)
{

	return(_n != i._n);
}

template<typename T> T* queuelim<T>::iterator::operator*()
{
		return (_n ? &(_n->_data) : nullptr);
}

template<typename T> queuelim<T>::queuelim(const queuelim& cpy)
{

}

template<typename T> const queuelim<T>& queuelim<T>::operator=(const queuelim& cpy)
{
	return *this;
}

//listlim

template<typename T> class listlim
{
private:
	struct node
	{
		T _data;
		node* _next;
		node* _prev;
		node(){}
		node(T& data){_data = data;}
	};
	node* _head;
	node* _tail;
	unsigned int _size;

public:
	class iterator
	{
	friend class listlim<T>;
	public:
		iterator(node* n);
		iterator operator++(int i);
		iterator& operator++();
		iterator operator--(int i);
		iterator& operator--();
		bool operator==(const iterator& i);
		bool operator!=(const iterator& i);
		T* operator*();
	private:
		node* _n;
	};

	listlim();
	listlim(const listlim& cpy);
	iterator begin();
	iterator end();
	iterator operator[](int i);
	const listlim& operator=(const listlim& cpy);
	unsigned int size();
	void remove(iterator& i);
	int addHead(T data);
	int addTail(T data);
	int clear();
	~listlim();

};

template<typename T> listlim<T>::listlim()
{
	_head=_tail=nullptr;
	_size = 0;
}

template<typename T>  typename listlim<T>::iterator listlim<T>::begin()
{
	return iterator(_head);
}

template<typename T>  typename listlim<T>::iterator listlim<T>::end()
{
	return iterator(nullptr);
}

template<typename T>  typename listlim<T>::iterator listlim<T>::operator[](int i)
{
	iterator it = begin();
	if (i >= _size)
		return iterator(nullptr);
	for(int j = 0; j < i; j++)
	{
		it++;
	}
	return it;
}

template<typename T> unsigned int listlim<T>::size()
{
	return _size;
}

template<typename T> void listlim<T>::remove(iterator& i)
{
	T ret;
	if(i._n == nullptr)
		throw sEx(-5, "tried removing a null iterator");
	else
		ret = i._n->_data;
	if(i._n == _head)
		_head = i._n->_next;
	if(i._n == _tail)
		_tail = i._n->_prev;
	if(i._n->_prev)
		(i._n->_prev)->_next = i._n->_next;
	if(i._n->_next)
		(i._n->_next)->_prev = i._n->_prev;
	delete i._n;
	i._n = nullptr;
	_size--;
}

template<typename T>  int listlim<T>::addHead(T data)
{
	if(_head == nullptr)
	{
		_head = new node(data);
		if (!_head)
			throw sEx(-3, "out of memory");
		_head->_next = nullptr;
		_head->_prev = nullptr;
		_tail = _head;
	}
	else
	{
		_head->_prev = new node(data);
		if (!_head->_prev)
			throw sEx(-3, "out of memory");
		(_head->_prev)->_next = _head;
		_head = _head->_prev;
		_head->_prev = nullptr;
	}
	return ++_size;
}

template<typename T>  int listlim<T>::addTail(T data)
{
	if(_tail == nullptr)
	{
		_head = new node(data);
		if (!_head)
			throw sEx(-3, "out of memory");
		_head->_next = nullptr;
		_head->_prev = nullptr;
		_tail = _head;
	}
	else
	{
		_tail->_next = new node(data);
		if (!_tail->_next)
			throw sEx(-3, "out of memory");
		(_tail->_next)->_prev = _tail;
		_tail = _tail->_next;
		_tail->_next = nullptr;
	}
	return ++_size;
}

template<typename T> int listlim<T>::clear()
{
	for(iterator i = begin(); i != end(); i++)
		remove(i);
	return 0;
}

template<typename T> listlim<T>::~listlim()
{
	clear();
}



template<typename T> listlim<T>::iterator::iterator(node* n)
{
	_n = n;
}

template<typename T> typename listlim<T>::iterator listlim<T>::iterator::operator++(int i)
{
	iterator ret = *this;
	if(_n)
		_n = _n->_next;
	else
		_n = nullptr;
	return ret;
}

template<typename T> typename listlim<T>::iterator& listlim<T>::iterator::operator++()
{
	if(_n)
		_n = _n->_next;
	else
		_n = nullptr;
	return *this;
}

template<typename T> typename listlim<T>::iterator listlim<T>::iterator::operator--(int i)
{
	iterator ret = *this;
	if(_n)
		_n = _n->_prev;
	else
		_n = nullptr;
	return ret;
}

template<typename T> typename listlim<T>::iterator& listlim<T>::iterator::operator--()
{
	if(_n)
		_n = _n->_prev;
	else
		_n = nullptr;
	return *this;
}

template<typename T> bool listlim<T>::iterator::operator==(const iterator& i)
{
	return(_n == i._n);
}

template<typename T> bool listlim<T>::iterator::operator!=(const iterator& i)
{

	return(_n != i._n);
}

template<typename T> T* listlim<T>::iterator::operator*()
{
	return (_n ? &(_n->_data) : nullptr);
}

template<typename T> listlim<T>::listlim(const listlim& cpy)
{

}

template<typename T> const listlim<T>& listlim<T>::operator=(const listlim& cpy)
{
	return *this;
}

#endif /* UTILITY_H_ */
