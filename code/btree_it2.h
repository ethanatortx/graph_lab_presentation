#ifndef BTREE_H
#define BTREE_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

template<class T>
class btree
{
	typedef std::size_t size_type;

	struct node
	{
		node(): parent(nullptr) {}
		node* parent;
		std::vector<T> data;
		std::vector<node*> children;
	};
public:
	btree(size_type);

	void insert(const T& data);
	void insert(node* n, const T& data);

	void deltree(node*);

	std::string print() const;
	std::string print(node*) const;

	~btree();
private:
	void split_node(node* n);
	const size_type kmin;
	const size_type kmax;
	const size_type cmin;
	const size_type cmax;
	node* root;
};

template<class T>
btree<T>::btree(typename btree<T>::size_type m):
	kmin((m-1)/2), kmax(m-1), cmin(m/2), cmax(m), root(nullptr)
{
	root = new node();
	for(typename btree<T>::size_type i = 0; i <= cmin; ++i)
	{
		node* tmp = new node();
		root->children.push_back(tmp);
	}
}

template<class T>
void btree<T>::insert(const T& data)
{
	insert(this->root, data);
}

template<class T>
void btree<T>::insert(typename btree<T>::node* n, const T& data)
{
	if(n != nullptr)
	{
		if(n->data.size() == 0)
		{
			n->data.insert(n->data.begin(), data);
		}
		typename std::vector<T>::iterator it = std::lower_bound(n->data.begin(), n->data.end(), data);
		auto index = std::distance(n->data.begin(), it);

		if(n->children.size() == 0)
			n->data.insert(n->data.begin()+index, data);
		else
			if(index <= (n->children.size()))
			{
				if((*(n->children.begin()+index)) == nullptr)
					n->children.insert(n->children.begin()+index, new node());
			}
			insert(*(n->children.begin()+index), data);
		if(n->data.size() > kmax)
			split_node(n);
	}
}

template<class T>
void btree<T>::split_node(typename btree<T>::node* n)
{
	if(n != nullptr)
	{
		if(n->data.size() >= kmax)
		{
			T mkey = n->data[kmin];
			typename btree<T>::node* b;
			for(typename std::vector<T>::iterator i = n->data.begin()+kmin+1; i != n->data.end(); ++i)
			{
				b->data.push_back(*i);
				n->data.erase(i);
			}
			for(typename std::vector<typename btree<T>::node*>::iterator k = n->children.begin()+cmin; k != n->children.end(); ++k)
			{
				b->children.push_back(*k);
				n->children.erase(k);
			}

			if(n == this->root)
			{
				typename btree<T>::node* nroot = new typename btree<T>::node;
				nroot->children.insert(nroot->children.begin(), n);
				this->root = nroot;
			}

			b->parent = n->parent;

			typename std::vector<T>::iterator pfirst = std::lower_bound(n->parent->data.begin(), n->parent->data.end(), mkey);
			n->parent->data.insert(pfirst, mkey);
			auto kindex = std::distance(n->parent->data.begin(), pfirst);
			n->parent->children.insert(n->parent->children.begin()+kindex+1, b);

			if(n->parent->data.size() >= kmax)
				split_node(n->parent);
		}
	}
}

template<class T>
void btree<T>::deltree(typename btree<T>::node* n)
{
	for(typename std::vector<typename btree<T>::node*>::iterator i = n->children.begin(); i != n->children.end(); ++i)
		deltree(*i);
	delete n;
}

template<class T>
std::string btree<T>::print() const
{
	std::string p = print(this->root);
	std::cout << p;
	return p;
}

template<class T>
std::string btree<T>::print(typename btree<T>::node* n) const
{
	std::stringstream ss;
	if(n->children.size() == 0)
	{
		for(typename std::vector<T>::iterator i = n->data.begin(); i != n->data.end(); ++i)
		{
			ss << (*i);
		}
	}
	else
	{
		for(typename std::vector<typename btree<T>::node*>::iterator i = n->children.begin(); i != n->children.end(); ++i)
		{
			print(*i);
		}
	}

	return ss.str();
}

template<class T>
btree<T>::~btree()
{
	deltree(this->root);
}

#endif