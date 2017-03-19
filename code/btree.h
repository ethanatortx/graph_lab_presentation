#ifndef BTREE_H
#define BTREE_H

// self balancing binary search tree
template<class T>
class binary_tree
{
	static const char default_print_delim = ' ';
	typedef T value_type;
	typedef std::size_t size_type;

	struct node
	{
		node(const value_type& x, node* l = nullptr, node* r = nullptr):
			data(x), left(l), right(r) {}

		value_type data;
		node* left;
		node* right;
	};

public:
	binary_tree();
	binary_tree(const binary_tree&);

	void insert(const value_type&);
	void insert(const value_type&, node*);

	size_type erase(const value_type&);
	size_type deltree(node*);



private:
	

	node* root;
};
#endif