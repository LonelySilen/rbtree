
#include "rbtree.h"
#include <iostream>
#include <stdint.h>

int cmp(const void* x, const void* y)
{
	return (int64_t)x - (int64_t)y;
}

void destroy_key(void* a)
{
	std::cout << "destroy key: " << a << std::endl;
}

void destroy_value(void* a)
{
	std::cout << "destroy val: " << a << std::endl;
}

int main()
{
	struct RBtree* rbt = rbt_create(cmp, destroy_key, destroy_value);
	rbt_insert(rbt, (void*)1, (void*)1);
	rbt_insert(rbt, (void*)3, (void*)3);
	rbt_insert(rbt, (void*)5, (void*)5);
	rbt_insert(rbt, (void*)7, (void*)7);

	int64_t r1 = (int64_t)rbt_find(rbt, (void*)3);
	int64_t r2 = (int64_t)rbt_find(rbt, (void*)4);

	rbt_remove(rbt, (void*)3);
	int64_t r3 = (int64_t)rbt_find(rbt, (void*)3);

	std::cout << r1 << " " << r2 << " " << r3 << std::endl;

	rbt_foreach(rbt, [](struct RbtIter* it)->void { std::cout << it->value << ", "; });
	std::cout << std::endl;
	std::cout << rbt_size(rbt) << std::endl;

	rbt_destory(rbt);
	return 0;
}