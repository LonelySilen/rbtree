
#include "rbtree.h"
#include <iostream>
#include <stdint.h>

int main()
{
	struct RBtree* rbt = rbt_new();
	rbt_insert_pair(rbt, 1, (void*)(1));
	rbt_insert_pair(rbt, 3, (void*)(3));
	rbt_insert_pair(rbt, 5, (void*)(5));
	rbt_insert_pair(rbt, 7, (void*)(7));

	int64_t r1 = (int64_t)rbt_find(rbt, 3);
	int64_t r2 = (int64_t)rbt_find(rbt, 4);

	rbt_delete_pair(rbt, 3);
	int64_t r3 = (int64_t)rbt_find(rbt, 3);

	std::cout << r1 << " " << r2 << " " << r3 << std::endl;

	rbt_foreach(rbt, [](struct RBT_Iter* it)->void { std::cout << it->value << ", "; });
	std::cout << std::endl;
	std::cout << rbt_size(rbt) << std::endl;
	return 0;
}