#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>

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
  void validate(tree_t *tree, int numblacks);
  void check(tree_t *child);
  int getnumblacks(void);

  void rotateR(tree_t *tree);
  void rotateL(tree_t *tree);
  
public:
   binarytree(void);
  ~binarytree(void);

  /* operation */
  void add (int value);
  void del (int value);

  /* show tree */
  void show(void);

  /* search */
  int search(int value);
};

binarytree::binarytree(void) {
  root = NULL;
}

binarytree::~binarytree(void) {
}

int binarytree::search(int value) {
  return (findtree(value) != NULL);
}

int binarytree::getnumblacks(void) {
  int num = 0;
  tree_t *tree = root;

  while (tree != NULL) {
    if (tree -> color == black) num ++;
    tree = tree -> left;
  }
  return num;
}

void binarytree::validate(tree_t *tree, int numblacks) {
  if (tree == NULL) return;

  if ((tree == root) && (tree -> parent != NULL)) { cout << "Wrong Root parent" << endl; exit(1); }

  /* color should be red or black */
  if ((tree -> color != red) && (tree -> color != black)) { cout << "Wrong node color " << tree -> color << endl; exit(2); }
  /* root color should be black */
  if ((tree == root) && (tree -> color != black)) { cout << "Wrong Root color" << endl; exit(3); }
  /* if node is red, leaf should be black */
  if (tree -> color == red) {
    if ((tree -> left  != NULL) && (tree -> left  -> color != black)) { cout << "Wrong left color" << endl; exit(4); }
    if ((tree -> right != NULL) && (tree -> right -> color != black)) { cout << "Wrong right color" << endl; exit(5); }
  }
  
  /* if node is black, increment numblacks */
  if (tree -> color == black) numblacks ++;

  if (tree -> left != NULL) {
    if (tree -> left -> parent != tree) { cout << "Wrong Left parent" << endl; exit(6); }
    if (tree -> left -> value > tree -> value) { cout << "Wrong left value" << endl; exit(7); }
    validate(tree -> left, numblacks);
  } else {
    if (getnumblacks() !=  numblacks) {
      cout << "Wrong number of blacks for left  : " << numblacks << "(expected: " << getnumblacks() << ")" << endl;
      exit(8);
    }
  }
  
  if (tree -> right != NULL) {
    if (tree -> right -> parent != tree) { cout << "Wrong Right parent" << endl; exit(9); }
    if (tree -> right -> value < tree -> value) { cout << "Wrong right value" << endl; exit(10); }
    validate(tree -> right, numblacks);
  } else {
    if (getnumblacks() !=  numblacks) {
      cout << "Wrong number of blacks for right  : " << numblacks << "(expected: " << getnumblacks() << ")" << endl;
      exit(11);
    }
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

void binarytree::rotateR(tree_t *current) {
  tree_t *child, *parent, *leaf;

  if ((current == NULL) || (current -> left == NULL)) return;

  //  cout << "Rotate R for " << current -> value << endl;

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

  //  cout << "Rotate L for " << current -> value << endl;

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

  /* if last node exists */
  if (leaf != NULL) leaf -> parent = current;

  current -> parent = child;
  current -> right  = leaf;
  child   -> parent = parent;
  child   -> left   = current;
}

void binarytree::check(tree_t *current) {

  if ((current == NULL) || (current -> color == black)) return;

  /* if parent is black */
  if (current -> parent -> color == black) return;

  /* parent is red, then grandparent will be black */
  if (current -> parent == current -> parent -> parent -> left) {
    /* Left */
    if ((current -> parent -> parent -> right != NULL) && (current -> parent -> parent -> right -> color == red)) {
      /* if uncle is read, change color */
      current -> parent -> color = black;
      current -> parent -> parent -> right -> color = black;
      if (current -> parent -> parent == root) {
	current -> parent -> parent -> color = black;
      } else {
	current -> parent -> parent -> color = red;
      }
      check(current -> parent -> parent);
    } else {
      if (current == current -> parent -> right) {
	rotateL(current -> parent);
	current = current -> left;
      }
      rotateR(current -> parent -> parent);
      current -> parent -> color = black;
      if (current -> parent -> right != NULL) current -> parent -> right -> color = red;
    }
  } else {
    /* Right */
    if ((current -> parent -> parent -> left != NULL) && (current -> parent -> parent -> left -> color == red)) {
      /* if uncle is read, change color */
      current -> parent -> color = black;
      current -> parent -> parent -> left -> color = black;
      if (current -> parent -> parent == root) {
	current -> parent -> parent -> color = black;
      } else {
	current -> parent -> parent -> color = red;
      }
      check(current -> parent -> parent);
    } else {
      if (current == current -> parent -> left) {
	rotateR(current -> parent);
	current = current -> right;
      }
      rotateL(current -> parent -> parent);
      current -> parent -> color = black;
      if (current -> parent -> left != NULL) current -> parent -> left -> color = red;
    }
  }
}

void binarytree::add(int value) {
  tree_t *child, *data;

  child = findlast(value);

  if ((child != NULL) && (child -> value == value)) return;
  
  /* set up new data */
  data = new tree_t;
  data -> value  = value;
  data -> color  = red;
  data -> parent = NULL;
  data -> left   = NULL;
  data -> right  = NULL;

  if (child == NULL) {
    root = data;
    /* force to be black */
    data -> color = black;
  } else {
    if (value < child -> value) child -> left = data; else child -> right = data;
    data -> parent = child;
  }

  if ((data -> parent != NULL) && (data -> parent -> color == red)) check(data);

  validate(root, 0);
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
  cout << "   ======= Deleted " << value << " =======" << endl;
  show();
  validate(root, 0);
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
    for (z = 0; z < gap / 2; z ++) cout << " ";
    for (w = 0; w < getwidth(d); w ++) {
      tree = locatetree(d, w);
      if (tree == NULL) {
	cout << "     ";
      } else {
	//	cout << "%4d%c", tree -> value, (tree -> color == black)? ' ' : '*');
	cout << setw(4) << tree -> value;
	if (tree -> color == black) {
	  cout << ' ';
	} else {
	  cout << '*';
	}
      }
      for (z = 0; z < gap; z ++) cout << " ";
    }
    cout << endl;
  }
}

int main(void) {
  int i, value;
  int array[] = {783, 386, 277, 415, 293, 835, 886, 992, 149, 921, 862, 527, 190, 559, 263, 426,  40, 926, 3, 55, 845, -1};
  //  int array[] = {5, 3, 6, 2, 4, 1, -1};
  
  /* encoding */
  binarytree binarytree;
  
#if 0
  i = 0;
  while (array[i] >= 0) binarytree.add(array[i++]);
#endif
  for (i = 0; i < 30; i ++) {
    do {
      value = (rand() % 999) + 1;
    } while (binarytree.search(value) != 0);

    cout << "Value = " << value << endl;

    binarytree.add(value);
  }


  return 0;
}
