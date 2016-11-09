
#include "rbtree.h"
#include <stdlib.h>
#include <assert.h>

// 定义红黑颜色
#define RBN_COR_BLACK 0
#define RBN_COR_RED 1

struct rbnode
{
	// same as struct RbtIter
	void* key;
	void* value;

	int color;
	rbnode *left, *right, *parent;
};

struct RBtree
{
	tdRbtCmpFun cmp;
	tdRbtDestroyKeyFun destroy_key;
	tdRbtDestroyValueFun destroy_value;

	size_t size;
	struct rbnode *root;
	struct rbnode *nil;
};

static void 
left_ratation(RBtree* rbt, struct rbnode* x)
{
	assert(x->right != rbt->nil);
	struct rbnode* y = x->right;
	x->right = y->left;
	if (y->left != rbt->nil)
		y->left->parent = x;
	y->parent = x->parent;
	if (x->parent == rbt->nil)
		rbt->root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->left = x;
	x->parent = y;
}

static void 
right_ratation(RBtree* rbt, struct rbnode* x)
{
	assert(x->left != rbt->nil);
	struct rbnode* y = x->left;
	x->left = y->right;
	if (y->right != rbt->nil)
		y->right->parent = x;
	y->parent = x->parent;
	if (x->parent == rbt->nil)
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

static void 
rbt_insert_node(RBtree* rbt, struct rbnode* z)
{
	int r;
	struct rbnode* y = rbt->nil;
	struct rbnode* x = rbt->root;
	while (x != rbt->nil)
	{
		y = x;
		r = (rbt->cmp)(z->key, x->key);
		x = (r < 0) ? x->left : x->right;
	}
	z->parent = y;
	if (y == rbt->nil)
		rbt->root = z;
	else if (z->key < y->key)
		y->left = z;
	else
		y->right = z;
	z->left = rbt->nil;
	z->right = rbt->nil;
	z->color = RBN_COR_RED;
	rbt_insert_fixup(rbt, z);
}

static void 
rbt_transplant(RBtree* rbt, struct rbnode* u, struct rbnode* v)
{
	if (u->parent == rbt->nil)
		rbt->root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;
	v->parent = u->parent;
}

static struct rbnode*
rbt_min_node(RBtree* rbt, struct rbnode* x)
{
	while (x->left != rbt->nil)
		x = x->left;
	return x;
}

static struct rbnode*
rbt_max_node(RBtree* rbt, struct rbnode* x)
{
	while (x->right != rbt->nil)
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
	if (z->left == rbt->nil)
	{
		x = z->right;
		rbt_transplant(rbt, z, z->right);
	}
	else if (z->right == rbt->nil)
	{
		x = z->left;
		rbt_transplant(rbt, z, z->left);
	}
	else
	{
		y = rbt_min_node(rbt, z->right);
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
rbt_find_node(struct RBtree* rbt, void* k)
{
	int r;
	struct rbnode* x = rbt->root;
	while (x != rbt->nil)
	{
		r = (rbt->cmp)(k, x->key);
		if (r < 0)
			x = x->left;
		else if (r > 0)
			x = x->right;
		else
			break;
	}
	return x;
}

static void 
rbt_destroy_helper(struct RBtree* rbt, struct rbnode* r)
{
	if (r == rbt->nil)
		return;
	rbt_destroy_helper(rbt, r->left);
	rbt_destroy_helper(rbt, r->right);
	Rbt_Free(r);
}

static void
rbt_middle_order(struct RBtree* rbt, 
	struct rbnode* r, tdRbtForeachFun f)
{
	if (r == rbt->nil)
		return;
	rbt_middle_order(rbt, r->left, f);
	f((RbtIter*)r);
	rbt_middle_order(rbt, r->right, f);
}

//=========================== 模块接口 ===========================
struct RBtree* 
rbt_create(tdRbtCmpFun cmp,
	tdRbtDestroyKeyFun dk, tdRbtDestroyValueFun dv)
{
	struct RBtree* rbt;
	struct rbnode* nil;
	rbt = (struct RBtree*) Rbt_Malloc(sizeof(*rbt));
	assert(rbt != NULL);
	nil = (struct rbnode*)Rbt_Malloc(sizeof(*nil));
	assert(nil != NULL);

	nil->color = RBN_COR_BLACK;
	nil->key = nil->value = NULL;
	nil->parent = nil->left = nil->right = NULL;

	rbt->cmp = cmp;
	rbt->destroy_key = dk;
	rbt->destroy_value = dv;
	rbt->root = rbt->nil = nil;
	rbt->size = 0;
	return rbt;
}

void 
rbt_destory(struct RBtree* rbt)
{
	if (rbt == NULL)
		return;
	if (rbt->root)
		rbt_destroy_helper(rbt, rbt->root);
	Rbt_Free(rbt->nil);
	Rbt_Free(rbt);
}

void 
rbt_insert(struct RBtree* rbt, void* k, void* v)
{
	struct rbnode* n = (struct rbnode*)Rbt_Malloc(sizeof(*n));
	assert(n != NULL);
	n->key = k;
	n->value = v;
	rbt_insert_node(rbt, n);
	rbt->size += 1;
}

void
rbt_remove(struct RBtree* rbt, void* k)
{
	struct rbnode* z = rbt_find_node(rbt, k);
	if (z == rbt->nil)
		return;
	rbt_delete_node(rbt, z);
	rbt->size -= 1;
}

void* 
rbt_find(struct RBtree* rbt, void* k)
{
	struct rbnode* x = rbt_find_node(rbt, k);
	return x == rbt->nil ? NULL : x->value;
}

int 
rbt_size(struct RBtree* rbt)
{
	return rbt->size;
}

void
rbt_foreach(struct RBtree* rbt, tdRbtForeachFun f)
{
	rbt_middle_order(rbt, rbt->root, f);
}