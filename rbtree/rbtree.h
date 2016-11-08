
#ifndef REDBLACK_TREE_H_
#define REDBLACK_TREE_H_

// �����������ڴ����ģ��
#define Rbt_Malloc malloc
#define Rbt_Free free

struct RBtree;
struct RBT_Iter
{
	void* key;
	void* value;
};

// ����һ��rbtree
struct RBtree* rbt_create();

// ����һ��rbtree
void rbt_destory(struct RBtree*);

// ����һ��key-value
void rbt_insert(struct RBtree*, void*, void*);

// ɾ��һ��key-value
void rbt_remove(struct RBtree*, int);

// ����key����value
void* rbt_find(struct RBtree*, int);

// ����rbtree��size
int rbt_size(struct RBtree*);

// ����rbtree
void rbt_foreach(struct RBtree*, void(*f)(struct RBT_Iter*));

#endif // !REDBLACK_TREE_H_

