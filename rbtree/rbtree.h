
#ifndef REDBLACK_TREE_H_
#define REDBLACK_TREE_H_

// �����������ڴ����ģ��
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

// ����һ��rbtree
struct RBtree* rbt_create(tdRbtCmpFun cmp, 
	tdRbtDestroyKeyFun dk, tdRbtDestroyValueFun dv);

// ����һ��rbtree
void rbt_destory(struct RBtree*);

// ����һ��key-value
void rbt_insert(struct RBtree*, void*, void*);

// ɾ��һ��key-value
void rbt_remove(struct RBtree*, void*);

// ����key����value
void* rbt_find(struct RBtree*, void*);

// ����rbtree��size
int rbt_size(struct RBtree*);

// ����rbtree
void rbt_foreach(struct RBtree*, tdRbtForeachFun f);

#endif // !REDBLACK_TREE_H_

