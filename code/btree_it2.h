#ifndef BTREE_H
#define BTREE_H

#include <vector>
#include <algorithm>
#include <iostream>

template<class T>
class btree
{
	static const char delimeter = ' ';
public:
	typedef T value_type;
	typedef std::size_t size_type;

private:
	
	class node;

	class leaf;
	class internal;
	/*friend class node;
	friend class internal;
	friend class leaf;*/
public:
	btree(size_type m);

	bool empty() const;
	void clear();
	void deltree(node*);

	void insert(const T&);
	void insert(node*, const T&);

	void erase(const T&);
	void erase(node*, const T&);

	node* search(const T&);

	void print() const;
	void print(node*) const;

private:

	const size_type kmin;
	const size_type kmax;
	const size_type cmin;
	const size_type cmax;

	node* root;
};

template<class T>
class btree<T>::node
{
public:
	virtual void insert(const value_type&);
	virtual void erase(const value_type&);
	virtual void split();
	virtual size_type size() const;
	virtual void print() const;
};

template<class T>
btree<T>::btree(typename btree<T>::size_type m):
	kmin(m/2), kmax(m), cmin((m/2)+1), cmax(m+1), root(nullptr)
{
	root = new leaf;
}

template<class T>
bool btree<T>::empty() const
{
	return root == nullptr;
}

template<class T>
void btree<T>::clear()
{
	deltree(root);
}

template<class T>
void btree<T>::insert(const btree<T>::value_type& data)
{
	insert(root, data);
}

template<class T>
void btree<T>::insert(typename btree<T>::node* n, const typename btree<T>::value_type& data)
{
	n->insert(data);
}

template<class T>
void btree<T>::print() const
{
	print(root);
}

template<class T>
void btree<T>::print(typename btree<T>::node* n) const
{
	n->print();
}

template<class T>
class btree<T>::internal:
	public btree<T>::node
{
public:
	friend class btree;
	friend class btree<T>::leaf;

	internal():
		parent(nullptr) {}

	void split();
	size_type size() const;
	void insert(const value_type& d);
	void print() const;

private:
	internal* parent;
	std::vector<value_type> keys;
	std::vector<node*> children;
};

template<class T>
void btree<T>::internal::split()
{
	if(size() > kmax)
	{
		internal* ninternal = new internal;
		value_type midkey = keys[kmin];

		for(typename std::vector<value_type>::iterator i = keys.begin()+kmin+1; i != keys.end(); ++i)
		{
			ninternal->keys.push_back(*i);
		}
		keys.erase(keys.begin()+kmin, keys.end());

		for(typename std::vector<node*>::iterator i = children.begin()+cmin; i != children.end(); ++i)
		{
			ninternal->children.push_back(*i);
		}
		children.erase(children.begin()+cmin, children.end());

		if(parent == nullptr)
		{
			internal* nparent = new internal;
			nparent->keys.push_back(midkey);
			nparent->children.push_back(this);
			nparent->children.push_back(ninternal);

			this->parent = nparent;

			if(this == root)
				root = nparent;
		}
		else
		{
			typename std::vector<value_type>::iterator i = std::lower_bound(parent->keys.begin(), parent->keys.end(), midkey);
			if(i == parent->keys.end())
			{
				parent->keys.push_back(midkey);
				parent->children.push_back(ninternal);
			}
			else
			{
				auto index = std::distance(parent->keys.begin(), i);
				parent->keys.insert(i, midkey);
				parent->children.insert(parent->children.begin()+index+1, ninternal);
			}
		}

		ninternal->parent = this->parent;

		parent.split();
	}
}

template<class T>
inline typename btree<T>::size_type btree<T>::internal::size() const
{
	return this->keys.size();
}

template<class T>
void btree<T>::internal::insert(const typename btree<T>::value_type& d)
{
	typename std::vector<value_type>::iterator i = std::lower_bound(keys.begin(), keys.end(), d);
	
	if(i == keys.end())
		children.end()->insert(d);
	else
	{
		auto index = std::distance(keys.begin(), i);
		(children.begin()+index)->insert(d);
	}
}

template<class T>
void btree<T>::internal::print() const
{
	for(typename std::vector<node*>::iterator i = children.begin(); i != children.end(); ++i)
	{
		i->print();
	}
}

// derived class containing data
template<class T>
class btree<T>::leaf:
	public btree<T>::node
{
public:
	friend class btree;
	friend class internal;

	leaf():
		parent(nullptr) {}

	void split();
	size_type size() const;
	void insert(const value_type& d);
	void print() const;

private:
	internal* parent;
	std::vector<value_type> data;
};

template<class T>
void btree<T>::leaf::split()
{
	if(size() > kmax)
	{
		leaf* nleaf = new leaf;
		value_type midkey = data[kmin];
		for(typename std::vector<value_type>::iterator i = data.begin()+kmin; i != data.end(); ++i)
		{
			nleaf->data.push_back(*i);
		}
		data.erase(data.begin()+kmin, data.end());

		if(parent == nullptr)
		{
			internal* nparent = new internal;
			nparent->keys.push_back(midkey);
			nparent->children.push_back(this);
			nparent->children.push_back(nleaf);

			this->parent = nparent;

			if(this == root)
				root = nparent;
		}
		else
		{
			typename std::vector<value_type>::iterator i = std::lower_bound(parent->keys.begin(), parent->keys.end(), midkey);
			if(i == parent->keys.end())
			{
				parent->keys.push_back(midkey);
				parent->children.push_back(nleaf);
			}
			else
			{
				auto index = std::distance(parent->keys.begin(), i);
				parent->keys.insert(i, midkey);
				parent->children.insert(parent->children.begin()+index+1, nleaf);
			}
		}

		nleaf->parent = this->parent;

		parent->split();
	}
}

template<class T>
inline typename btree<T>::size_type btree<T>::leaf::size() const
{
	return data.size();
}

template<class T>
void btree<T>::leaf::insert(const typename btree<T>::value_type& d)
{
	if(size() == 0)
		data.push_back(d);
	else
	{
		typename std::vector<value_type>::iterator i = std::lower_bound(data.begin(), data.end(), d);
		data.insert(i, d);
	}

	if(size() > kmax)
		split();
}

template<class T>
void btree<T>::leaf::print() const
{
	for(typename std::vector<value_type>::iterator i = data.begin(); i != data.end(); ++i)
	{
		std::cout << *i << delimeter;
	}
}

#endif