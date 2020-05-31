#include <stack>
#include <iostream>

#define _DEBUG_

#ifndef RBTREE_H_
#define RBTREE_H_
enum colour {RED = 0, BLACK = 1};

template<typename K, typename V> class RBIterator;

template<typename K, typename V> class RBTree
{
	friend class RBIterator<K,V>;
	public:
	typedef RBIterator<K, V> iterator;
	RBTree();
	RBTree(const RBTree<K,V>& cpy);
	RBTree<K,V>& operator=(const RBTree<K, V>& cpy);
	V* operator[](K key);
	int put(K key, const V& value);
	int remove(K key);
	iterator root();
	iterator min();
	iterator max();
	iterator begin();
	iterator end();
	iterator byKey(K key);
	~RBTree();
#ifdef _DEBUG_
	int debug();
#endif
	private:
	struct node
	{
		node* _parent, * _right, * _left;
		colour _nodeclr;
		K _key;
		V _value;
		node(node* parent, K key, const V& value, colour clr);
		~node();
		node* parent();
		node* grandparent();
		node* uncle();
		node* sibling();
		node* redChild();
		node* blackChild();
		node* child();
	};
	bool add(node* n, node* parent = nullptr, int side = 0);
	bool find(K key, node** N) const;
	node* findMin(node* N) const;
	node* findMax(node* N) const;
	bool rotate(node* n, char dir);
	bool fix(node* n);
	bool fixBB(node* n);
	int erase(node* n);
	int topple(node* n = nullptr);
#ifdef _DEBUG_
	int debug(node* p);
#endif
	node* _root;
};

template <typename K, typename V> class RBIterator
{
	public:
		RBIterator(RBTree<K, V>* owner = nullptr, typename RBTree<K,V>::node* bnode = nullptr);
		RBIterator(const RBIterator& cpy);
		RBIterator& operator=(const RBIterator& cpy);
		RBIterator& operator++();
		RBIterator& operator--();
		V* operator*();
		bool operator!=(const RBIterator& i);
		bool operator==(const RBIterator& i);
		bool empty();
		~RBIterator();
		bool getKey(K* key);
#ifdef _DEBUG_
		colour getColour();
		bool pop();
#endif
	private:
		RBTree<K, V>* _owner;
		std::stack<typename RBTree<K, V>::node*>* _path;
};

template<typename K, typename V> RBTree<K, V>::RBTree()
{
	_root = nullptr;
}

template<typename K, typename V> RBTree<K, V>::RBTree(const RBTree<K, V>& cpy)
{
	add(cpy._root);
}

template<typename K, typename V> RBTree<K, V>& RBTree<K, V>::operator=(const RBTree<K, V>& cpy)
{
	topple();
	add(cpy._root);
	return *this;
}

template <typename K, typename V> bool RBTree<K, V>::add(node* n, node* parent, int side)
{
	RBTree<K, V>::node* temp;
	if (!n)
		return false;
	temp = new node(parent, n->_key, n->_value, n->_nodeclr);
	if (!parent)
		_root = temp;
	else
	{
		if (side >= 0)
			parent->_left = temp;
		else
			parent->_right = temp;
	}
	if (n->_right)
		add(n->_right, temp, -1);
	if (n->_left)
		add(n->_left, temp, 1);
	return true;
}

template <typename K, typename V> V* RBTree<K, V>::operator[](K key)
{
	node* nf;
	if(find(key, &nf))
		return &(nf->_value);
	else
		return nullptr;
}

template<typename K, typename V> int RBTree<K, V>::put(K key, const  V& value)
{
	typename RBTree<K, V>::node* parent, *temp;
	bool ret = find(key, &parent);
	if(ret)
	{
		parent->_value = value;
		return 1;
	}
	else
	{
		temp = new node(parent, key, value, colour::RED);
		if (!_root)
			_root = temp;
		else if(parent->_key > key)
			parent->_right = temp;
		else
			parent->_left = temp;
		fix(temp);
		return 0;
	}
}

template <typename K, typename V> bool RBTree<K, V>::find(K key, node** N) const
{
	node* nx = _root, *cur = nullptr;
	while(nx)
	{
		cur = nx;
		if(cur->_key == key)
		{
			*N = cur;
			return true;
		}
		nx = ((cur->_key > key) ? cur->_right : cur->_left);
	}
	*N = cur;
	return false;
}

template <typename K, typename V> typename RBTree<K, V>::node* RBTree<K, V>::findMin(node* N) const
{
	node* cur = N;
	if(N)
		while (cur->_left)
		{
			cur = cur->_left;
		}
	return cur;
}

template <typename K, typename V> typename RBTree<K, V>::node* RBTree<K, V>::findMax(node* N) const
{
	node* cur = N;
	if(N)
		while (cur->_right)
		{
			cur = cur->_right;
		}
	return cur;
}

template <typename K, typename V> bool RBTree<K, V>::fix(typename RBTree::node* n)
{
	if (!n)
		throw - 5;
	if (n->_nodeclr == colour::BLACK)
		return true;
	if (n == _root)
	{
		n->_nodeclr = colour::BLACK;
		return true;
	}
	if (n->parent()->_nodeclr == colour::BLACK)
	{
		return true;
	}
	if (n->parent()->_nodeclr == colour::RED)
	{
		if ((n->uncle()) && (n->uncle()->_nodeclr == colour::RED))
		{
			n->uncle()->_nodeclr = colour::BLACK;
			n->parent()->_nodeclr = colour::BLACK;
			n->parent()->parent()->_nodeclr = colour::RED;
			return fix(n->grandparent());
		}
		else
		{
			node* p = n->parent();
			if ((n->parent() == n->grandparent()->_right) && (n == n->parent()->_left))
			{
				if (!rotate(n->parent(), -1))
					throw - 2;
				p = n;
			}
			else if ((n->parent() == n->grandparent()->_left) && (n == n->parent()->_right))
			{
				if (!rotate(n->parent(), 1))
					throw - 2;
				p = n;
			}
			p->_nodeclr = colour::BLACK;
			p->parent()->_nodeclr = colour::RED;
			rotate(p->parent(), ((p == p->parent()->_left) ? -1 : 1));
			return true;
		}
	}
	return false;
}

template <typename K, typename V> int RBTree<K,V>::remove(K key)
{
	node* convict, *leftFake, *rightFake;
	if (!find(key, &convict))
	{
		return -1;
	}
	else
	{
		rightFake = findMax(convict->_right);
		leftFake = findMin(convict->_left);
		if (((rightFake) && (rightFake->_left) && (rightFake->_right)) || ((leftFake) && (leftFake->_left) && (leftFake->_right)))
			throw 1;
		if ((!leftFake) && (!rightFake));
		else if ((!rightFake) || (leftFake->_nodeclr == colour::RED) || ((rightFake->_nodeclr == colour::BLACK) && (leftFake->redChild())) || ((rightFake->_nodeclr == colour::BLACK) && (!(rightFake->redChild())) && (leftFake->sibling()->redChild())))
		{
			convict->_key = leftFake->_key;
			convict->_value = leftFake->_value;
			convict = leftFake;
		}
		else 
		{
			convict->_key = rightFake->_key;
			convict->_value = rightFake->_value;
			convict = rightFake;
		}
		if (convict->_nodeclr == colour::RED)
		{
			return erase(convict);
		}
		else if (convict->redChild())
		{
			convict->redChild()->_nodeclr = colour::BLACK;
			return erase(convict);
		}
		else
		{
			fixBB(convict);
			return erase(convict);
		}
	}
}

template <typename K, typename V> bool RBTree<K, V>::fixBB(typename RBTree::node* n)
{
	if (!n)
		throw -5;
	if (n == _root)
	{
		n->_nodeclr = colour::BLACK;
		return true;
	}
	if (!(n->sibling()))
		throw -9;
	if (n->sibling()->_nodeclr == colour::RED)
	{
		if (n->parent()->_nodeclr != colour::BLACK)
			throw -6;
		n->sibling()->_nodeclr = colour::BLACK;
		n->parent()->_nodeclr = colour::RED;
		rotate(n->parent(), ((n->parent()->_left == n) ? 1 : -1));
	}
	//if (n->sibling()->_nodeclr == colour::BLACK)
	//	throw -7;
	else if (!(n->sibling()->child()))
	{
		rotate(n->parent(), ((n->parent()->_left == n) ? 1 : -1));
		if (n->parent()->_nodeclr == colour::BLACK)
		{
			n->parent()->_nodeclr = colour::RED;
			return fixBB(n->grandparent());
		}
		else
			return true;
	}
	else if  (n->sibling()->redChild())
	{
		if ((n->parent()->_left == n) && (n->sibling()->_left) && (n->sibling()->_left->_nodeclr == colour::RED))
		{
			rotate(n->sibling(), -1);
		}
		else if ((n->parent()->_right == n) && (n->sibling()->_right) && (n->sibling()->_right->_nodeclr == colour::RED))
		{
			rotate(n->sibling(), 1);
		}
		if (n->parent()->_nodeclr == colour::BLACK)
			n->sibling()->_nodeclr = colour::BLACK;
		rotate(n->parent(), ((n->parent()->_left == n) ? 1 : -1));
		n->parent()->_nodeclr = colour::BLACK;
		return true;
	}
	else if (n->parent()->_nodeclr == colour::BLACK)
	{
		n->sibling()->_nodeclr = colour::RED;
		return fixBB(n->parent());
	}
	else
	{
		n->sibling()->_nodeclr = colour::RED;
		n->parent()->_nodeclr = colour::BLACK;
		return true;
	}
	return false;
}

template <typename K, typename V> int RBTree<K, V>::erase(typename RBTree::node* n)
{
	if (!n)
		return -1;
	if(n->_left && n->_right)
		throw -100;
	if (_root == n)
	{
		_root = n->child();
		if(n->child())
			n->child()->_parent = nullptr;
	}
	else
		if (n->parent()->_left == n)
		{
			n->parent()->_left = n->child();
			if(n->child())
				n->child()->_parent = n->parent();
		}
		else
		{
			n->parent()->_right = n->child();
			if(n->child())
				n->child()->_parent = n->parent();
		}
	delete n;
	return 0;
}

template <typename K, typename V> int RBTree<K, V>::topple(typename RBTree::node* n)
{
	node* cur = (n ? n : _root);
	if(!cur)
		return 0;
	else
	{
		while(cur->_right)
		{
		//	if(!(cur->_right->parent()))
		//		std::cout << std::endl;
			topple(cur->_right);
		}
		while(cur->_left)
		{
		//	if(!(cur->_left->parent()))
		//		std::cout << std::endl;
			topple(cur->_left);
		}
		erase(cur);
		return 0;
	}
}

template <typename K, typename V> bool RBTree<K, V>::rotate(typename RBTree::node* n, char dir)
{
	node* successor = ((dir >= 0) ? (n->_right) : (n->_left));
	if ((!successor) || ((_root == n) && (successor->_nodeclr == colour::RED)))
		throw -6;
	//return false;
	successor->_parent = n->_parent;
	n->_parent = successor;
	if(successor->parent())
	{
		if(successor->parent()->_left == n)
			successor->parent()->_left = successor;
		else
			successor->parent()->_right = successor;
	}
	if (dir >= 0)
	{
		n->_right = successor->_left;
        if(n->_right)
			n->_right->_parent = n;
		successor->_left = n;
	}
	else
	{
		n->_left = successor->_right;
        if(n->_left)
			n->_left->_parent = n;
		successor->_right = n;
	}
	if(_root == n)
		_root = successor;
	return true;
}

template<typename K, typename V> typename RBTree<K, V>::iterator RBTree<K, V>::root()
{
	return RBIterator<K, V> (this, _root);
}

template<typename K, typename V> typename RBTree<K, V>::iterator RBTree<K, V>::min()
{
	return begin();
}

template<typename K, typename V> typename RBTree<K, V>::iterator RBTree<K, V>::max()
{
	return end();
}

template<typename K, typename V> typename RBTree<K, V>::iterator RBTree<K, V>::begin()
{
	node* i = _root;
	if(!i)
		return RBIterator<K, V>();
	while(i->_right)
		i = i->_right;
	return RBIterator<K, V>(this, i);
}

template<typename K, typename V> typename RBTree<K, V>::iterator RBTree<K, V>::end()
{
    return RBIterator<K, V>();
}

template<typename K, typename V> typename RBTree<K, V>::iterator RBTree<K, V>::byKey(K key)
{
	node* i;
	if(!find(key, i))
		return RBIterator<K, V>();
	else
		return RBIterator<K, V>(this, i);
}


template <typename K, typename V> RBTree<K, V>::node::node(RBTree<K, V>::node* parent, K key, const V& value, colour clr)
{
	_parent = parent;
	_right = nullptr;
	_left = nullptr;
	_key = key;
	_value = value;
	_nodeclr = clr;
}

template <typename K, typename V> typename RBTree<K, V>::node* RBTree<K, V>::node::parent()
{
	return _parent;
}

template <typename K, typename V> typename RBTree<K, V>::node* RBTree<K, V>::node::grandparent()
{
	return (_parent ? _parent->parent() : nullptr);
}

template <typename K, typename V> typename RBTree<K, V>::node* RBTree<K, V>::node::uncle()
{
	return (_parent ? _parent->sibling() : nullptr);
}

template <typename K, typename V> typename RBTree<K, V>::node* RBTree<K, V>::node:: sibling()
{
	if(_parent)
	{
		if(_parent->_right == this)
			return _parent->_left;
		else
			return _parent->_right;
	}
	return nullptr;
}

template <typename K, typename V> typename RBTree<K, V>::node* RBTree<K, V>::node::redChild()
{
	if ((_left) && (_left->_nodeclr == colour::RED))
		return _left;
	if ((_right) && (_right->_nodeclr == colour::RED))
		return _right;
	return nullptr;
}

template <typename K, typename V> typename RBTree<K, V>::node* RBTree<K, V>::node::blackChild()
{
	if ((_left) && (_left->_nodeclr == colour::BLACK))
		return _left;
	if ((_right) && (_right->_nodeclr == colour::BLACK))
		return _right;
	return nullptr;
}

template <typename K, typename V> typename RBTree<K, V>::node* RBTree<K, V>::node::child()
{
	return (_left ? _left : _right);
}

template <typename K, typename V> RBTree<K, V>::node::~node()
{

}

template <typename K, typename V> RBTree<K, V>::~RBTree()
{
	topple();
}



template<typename K, typename V> RBIterator<K,V>::RBIterator(RBTree<K, V>* owner, typename RBTree<K, V>::node* bnode)
{
	typename RBTree<K, V>::node* i;
	_owner = owner;
	if (_owner)
	{
		i = _owner->_root;
		_path = new std::stack<typename RBTree<K, V>::node*>();
		if (bnode)
			while ((i) && (i != bnode))
			{
				_path->push(i);
				i = ((i->_key > bnode->_key) ? (i->_right) : (i->_left));
			}
		if (!i)
			throw - 1;
		else
			_path->push(i);
	}
}

template<typename K, typename V> RBIterator<K,V>::RBIterator(const RBIterator& cpy)
{
	_owner = cpy._owner;
	_path = new std::stack<typename RBTree<K, V>::node*>(*(cpy._path));
}

template <typename K, typename V> RBIterator<K,V>& RBIterator<K,V>::operator=(const RBIterator& cpy)
{
	_owner = cpy._owner;
	_path = new std::stack<typename RBTree<K,V>::node*>(*(cpy._path));
	return *this;
}

template<typename K, typename V> RBIterator<K,V>& RBIterator<K, V>::operator++()
{
	typename RBTree<K, V>::node* prev, *cur;
	if (!_owner)
		return *this;
	cur = _path->top();
	prev = cur;
	while (cur)
	{
		if ((cur->_left) && (cur->_left != prev))
		{
			cur = cur->_left;
			while (cur)
			{
				_path->push(cur);
				cur = cur->_right;
			}
			return *this;
		}
		_path->pop();
		if (_path->size() == 0)
		{
			cur = nullptr;
			delete _path;
			_owner = nullptr;
			return *this;
		}
		prev = cur;
		cur = _path->top();
		if (prev == cur->_right)
			return *this;
	}
	//*this = _owner->max();
	throw -3;
	return *this;
}

template<typename K, typename V> RBIterator<K, V>& RBIterator<K, V>::operator--()
{
	typename RBTree<K, V>::node* prev, *cur;
	if (!_owner)
		return *this;
	cur = _path->top();
	prev = cur;
	while (cur)
	{
		if ((cur->_right) && (cur->_right != prev))
		{
			cur = cur->_right;
			while (cur)
			{
				_path->push(cur);
				cur = cur->_left;
			}
			return *this;
		}
		_path->pop();
		if (_path->empty())
		{
			cur = nullptr;
			delete _path;
			_owner = nullptr;
			return *this;
		}
		prev = cur;
		cur = _path->top();
	}
	//*this = _owner->max();
	throw -3;
	return *this;
}

template<typename K, typename V> V* RBIterator<K, V>::operator*()
{
	return &(_path->top()->_value);
}

template<typename K, typename V> bool RBIterator<K, V>::operator!=(const RBIterator& i)
{
	if((_owner == i._owner)&&((!_owner)||(_path->top() == i._path->top())))
		return false;
	else
		return true;
}

template<typename K, typename V> bool RBIterator<K, V>::operator==(const RBIterator& i)
{
	if ((_owner == i._owner) && ((!_owner) || (_path->top() == i._path->top())))
		return true;
	else
		return false;
}

template <typename K, typename V> bool RBIterator<K, V>::empty()
{
	if(!_owner)
		return true;
	else
		return false;
}

template<typename K, typename V> RBIterator<K, V>::~RBIterator()
{
	if(!empty())
		delete _path;
}

template <typename K, typename V> bool RBIterator<K, V>::getKey(K* key)
{
	if (_owner)
	{
		*key = (_path->top()->_key);
		return true;
	}
	else
		return false;
}


#ifdef _DEBUG_
template<typename K, typename V> int RBTree<K, V>::debug()
{
	return debug(_root);
}

template<typename K, typename V> int RBTree<K, V>::debug(node* p)
{
	std::cout << p << " { " <<" colour " << p->_nodeclr << " key " << p->_key << " parent " << p->_parent << " left " << p->_left << " right " << p->_right << std::endl;
	if (p->_left)
		debug(p->_left);
	if (p->_right)
		debug(p->_right);
	return 0;
}

template <typename K, typename V> colour RBIterator<K,V>::getColour()
{
	if (_owner)
		return _path->top()->_nodeclr;
	else
		return colour::RED;
}


template <typename K, typename V> bool RBIterator<K, V>::pop()
{
	if (!_owner)
		return false;
	else
	{
		if (_path->size() == 0)
		{
			_owner = nullptr;
			delete _path;
			return false;
		}
		else
		{
			_path->pop();
			if(_path->size() == 0)
			{
				_owner = nullptr;
				delete _path;
				return false;
			}
			else
				return true;
		}
	}
}
#endif
#endif
