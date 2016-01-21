typedef struct AVLTREE avltree;

extern avltree *mkNode( int key, int value, avltree *left, avltree *right);

extern void freeNode( avltree *tree);

extern void freeBinTree( avltree *tree);

extern void printBinTree( avltree *tree);

extern avltree *mergeBinTrees( avltree *tree1, avltree *tree2);

extern avltree *insertKey( int key, int value, avltree *tree);

extern avltree *deleteKey( int key, avltree *tree);

extern int findKey( int key, avltree *tree);

extern int findHeight(avltree *tree);

extern int get_diff(avltree *tree);

extern avltree* right_right_rotation(avltree *parent);

extern avltree* right_left_rotation(avltree *parent);

extern avltree* left_left_rotation(avltree *parent);

extern avltree* left_right_rotation(avltree *parent);

extern avltree* balance(avltree *node);

extern int height(avltree * tree);

extern avltree * minValueNode(avltree* tree);

extern avltree *leftRotate(avltree *tree);

extern avltree *rightRotate(avltree *tree);
