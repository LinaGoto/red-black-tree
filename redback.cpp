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

  /* finding values */
  //  tree_t *locatetree(int x, int y);
  //tree_t *findtree(int value);
  tree_t *findlast(int value);
  tree_t *findmin(tree_t *tree);
  tree_t *findmax(tree_t *tree);
  //void validate(tree_t *tree, int numblacks);
  //void check(tree_t *child);
  //int getnumblacks(void);

  /* rotation */
  //void rotateR(tree_t *tree);
  //void rotateL(tree_t *tree);

public:
  binarytree(void);
  ~binarytree(void);

  /* adding and deleting */
  //void add (int value);
  //void del (int value);

  /* printing tree */
  //void show(void);

  /* search value */
  //int search(int value);
};

binarytree::binarytree(void) {
  root = NULL;
}

binarytree::~binarytree(void) {
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
void binarytree::rotateR(tree_t *current) {
  tree_t *child, *parent, *leaf;

  if ((current == NULL) || (current -> left == NULL)) return;

  printf("Rotate R for %d\n", current -> value);

  parent = current -> parent;
  child  = current -> left;
  leaf   = child   -> right;

  /* change root */
  if (current == root) {
    root = child;
  } else {
    if (current == parent -> left) {
      parent -> left  = child;
    } else {
      parent -> right = child;
    }
  }

  /* if last node exists */
  if (leaf != NULL) leaf -> parent = current;

  current -> parent = child;
  current -> left   = leaf;
  child   -> parent = parent;
  child   -> right  = current;
}

void binarytree::rotateL(tree_t *current) {
  tree_t *child, *parent, *leaf;

  if ((current == NULL) || (current -> right == NULL)) return;

  printf("Rotate L for %d\n", current -> value);

  parent = current -> parent;
  child  = current -> right;
  leaf   = child   -> left;

  /* change root */
  if (current == root) {
    root = child;
  } else {
    if (current == parent -> left) {
      parent -> left  = child;
    } else {
      parent -> right = child;
    }
  }
