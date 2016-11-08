
#ifndef REDBLACK_TREE_H_
#define REDBLACK_TREE_H_

// 定义红黑树的内存分配模块
#define Rbt_Malloc malloc
#define Rbt_Free free

typedef int(*tdRbtCmpFun)(const void*, const void*);
typedef void(*tdDestroyKeyFun)(void* a);
typedef void(*tdDestroyValueFun)(void* a);

typedef void(*tdForeachFun)(struct RbtIter*);

struct RBtree;

struct RbtIter
{
	void* key;
	void* value;
};

// 创建一个rbtree
struct RBtree* rbt_create(tdRbtCmpFun cmp, 
	tdDestroyKeyFun dk, tdDestroyValueFun dv);

// 销毁一个rbtree
void rbt_destory(struct RBtree*);

// 插入一个key-value
void rbt_insert(struct RBtree*, void*, void*);

// 删除一个key-value
void rbt_remove(struct RBtree*, void*);

// 根据key查找value
void* rbt_find(struct RBtree*, void*);

// 返回rbtree的size
int rbt_size(struct RBtree*);

// 遍历rbtree
void rbt_foreach(struct RBtree*, tdForeachFun f);

#endif // !REDBLACK_TREE_H_

