#include <iostream>
#include "graph_presentation_lab.h"

graph::graph()
{

}

void graph::clear()
{
	node_map.clear();
}

bool graph::empty() const
{
	return node_map.empty();
}

void graph::emplace(const string& nm)
{
	node_map[nm];
}

void graph::erase(const string& nm)
{
	node_map.erase(nm);
}

list<typename graph::edge>& graph::adjacent(const string& nm)
{
	return node_map[nm];
}

void graph::add_edge(const string& o, const string& e, double c)
{
	node_map[o].push_back(typename graph::edge(e, c));
}

void graph::remove_edge(const string& o, const string& e)
{
	for(typename list<edge>::iterator it = node_map[o].begin(); it != node_map[o].end(); ++it)
	{
		if(it->end == e)
		{
			node_map[o].erase(it);
		}
	}
}

void graph::print()
{
	for (typename graph::nmap::const_iterator i = node_map.begin(); i != node_map.end(); ++i)
	{
		std::cout << "Node: " << i->first << '\t' << "Adjacent:\n";
		for (typename list<edge>::const_iterator k = (i->second).begin(); k != (i->second).end(); ++k)
		{
			std::cout << k->end << "\t" << k->cost << '\n';
		}
	}
}