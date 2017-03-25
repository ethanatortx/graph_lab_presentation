#include <iostream>
#include <cstdlib>
#include "btree_it2.h"

int main()
{
	btree<int> b(3);
	int k;
	for(int i = 0; i < 100; ++i)
	{
		std::cout << "\nTest " << i << '\n';
		k = std::rand();
		std::cout << "Inserting " << k << " into tree\n";
		b.insert(k);
		std::cout << "Data in tree: \n";
		b.print();
		std::cout << "\nTree histogram: \n";
		b.print_histogram();
		std::cout << "\nSize of tree: \n" << b.size() << '\n';
	}

	b.print();

	return 0;
}