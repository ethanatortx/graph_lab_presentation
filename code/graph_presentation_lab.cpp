#include "graph_presentation_lab.h"

struct airport
{
	char code[5];
	double longitude;
	double latitude;
};

int main()
{
	graph<typename airport> airport_graph;
	return 0;
}