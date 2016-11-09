
#ifndef REDBLACK_TREE_H_
#define REDBLACK_TREE_H_

// 定义红黑树的内存分配模块
#define Rbt_Malloc malloc
#define Rbt_Free free

typedef int(*tdRbtCmpFun)(const void*, const void*);
typedef void(*tdRbtDestroyKeyFun)(void* a);
typedef void(*tdRbtDestroyValueFun)(void* a);

typedef void(*tdRbtForeachFun)(struct RbtIter*);

struct RBtree;

struct RbtIter
{
	void* key;
	void* value;
};

struct RBtree* rbt_create(tdRbtCmpFun cmp, 
	tdRbtDestroyKeyFun dk, tdRbtDestroyValueFun dv);
void rbt_destory(struct RBtree*);
void rbt_insert(struct RBtree*, void*, void*);
void rbt_remove(struct RBtree*, void*);
void* rbt_find(struct RBtree*, void*);
int rbt_size(struct RBtree*);
void rbt_foreach(struct RBtree*, tdRbtForeachFun f);

#endif // !REDBLACK_TREE_H_