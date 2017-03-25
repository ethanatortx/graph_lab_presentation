#ifndef BTREE_H
#define BTREE_H

#include <vector>
#include <typeinfo>
#include <algorithm>
#include <string>
#include <sstream>

template<class T>
class btree
{
	typedef std::size_t size_type;

	struct node
	{
		node(node* p = nullptr)
		{
			this->parent = p;
		}
		node* parent;
		std::vector<T> data;
		std::vector<node*> children;
	};

public:
	btree(size_type);

	void insert(const T& data);
	void insert(node* n, const T& data);

	void deltree(node*);

	void print() const;
	void print(node*) const;

	void print_histogram() const;
	void print_histogram(node*) const;

	size_type size() const;
	size_type size(node*) const; 

	~btree();
private:
	void node_split(node*);
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
	root = new node;
}

template<class T>
btree<T>::~btree()
{
	deltree(this->root);
}

template<class T>
void btree<T>::deltree(typename btree<T>::node* n)
{
	if(n != nullptr)
	{
		if(n->children.size() == 0)
		{
			delete n;
		}
		else
		{
			for(typename std::vector<node*>::iterator i = n->children.begin(); i != n->children.end(); ++i)
			{
				deltree(*i);
			}
			delete n;
		}
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
		// this means it is a leaf node
		if(n->children.size() == 0)
		{
			if(n->data.size() == 0)
				n->data.push_back(data);
			else
			{
				typename std::vector<T>::iterator i = std::lower_bound(n->data.begin(), n->data.end(), data);
				n->data.insert(i, data);
			}

			// split node
			node_split(n);
		}
		// this means it is a child node
		else
		{
			typename std::vector<T>::iterator i = std::lower_bound(n->data.begin(), n->data.end(), data);
			auto index = std::distance(n->data.begin(), i);

			insert(*(n->children.begin()+index), data);
		}
	}
}

template<class T>
void btree<T>::node_split(typename btree<T>::node* n)
{
	if(n != nullptr)
	{
		// this means it is a leaf node
		if (n->children.size() == 0)
		{
			std::cout << "Leaf split:\n";
			std::cout << "Before split: ";
			print(n);
			std::cout << '\n';
			// check if keys are actually overflowing
			if(n->data.size() > kmax)
			{
				// generate new node for second data half
				node* b = new node;

				// get middle key to pass up
				T midkey = *(n->data.begin()+kmin);

				// get second half of data and push to new node
				for (typename std::vector<T>::iterator i = n->data.begin()+kmin; i != n->data.end(); ++i)
				{
					b->data.push_back(*i);
				}
				// clear second half data from original node
				n->data.erase(n->data.begin()+kmin, n->data.end());

				// check if parent exists
				if(n->parent != nullptr)
				{
					std::cout << "Here! 0\n";
					// get index for middle key insertion of parent
					typename std::vector<T>::iterator i = std::lower_bound(n->parent->data.begin(), n->parent->data.end(), midkey);
					std::cout << "Here! 1\n";
					// get index for node insertion
					auto index = std::distance(n->parent->data.begin(), i);
					std::cout << "Here! 2\n";
					// insert mid key into position at parent
					n->parent->data.insert(i, midkey);
					std::cout << "Here! 3\n" << n->parent->children.size() << ' ' << index+1;
					// insert node pointer of new node into parent at position
					n->parent->children.insert(n->parent->children.begin()+index, b);
					std::cout << "Here! 4\n";
					b->parent = n->parent;
				}
				else
				{
					// create new parent node
					node* p = new node;
					// add mid key as first key to new parent
					p->data.push_back(midkey);
					// push original node and second half node to new parent
					p->children.push_back(n);
					p->children.push_back(b);

					// change parents of nodes
					n->parent = p;		// original
					b->parent = p;		// new (second half node)

					// update btree root node
					if(n == this->root)
						this->root = p;
				}

				std::cout << "Left after split: ";
				print(n);
				std::cout << "\nRight after split: ";
				print(b);
				std::cout << '\n';

				// call split on parent
				node_split(n->parent);
			}
		}
		// this means it is a child node
		else
		{
			std::cout << "Child split:\n";
			// check if keys are actually overflowing
			std::cout << "Node key count: " << n->data.size() << '\n';
			std::cout << "Node child count: " << n->children.size() << '\n';
			if(n->data.size() > kmax)
			{
				std::cout << "Here! 0\n";
				// generate new node to hold split keys/node pointers
				node* b = new node;

				std::cout << "Here! 1\n";
				// get midkey to pass up to parent
				T midkey = *(n->data.begin()+kmin);

				std::cout << "Here! 2\n";
				// get second half of keys, but exclude midkey from copy
				for (typename std::vector<T>::iterator i = n->data.begin()+kmin+1; i != n->data.end(); ++i)
				{
					b->data.push_back(*i);
				}
				std::cout << "Here! 3\n";
				// erase second half of keys from original node including mid key
				n->data.erase(n->data.begin()+kmin, n->data.end());

				std::cout << "Here! 4\n";
				// copy second half of node pointers to new node
				for (typename std::vector<node*>::iterator i = n->children.begin()+cmin; i != n->children.end(); ++i)
				{
					b->children.push_back(*i);
				}
				std::cout << "Here! 5\n";
				// erase second half of node pointers from original node
				n->children.erase(n->children.begin()+cmin, n->children.end());

				// check if parent exists
				if(n->parent != nullptr)
				{
					std::cout << "Here! 6\n";
					// get index of parent to insert mid key at
					std::cout << n->parent->data.size() << '\n';
					typename std::vector<T>::iterator i = std::lower_bound(n->parent->data.begin(), n->parent->data.end(), midkey);
					std::cout << "Here! 7\n";
					// get index for node insertion
					auto index = std::distance(n->parent->data.begin(), i);
					std::cout << "Here! 8\n";
					// insert key into parent
					n->parent->data.insert(i, midkey);
					std::cout << "Here! 9\n";
					// insert node into parent
					std::cout << n->parent->data.size() << ' ' << n->parent->children.size() << ' ' << index+1;
					n->parent->children.insert(n->parent->children.begin()+index+1, b);
				}
				else
				{
					std::cout << "Here! 10\n";
					// create new parent node
					node* p = new node;
					std::cout << "Here! 11\n";
					// push midkey to parent as new node
					p->data.push_back(midkey);
					std::cout << "Here! 12\n";
					// push nodes to parent
					p->children.push_back(n);	// original
					p->children.push_back(b);	// new (second half)

					// set new parent as parent of original and half node
					n->parent = p;
					b->parent = p;

					// update btree root node
					if(n == this->root)
						std::cout << "Here! 13\n";
						this->root = p;
				}

				// call split on parent
				node_split(n->parent);
			}
		}
	}
}

template<class T>
void btree<T>::print() const
{
	print(this->root);
}

template<class T>
void btree<T>::print(typename btree<T>::node* n) const
{
	if(n != nullptr)
	{
		if(n->children.size() == 0)
		{
			for(typename std::vector<T>::iterator i = n->data.begin(); i != n->data.end(); ++i)
			{
				std::cout << *i << ' ';
			}
		}
		else
		{
			for(typename std::vector<node*>::iterator i = n->children.begin(); i != n->children.end(); ++i)
			{
				print(*i);
			}
		}
	}
}

template<class T>
void btree<T>::print_histogram() const
{
	print_histogram(this->root);
}

template<class T>
void btree<T>::print_histogram(typename btree<T>::node* n) const
{
	if(n != nullptr)
	{
		if(n->children.size() == 0)
		{
			std::cout << "Node: ";
			for(typename std::vector<T>::iterator i = n->data.begin(); i != n->data.end(); ++i)
			{
				std::cout << "x ";
			}
			std::cout << '\n';
		}
		else
		{
			for(typename std::vector<node*>::iterator i = n->children.begin(); i != n->children.end(); ++i)
			{
				print_histogram(*i);
			}
		}
	}
}

template<class T>
typename btree<T>::size_type btree<T>::size() const
{
	return size(this->root);
}

template<class T>
typename btree<T>::size_type btree<T>::size(typename btree<T>::node* n) const
{
	if(n != nullptr)
	{
		typename btree<T>::size_type count = 0;
		if(n->children.size() == 0)
		{
			for(typename std::vector<T>::iterator i = n->data.begin(); i != n->data.end(); ++i)
			{
				++count;
			}
		}
		else
		{
			for(typename std::vector<node*>::iterator i = n->children.begin(); i != n->children.end(); ++i)
			{
				count += size(*i);
			}
		}

		return count;
	}
}

#endif