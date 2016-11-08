
#include "rbtree.h"
#include <stdlib.h>
#include <assert.h>

// 定义红黑颜色
#define RBN_COR_BLACK 0
#define RBN_COR_RED 1

// rbtree内部节点
struct rbnode
{
	// same as struct RBT_Iter
	void* key;
	void* value;

	int color;
	rbnode *left, *right, *parent;
};

// 定义全局唯一的叶子节点
static rbnode null_node = { RBN_COR_BLACK };
#define NIL &null_node

// 定义红黑树结构
struct RBtree
{
	int(*cmp_fun)(const void* a, const void* b);
	void(*destroy_key_fun)(void* a);
	void(*destroy_value_fun)(void* a);

	size_t size;
	struct rbnode *root;
	struct rbnode *nil;
};

//
// 左旋
//
static void 
left_ratation(RBtree* rbt, struct rbnode* x)
{
	assert(x->right != NIL);
	struct rbnode* y = x->right;
	x->right = y->left;
	if (y->left != NIL)
		y->left->parent = x;
	y->parent = x->parent;
	if (x->parent == NIL)
		rbt->root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->left = x;
	x->parent = y;
}

// 
// 右旋
//
static void 
right_ratation(RBtree* rbt, struct rbnode* x)
{
	assert(x->left != NIL);
	struct rbnode* y = x->left;
	x->left = y->right;
	if (y->right != NIL)
		y->right->parent = x;
	y->parent = x->parent;
	if (x->parent == NIL)
		rbt->root = y;
	else if (x == x->parent->right)
		x->parent->right = y;
	else
		x->parent->left = y;
	y->right = x;
	x->parent = y;
}

static void 
rbt_insert_fixup(RBtree* rbt, struct rbnode* z)
{
	struct rbnode* y;
	while (z->parent->color == RBN_COR_RED)
	{
		if (z->parent == z->parent->parent->left)
		{
			y = z->parent->parent->right;

			// case 1:
			if (y->color == RBN_COR_RED)
			{
				z->parent->color = RBN_COR_BLACK;
				y->color = RBN_COR_BLACK;
				z->parent->parent->color = RBN_COR_RED;
				z = z->parent->parent;
			}
			else
			{
				// case 2:
				if (z == z->parent->right)
				{
					// change it to case 3
					z = z->parent;
					left_ratation(rbt, z);
				}
				// case 3:
				z->parent->color = RBN_COR_BLACK;
				z->parent->parent->color = RBN_COR_RED;
				right_ratation(rbt, z->parent->parent);
			}
		}
		else // z->parent == z->parent->parent->right
		{
			y = z->parent->parent->left;
			// case 4:
			if (y->color == RBN_COR_RED)
			{
				z->parent->color = RBN_COR_BLACK;
				y->color = RBN_COR_BLACK;
				z->parent->parent->color = RBN_COR_RED;
				z = z->parent->parent;
			}
			else
			{
				// case 5:
				if (z == z->parent->left)
				{
					// change it to case 6
					z = z->parent;
					right_ratation(rbt, z);
				}
				// case 6:
				z->parent->color = RBN_COR_BLACK;
				z->parent->parent->color = RBN_COR_RED;
				left_ratation(rbt, z->parent->parent);
			}
		}
	}
	rbt->root->color = RBN_COR_BLACK;
}

//
// 插入节点算法
//
static void 
rbt_insert_node(RBtree* rbt, struct rbnode* z)
{
	struct rbnode* y = NIL;
	struct rbnode* x = rbt->root;
	while (x != NIL)
	{
		y = x;
		x = z->key < x->key ? x->left : x->right;
	}
	z->parent = y;
	if (y == NIL)
		rbt->root = z;
	else if (z->key < y->key)
		y->left = z;
	else
		y->right = z;
	z->left = NIL;
	z->right = NIL;
	z->color = RBN_COR_RED;
	rbt_insert_fixup(rbt, z);
}

static void 
rbt_transplant(RBtree* rbt, struct rbnode* u, struct rbnode* v)
{
	if (u->parent == NIL)
		rbt->root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;
	v->parent = u->parent;
}

static struct rbnode*
rbt_min_node(struct rbnode* x)
{
	while (x->left != NIL)
		x = x->left;
	return x;
}

static struct rbnode*
rbt_max_node(struct rbnode* x)
{
	while (x->right != NIL)
		x = x->right;
	return x;
}

static void
rbt_delete_fixup(RBtree* rbt, struct rbnode* x)
{
	struct rbnode* w;
	while (x != rbt->root && x->color == RBN_COR_BLACK)
	{
		if (x == x->parent->left)
		{
			w = x->parent->right;
			if (w->color == RBN_COR_RED)
			{
				w->color = RBN_COR_BLACK;
				x->parent->color = RBN_COR_RED;
				left_ratation(rbt, x->parent);
				w = x->parent->right;
			}
			if (w->left->color == RBN_COR_BLACK && w->right->color == RBN_COR_BLACK)
			{
				w->color = RBN_COR_RED;
				x = x->parent;
			}
			else
			{
				if (w->right->color == RBN_COR_BLACK)
				{
					w->left->color = RBN_COR_BLACK;
					w->color = RBN_COR_RED;
					right_ratation(rbt, w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = RBN_COR_BLACK;
				w->right->color = RBN_COR_BLACK;
				left_ratation(rbt, x->parent);
				x = rbt->root;
			}
		}
		else
		{
			w = x->parent->left;
			if (w->color == RBN_COR_RED)
			{
				w->color = RBN_COR_BLACK;
				x->parent->color = RBN_COR_RED;
				right_ratation(rbt, x->parent);
				w = x->parent->left;
			}
			if (w->left->color == RBN_COR_BLACK && w->right->color == RBN_COR_BLACK)
			{
				w->color = RBN_COR_RED;
				x = x->parent;
			}
			else
			{
				if (w->left->color == RBN_COR_BLACK)
				{
					w->right->color = RBN_COR_BLACK;
					w->color = RBN_COR_RED;
					left_ratation(rbt, w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = RBN_COR_BLACK;
				w->left->color = RBN_COR_BLACK;
				right_ratation(rbt, x->parent);
				x = rbt->root;
			}
		}
	}
	x->color = RBN_COR_BLACK;
}

static void 
rbt_delete_node(RBtree* rbt, struct rbnode* z)
{
	struct rbnode *x, *y = z;
	int color = y->color;
	if (z->left == NIL)
	{
		x = z->right;
		rbt_transplant(rbt, z, z->right);
	}
	else if (z->right == NIL)
	{
		x = z->left;
		rbt_transplant(rbt, z, z->left);
	}
	else
	{
		y = rbt_min_node(z->right);
		color = y->color;
		x = y->right;
		if (y->parent != z)
		{
			rbt_transplant(rbt, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		rbt_transplant(rbt, z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}
	if (color == RBN_COR_BLACK)
		rbt_delete_fixup(rbt, x);
	Rbt_Free(z);
}

struct rbnode*
rbt_find_node(struct RBtree* rbt, int k)
{
	struct rbnode* x = rbt->root;
	while (x != NIL && k != x->key)
		x = k < x->key ? x->left : x->right;
	return x;
}

static void 
rbt_dealloc(struct rbnode* r)
{
	if (r == NIL)
		return;
	rbt_dealloc(r->left);
	rbt_dealloc(r->right);
	Rbt_Free(r);
}

static void
rbt_middle_order(struct rbnode* r, void(*f)(struct RBT_Iter*))
{
	if (r == NIL)
		return;
	rbt_middle_order(r->left, f);
	f((RBT_Iter*)r);
	rbt_middle_order(r->right, f);
}

//=========================== 模块接口 ===========================
struct RBtree* 
rbt_create()
{
	struct RBtree* rbt = (struct RBtree*) Rbt_Malloc(sizeof(*rbt));
	if (rbt == NULL)
		return rbt;
	rbt->root = NIL;
	rbt->size = 0;
	return rbt;
}

void 
rbt_destory(struct RBtree* rbt)
{
	if (rbt == NULL)
		return;
	if (rbt->root)
		rbt_dealloc(rbt->root);
	Rbt_Free(rbt);
}

void 
rbt_insert(struct RBtree* rbt, void* k, void* v)
{
	struct rbnode* n = (struct rbnode*)Rbt_Malloc(sizeof(*n));
	if (n == NULL)
		return;
	n->key = k;
	n->value = v;
	rbt_insert_node(rbt, n);
	rbt->size += 1;
}

void
rbt_remove(struct RBtree* rbt, int k)
{
	struct rbnode* z = rbt_find_node(rbt, k);
	if (z == NIL)
		return;
	rbt_delete_node(rbt, z);
	rbt->size -= 1;
}

void* 
rbt_find(struct RBtree* rbt, int k)
{
	struct rbnode* x = rbt_find_node(rbt, k);
	return x == NIL ? NULL : x->value;
}

int 
rbt_size(struct RBtree* rbt)
{
	return rbt->size;
}

void
rbt_foreach(struct RBtree* rbt, void(*f)(struct RBT_Iter*))
{
	rbt_middle_order(rbt->root, f);
}