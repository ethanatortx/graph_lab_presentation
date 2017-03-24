#include <iostream>
#include <cstdlib>
#include "btree_it2.h"

int main()
{
	btree<int> b(3);
	for(int i = 0; i < 100; ++i)
	{
		std::cout << "\nTest " << i << '\n';
		b.insert(i);
		std::cout << "Data in tree: \n";
		b.print();
		std::cout << "Tree histogram: \n";
		b.print_histogram();
	}

	b.print();

	return 0;
}