#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

enum { black, red };

typedef struct tree_s {
  int value;
  int color;
  tree_s *parent, *left, *right;
} tree_t;

class binarytree {
private:
  tree_t *root;

  /* supplimental code */
  int getdepth(tree_t *tree, int depth);
  int leftright(int x, int y, int depth);

  tree_t *locatetree(int x, int y);
  tree_t *findtree(int value);
  tree_t *findlast(int value);
  tree_t *findmin(tree_t *tree);
  tree_t *findmax(tree_t *tree);
  void validate(tree_t *tree);
  
public:
   binarytree(void);
  ~binarytree(void);

  /* operation */
  void add (int value);
  void del (int value);

  /* show tree */
  void show(void);
};

binarytree::binarytree(void) {
  root = NULL;
}

binarytree::~binarytree(void) {
  tree_t *tree;

  show();
  printf(" ----\n");

  while (root != NULL) {
    switch (rand() % 5) {
    case 0:
      tree = findmin(root);
      break;
    case 1:
      tree = findmax(root);
      break;
#if 1
    case 2:
      tree = (root -> left != NULL)? root -> left : root;
      break;
    case 3:
      tree = (root -> right != NULL)? root -> right : root;
      break;
#endif
    default:
      tree = root;
      break;
    }

    printf("Deleting %4d\n", tree -> value);
    del(tree -> value);
    show();
    printf(" ----\n");
  }
}

void binarytree::validate(tree_t *tree) {
  if (tree == NULL) return;

  if ((tree == root) && (tree -> parent != NULL)) printf("Wrong Root parent.\n");

  if (tree -> left != NULL) {
    if (tree -> left -> parent != tree) printf("Wrong Left parent.\n");
    if (tree -> left -> value > tree -> value) printf("Wrong left value.\n");
    validate(tree -> left);
  }
  
  if (tree -> right != NULL) {
    if (tree -> right -> parent != tree) printf("Wrong Right parent.\n");
    if (tree -> right -> value < tree -> value) printf("Wrong right value.\n");
    validate(tree -> right);
  }
}

/* find the tree that has the value */
tree_t *binarytree::findtree(int value) {
  tree_t *cur = root;
  
  /* finding value */
  while ((cur != NULL) && (cur -> value != value)) {
    if (value < cur -> value) cur = cur -> left; else cur = cur -> right;
  }
  return cur;
}

/* find the tree to add */
tree_t *binarytree::findlast(int value) {
  tree_t *cur = root;
  
  /* finding the value */
  while (cur != NULL) {
    /* value is already defined */
    if (value == cur -> value) break;
    /* if left is terminated */
    if ((value < cur -> value) && (cur -> left  == NULL)) break;
    /* if right is terminated */
    if ((value > cur -> value) && (cur -> right == NULL)) break;
    /* update pointer */
    if (value < cur -> value) cur = cur -> left; else cur = cur -> right;
  }

  return cur;
}

/* find the mininum value; very right tree will be the maximum */
tree_t *binarytree::findmin(tree_t *target) {
  while ((target != NULL) && (target -> left != NULL)) target = target -> left;
  return target;
}

/* find the maximum value; very left tree will be the minimum */
tree_t *binarytree::findmax(tree_t *target) {
  while ((target != NULL) && (target -> right != NULL)) target = target -> right;
  return target;
}

void binarytree::add(int value) {
  tree_t *cur, *data;

  /* set up new data */
  data = new tree_t;
  data -> value  = value;
  data -> color  = (rand() % 2 == 0)? red : black;
  data -> parent = NULL;
  data -> left   = NULL;
  data -> right  = NULL;

  cur = findlast(value);

  if (cur == NULL) {
    root = data;
  } else {
    if (value < cur -> value) cur -> left = data; else cur -> right = data;
    data -> parent = cur;
  }

  validate(root);
}

void binarytree::del(int value) {
  tree_t *cur = root;
  tree_t *parent, *maxtree;
  
  /* find the poiter */
  cur = findtree(value);

  /* value was not found */
  if (cur == NULL) return;

  if (cur == root) {
    /* if it's root */
    if ((cur -> left == NULL) && (cur -> right == NULL)) {
      /* no leaves, simply delete it */
      root = NULL;
    } else if (cur -> right == NULL) {
      /* left only */
      root  = cur -> left;
      cur -> left -> parent = NULL;
    } else if (cur -> left == NULL) {
      /* right only */
      root  = cur -> right;
      cur -> right -> parent = NULL;
    } else {
      /* two leaves */
      /* find the maximum tree */
      maxtree = findmax(cur -> left);

      /* max tree is next pointer */
      if (cur -> left == maxtree) {
	maxtree -> right = cur -> right;
	if (maxtree -> right != NULL) maxtree -> right -> parent = maxtree;
	maxtree -> parent = NULL;
      } else {
	maxtree -> parent -> right = maxtree -> left;
	if (maxtree -> left != NULL) maxtree -> left -> parent = maxtree -> parent;
	maxtree -> left = cur -> left;
	if (cur -> left != NULL) cur -> left -> parent = maxtree;
	maxtree -> right = cur -> right;
	if (cur -> right != NULL) cur -> right -> parent = maxtree;
	maxtree -> parent = NULL;
      }
      root = maxtree;
    }
  } else {
    /* if not root, find parent */
    parent = cur -> parent;

    if ((cur -> left == NULL) && (cur -> right == NULL)) {
      /* no leaves, simply delete it */
      if (value < parent -> value) parent -> left = NULL; else parent -> right = NULL;
    } else if (cur -> right == NULL) {
      /* left only */
      if (value < parent -> value) parent -> left = cur -> left; else parent -> right = cur -> left;
      cur -> left -> parent = parent;
    } else if (cur -> left == NULL) {
      /* right only */
      if (value < parent -> value) parent -> left = cur -> right; else parent -> right = cur -> right;
      cur -> right -> parent = parent;
    } else {
      /* two leaves */
      /* find the maximum tree */
      maxtree = findmax(cur -> left);

      /* max tree is next pointer */
      if (cur -> left == maxtree) {
	maxtree -> right = cur -> right;
	if (maxtree -> right != NULL) maxtree -> right -> parent = maxtree;
	maxtree -> parent = parent;
      } else {
	maxtree -> parent -> right = maxtree -> left;
	if (maxtree -> left != NULL) maxtree -> left -> parent = maxtree -> parent;
	maxtree -> left  = cur -> left;
	if (cur -> left != NULL) cur -> left -> parent = maxtree;
	maxtree -> right = cur -> right;
	if (cur -> right != NULL) cur -> right -> parent = maxtree;
	maxtree -> parent = parent;
      }
      
      if (value < parent -> value) parent -> left = maxtree; else parent -> right = maxtree;
    }
  }
  
  delete cur;
  validate(root);
}

static int getparent(int i) { return (i - 1) / 2; }
static int getwidth (int i) { return (1 << i); }

int binarytree::getdepth(tree_t *tree, int depth) {
  static int maxdepth;

  if (depth == 0) maxdepth = 0;

  if ((tree -> left == NULL) && (tree -> right == NULL)) {
    if (maxdepth < depth) maxdepth = depth;
  } else {
    if (tree -> left  != NULL) getdepth(tree -> left,  depth + 1);
    if (tree -> right != NULL) getdepth(tree -> right, depth + 1);
  }
  return maxdepth;
}

int binarytree::leftright(int x, int y, int relativedepth) {
  int target = getwidth(x) + y - 1;
  int amIleft = 0;

  while (relativedepth > 0) {
    amIleft = ((target & 1) == 0);
    target = getparent(target);
    relativedepth --;
  }
  return amIleft;
}

tree_t *binarytree::locatetree(int x, int y) {
  tree_t *tree = root;
  int depth = x;

  while ((tree != NULL) && (depth > 0)) {
    if (leftright(x, y, depth) == 0) {
      tree = tree -> left;
    } else {
      tree = tree -> right;
    }
    depth --;
  }
  
  return tree;
}

void binarytree::show(void) {
  int d, w, i, z;
  tree_t *tree;
  
  if (root == NULL) return;

  for (d = 0; d < getdepth(root, 0) + 1; d ++) {
    int gap = (getwidth(getdepth(root, 0) - d) - 1) * 5;
    for (z = 0; z < gap / 2; z ++) printf(" ");
    for (w = 0; w < getwidth(d); w ++) {
      tree = locatetree(d, w);
      if (tree == NULL) {
	printf("  x  ");
      } else {
	printf("%4d%c", tree -> value, (tree -> color == black)? ' ' : '*');
      }
      for (z = 0; z < gap; z ++) printf(" ");
    }
    printf("\n");
  }
  printf("\n");
}

int main(void) {
  int i;
  int array[] = {783, 386, 277, 415, 293, 835, 886, 992, 149, 921, 862, 527, 190, 559, 263, 426,  40, 926, 3, 55, 845, -1};
  
  /* encoding */
  binarytree binarytree;
  
  i = 0;
  while (array[i] >= 0) binarytree.add(array[i++]);

  return 0;
}
