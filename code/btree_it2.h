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

	struct child;

	struct leaf
	{
		leaf(child* n = nullptr): parent(n) {}
		child* parent;
		std::vector<T> data;
	};

	struct child:
		btree<T>::leaf
	{
		child(child* p = nullptr)
		{
			this->parent = p;
		}
		std::vector<leaf*> children;
	};

public:
	btree(size_type);

	void insert(const T& data);
	void insert(leaf* n, const T& data);
	void insert(child* n, const T& data);

	void deltree(child*);
	void deltree(leaf*);

	void print() const;
	void print(leaf*) const;
	void print(child*) const;

	void print_histogram() const;
	void print_histogram(leaf*) const;
	void print_histogram(child*) const;

	~btree();
private:
	void split_node(leaf* n);
	void split_node(child* n);
	const size_type kmin;
	const size_type kmax;
	const size_type cmin;
	const size_type cmax;
	child* root;
};

template<class T>
btree<T>::btree(typename btree<T>::size_type m):
	kmin((m-1)/2), kmax(m-1), cmin(m/2), cmax(m), root(nullptr)
{
	root = new child;
}

template<class T>
void btree<T>::insert(const T& data)
{
	std::cout << "\"Top level insert\"\n";
	insert(this->root, data);
}

template<class T>
void btree<T>::insert(typename btree<T>::leaf* n, const T& data)
{
	std::cout << "\"Leaf insert\"\n";
	if(n != nullptr)
	{
		std::cout << -1;
		if(n->data.size() == 0)
		{
			std::cout << 0;
			n->data.push_back(data);
		}
		else
		{
			std::cout << 1;
			typename std::vector<T>::iterator i = std::lower_bound(n->data.begin(), n->data.end(), data);
			n->data.insert(i, data);
		}
		std::cout << 2;
		std::cout << "Leaf pre-split: ";
		print(n);
		split_node(n);
	}
}

template<class T>
void btree<T>::insert(typename btree<T>::child* n, const T& data)
{
	std::cout << "\"Child insert\"\n";
	if(n != nullptr)
	{
		if(n->children.size() == 0 && n->data.size() == 0)
		{
			std::cout << "Child nodes empty";
			leaf *a;
			a = new leaf;
			a->parent = n;

			n->children.push_back(a);
			insert(*(n->children.begin()), data);
		}
		else if(n->children.size() == 1)
		{
			std::cout << "Child nodes have one position";
			insert(*(n->children.begin()), data);
		}
		else
		{
			std::cout << "Child ndoes have " << n->children.size() << " positions\n";
			typename std::vector<T>::iterator i = std::lower_bound(n->data.begin(), n->data.end(), data);
			auto index = std::distance(n->data.begin(), i);
			std::cout << "Inserting at child index: " << index << '\n';
			std::cout << "Index child " << index << " has " << n->children[index]->data.size() << " data pieces\n";
			insert(*(n->children.begin()+index), data);
		}
	}
}

template<class T>
btree<T>::~btree()
{
	deltree(this->root);
}

template<class T>
void btree<T>::deltree(typename btree<T>::child* n)
{
	if(n != nullptr)
	{
		for (typename std::vector<leaf*>::iterator i = n->children.begin(); i != n->children.end(); ++i)
		{
			deltree(*i);
		}
		delete n;
	}
}

template<class T>
void btree<T>::deltree(typename btree<T>::leaf* n)
{
	if(n != nullptr)
		delete n;
}

template<class T>
void btree<T>::split_node(typename btree<T>::leaf* n)
{
	if(n != nullptr)
	{
		if(n->parent == nullptr)
		{
			std::cout << "\"Leaf split\"\t\"Create new parent for leaf\"\n";
			child* p = new child;
			n->parent = p;
			p->data.push_back(*(n->data.end()));
			p->children.push_back(n);
		}
		if(n->data.size() > kmax)
		{
			std::cout << "\"Leaf split\"\n";
			std::cout << "\"Leaf split\"\t\"Create new leaf\"\n";
			leaf* b = new leaf;
			b->parent = n->parent;

			std::cout << "\"Leaf split\"\t\"Generate median key to pass up\"\n";
			T midkey = *(n->data.begin()+kmin);
			
			std::cout << "\"Leaf split\"\t\"Pushing second half of data to new leaf\"\n";
			for (typename std::vector<T>::iterator i = n->data.begin()+kmin+1; i != n->data.end(); ++i)
			{
				b->data.push_back(*i);
			}
			n->data.erase(n->data.begin()+kmin+1, n->data.end());


			std::cout << "\"Leaf split\"\t\"Getting vector index for passed up median key\"\n";
			typename std::vector<T>::iterator it = std::lower_bound(n->parent->data.begin(), n->parent->data.end(), midkey);

			std::cout << "\"Leaf split\"\t\"Insert new middle key into parent\"\n";
			std::cout << "\"Leaf split\"\n\"Size of parent data: " << n->parent->data.size() << "\"\n\"Size of parent children:" << n->parent->children.size() << "\"\n";
			int index = std::distance(n->parent->data.begin(), it);
			n->parent->data.insert(it, midkey);
			std::cout << "Parent keys:\t\t";
			for (typename std::vector<T>::iterator i = n->parent->data.begin(); i != n->parent->data.end(); ++i)
			{
				std::cout << *i << ' ';
			}
			std::cout << '\n' << "Left leaf keys:\t\t";
			for (typename std::vector<T>::iterator i = n->data.begin(); i != n->data.end(); ++i)
			{
				std::cout << *i << ' ';
			}
			std::cout << '\n' << "Right leaf keys:\t";
			for (typename std::vector<T>::iterator i = b->data.begin(); i != b->data.end(); ++i)
			{
				std::cout << *i << ' ';
			}
			std::cout << '\n';
			//if(it < n->parent->data.end())\
			{
				std::cout << "\"Leaf split\"\t\"Inserting index of second half leaf to parent at index: " << index << "\"\n";
				n->parent->children.insert(n->parent->children.begin()+index+1, b);
			/*}
			else
			{
				std::cout << "\"Leaf split\"\t\"Inserting index of second half leaf to parent at end\"\n";
				n->parent->children.push_back(b);
			}*/

			std::cout << "\"Leaf split\"\t\"Call split on parent\"\n";
			split_node(n->parent);
		}
	}
}

template<class T>
void btree<T>::split_node(typename btree<T>::child* n)
{
	if(n != nullptr)
	{
		std::cout << "\"Child split\"\n";
		if(n->data.size() > kmax)
		{
			std::cout << "\"Child split\"\t\"Create second child to hold once split\"\n";
			child* b = new child;

			std::cout << "\"Child split\"\t\"Checking if current child has a parent\"\n";
			if(n == this->root)
			{
				std::cout << "\"Child split\"\t\"Create new parent child\"\n";
				child* newch = new child;

				n->parent = newch;

				std::cout << "\"Child split\"\t\"Check if current child is root\"\n";
				std::cout << "\"Child split\"\t\"Set root to newly created child\"\n";
				this->root = newch;
			}

			b->parent = n->parent;

			std::cout << "\"Child split\"\t\"Store new middle key for parent\"\n";
			T midkey = *(n->data.begin()+kmin);

			std::cout << "\"Child split\"\t\"Split keys and store in new node\"\n";
			for(typename std::vector<T>::iterator i = n->data.begin()+kmin+1; i != n->data.end(); ++i)
			{
				b->data.push_back(*i);
			}
			n->data.erase(n->data.begin()+kmin, n->data.end());

			std::cout << "\"Child split\"\t\"Split child nodes and store in new node\"\n";
			for(auto i = n->children.begin()+cmin; i != n->children.end(); ++i)
			{
				b->children.push_back(*i);
			}
			n->children.erase(n->children.begin()+cmin, n->children.end());

			if(n->parent->data.size() == 0 && n->parent->children.size() == 0)
			{
				n->parent->data.push_back(midkey);
				n->parent->children.push_back(n);
				n->parent->children.push_back(b);
			}
			else
			{
				std::cout << "\"Child split\"\t\"Get index for insertion of middle key to parent\"\n";
				typename std::vector<T>::iterator lb = std::lower_bound(n->parent->data.begin(), n->parent->data.end(), midkey);
	
				std::cout << "\"Child split\"\t\"Insert middle key to parent\"\n";
				n->parent->data.insert(lb, midkey);
				std::cout << "\"Child split\"\t\"Check if lb is outside the range of the parent data set\"\n";
				if(lb != n->parent->data.end())
				{
					std::cout << "\"Child split\"\t\"Get index for node insertion\"\n";
					int index = std::distance(n->parent->data.begin(), lb);
					std::cout << "\"Child split\"\t\"Inserting index of second half child to parent at index: " << index << "\"\n";
					n->parent->children.insert(n->parent->children.begin()+index, b);
				}
				else
				{
					std::cout << "\"Child split\"\t\"Inserting index of second half child to parent at end\"\n";
					n->parent->children.push_back(b);
				}
			}

			split_node(n->parent);
		}
	}
}

template<class T>
void btree<T>::print() const
{
	print(this->root);
}

template<class T>
void btree<T>::print(typename btree<T>::leaf* n) const
{
	if(n != nullptr)
	{
		for (typename std::vector<T>::iterator i = n->data.begin(); i != n->data.end(); ++i)
		{
			std::cout << *i << ' ';
		}
	}
	std::cout << '\n';
}

template<class T>
void btree<T>::print(typename btree<T>::child* n) const
{
	if(n != nullptr)
	{
		for (typename std::vector<leaf*>::iterator i = n->children.begin(); i != n->children.end(); ++i)
		{
			print(*i);
		}
	}
}

template<class T>
void btree<T>::print_histogram() const
{
	print_histogram(this->root);
}

template<class T>
void btree<T>::print_histogram(typename btree<T>::leaf* n) const
{
	for (typename std::vector<T>::iterator i = n->data.begin(); i != n->data.end(); ++i)
	{
		std::cout << 'x' << ' ';
	}
	std::cout << '\n';
}

template<class T>
void btree<T>::print_histogram(typename btree<T>::child* n) const
{
	for (auto i = n->children.begin(); i != n->children.end(); ++i)
	{
		print_histogram(*i);
	}
}
#endif