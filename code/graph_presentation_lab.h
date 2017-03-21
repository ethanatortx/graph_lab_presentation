#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <map>
#include <cstdlib>
#include <limits>
#include <functional>
#include "list.h"
#include "error.h"

template<class T>
class digraph
{
public:
	typedef T value_type;

	struct edge;

	struct node
	{
		node(const value_type& x): data(x)
			{ reset(); }

		// reset variables used by shortest path algorithm
		void reset()
			{ dist = std::numeric_limits<double>::infinity; prev = nullptr; scratch = 0; }

		// unchanged by shortest path
		value_type data;		// name/data of node
		list<edge> adjacent;	// edges connecting to adjacent nodes

		// used only for shortest path algorithm
		double dist;			// total cost to traverse from origin node to this node
		node *prev; 			// previous node in shortest path
		int scratch;			// extra var used by shortest path
	};

	struct edge
	{
		edge(node* n = nullptr, double c = 0.0):
			dest(n), cost(c) {}

		node *dest;		// destination of edge
		double cost;	// cost to traverse edge
	};

	digraph() {}
	~digraph();

	void add_node(const value_type& data);
	void add_node(const value_type& data, const list<edge>& adj);
	void remove_node(const value_type& data);

	list<edge> get_adjacent_to(const value_type& data);

	std::pair<list<node*>, long double> shortest_path(const value_type& origin, const value_type& dest);

	double is_edge(const value_type& source, const value_type& dest);
	void add_edge(const value_type& source,
		const value_type& dest, double cost);
	void remove_edge(const value_type& source,
		const value_type& dest);

private:
	typedef std::map<value_type, node*, std::less<value_type> > nmap;
	nmap node_map;
};

#endif
