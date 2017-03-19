#include "graph_presentation_lab.h"

int main()
{
	graph g;
	g.emplace("Ethan");
	g.emplace("Julia");
	g.emplace("Ashley");
	g.emplace("Tim");
	g.emplace("James");

	g.add_edge("Ethan", "Julia", 3.0);
	g.add_edge("Ethan", "James", 4.0);
	
	g.add_edge("Julia", "Ashley", 5.0);
	g.add_edge("Julia", "Tim", 2.0);
	g.add_edge("Julia", "Ethan", 3.0);

	g.add_edge("Ashley", "Ethan", 2.0);
	g.add_edge("Ashley", "Tim", 3.0);

	g.add_edge("Tim", "James", 2.0);
	g.add_edge("Tim", "Julia", 1.0);

	g.add_edge("James", "Ethan", 4.0);
	g.add_edge("James", "Ashley", 5.0);

	g.print();

	return 0;
}