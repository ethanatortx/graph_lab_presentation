#include <iostream>
#include "btree_it2.h"

int main()
{
	btree<int> b(2);
	b.insert(5);
	b.insert(6);
	b.insert(2);

	b.print();
	return 0;
}