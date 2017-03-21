#ifndef BTREE_H
#define BTREE_H

#include <vector>
#include <algorithm>

template<class T>
class btree
{
	struct node
	{
		node():
			kcount(0), ccount(0)
		{
			keys = std::valarray<T>(kmax);
			children = std::valarray<node*>(cmax);
			parent = nullptr;
		}

		std::size_t kcount;
		std::valarray<T> keys;
		std::size_t ccount;
		std::valarray<node*> children;
		node* parent;
	};

public:
	btree(std::size_t _m);
	btree(const btree<T>& other);

	bool empty() const;

	std::size_t insert(const T&);
	node* insert(node*, const T&);

	void clear();

	std::size_t erase(const T&);
	node* erase(node*, const T&);
	std::size_t deltree(node*);

	std::vector<T> data_to_vector() const;
	std::vector<T> data_to_vector(std::vector<T>&, node*) const;
	std::ostringstream format() const;

private:
	// child count max/min
	const std::size_t cmax;		// maximum children per node
	const std::size_t cmin;		// minimum children per node
	// key count max
	const std::size_t kmax;		// maximum keys per node
	// root node
	node* root;
};

template<class T>
btree<T>::btree(unsigned _m): 
	cmax(_m), cmin(_m/2),
	kmax(_m-1),
	root(nullptr) {}

template<class T>
btree<T>::btree(const btree<T>& other):
	cmax(other.cmax), cmin(other.cmin),
	kmax(other.kmax),
	root(nullptr)
{
	std::vector<T> dvec = other.data_to_vector();
	root = new node();
	for (std::vector<T>::iterator i = dvec.begin(); i != dvec.end(); ++i)
	{
		insert(*i);
	}
}

template<class T>
std::vector<T>& btree<T>::data_to_vector() const
{
	std::vector<T> v;
	return data_to_vector(v, this->root);
}

template<class T>
std::vector<T>& btree<T>::data_to_vector(std::vector<T>& vref, node* n)
{
	vref.push_back(n->data);
	for(std::valarray<node*>::iterator i = n->children.begin(); i != n->children.end(); ++i)
	{
		data_to_vector(vref, (*i));
	}
	return vref;
}

template<class T>
unsigned btree<T>::insert(const T& d)
{
	insert(d, this->root);
}

template<class T>
typename btree<T>::node* btree<T>::insert(typename btree<T>::node* n, const T& d)
{
	if(n->kcount < kmax)
		n->keys[(n->kcount)++] = d;
	else
	{
		std::valarray<T>::iterator i = std::lower_bound(std::begin(n->keys), std::begin(n->keys)+kcountr, d);

	}
}

#endif