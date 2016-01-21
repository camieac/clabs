#include <stdio.h>
#include <stdlib.h>
#include "avltree.h"
/********************************************
* Cameron A. Craig and Stuart J Thain.
********************************************/
struct AVLTREE {
	int key;
	int value;
	int height;
	avltree *left;
	avltree *right;
};
/* avltree *mkNode( int key, int value, avltree *left, avltree *right)
 * Make a node from a key, value, left and right pointer.
 *      Inputs: a key, value, left and right pointer.
 *      Outputs: The new tree that has been created.
 */
avltree *mkNode( int key, int value, avltree *left, avltree *right)
{
	//Must allocate memory using malloc() to prevent segmentation error.
	avltree* tree = (avltree *)malloc(sizeof(avltree));
	//Now assign all the inputs to the tree.
	tree->key = key;
	tree->value = value;
	tree->left = left;
	tree->right = right;
	tree->height = 1;
	return tree;
}

/* int max(int a, int b){
 * Return the max of two given integers.
 *      Inputs: a and b, the two integers to find the max from.
 *      Output: The maximum of a and b.
 */
int max(int a, int b){
	if(a > b) return a;
	else return b;
}

/* void updateHeight(avltree * tree){
 * Set the tree's height to the maximum of th eleft and right node + 1.
 * Input: The tree to update.
 * Output: No return, Since input is  pointer the tree that is pointed to is changed.
 */
void updateHeight(avltree * tree){
	tree->height = max(height(tree->left), height(tree->right))+1;
}

/* void freeNode( avltree * tree)
 * Deallocate memory for a given node.
 *  Input: The node to deallocate
 *  Output: No return.
 */
void freeNode(avltree * tree)
{
	free(tree);
}
/* void freeBinTree(avltree *tree)
 *  Inputs: The root of the tree to free.
 *  Outputs:No return, tree is a pointer so the tree being pointed to is changed.
 */
void freeBinTree(avltree *tree)
{
	if(tree != NULL) {
		freeBinTree(tree->left);
		freeBinTree(tree->right);
		freeNode(tree);
	}
}

/*
 * static void indent( int off)
 * Indents by printing out some spaces.
 *  Input: The number of indents to make.
 *  Output: No return, prints to console.
 */
static void indent( int off)
{
	int i;

	for( i=0; i<off; i++) {
		printf( " ");
	}
}

/*
 * static void printBinTreeOff( int off, avltree *tree)
 * Prints the current tree using the indent function to
 * give a clear(ish) picture of the tree.
 *  Inputs: The initial offset, the tree tree to be printed.
 *  Outputs: No return.
 */
static void printBinTreeOff( int off, avltree *tree)
{
	if (tree != NULL) {
		printf( "Node{ key:%d value:%d\n", tree->key, tree->value);
		off += 6;
		indent( off);
		printf( "left: ");
		printBinTreeOff( off+6, tree->left);
		indent( off);
		printf( "right:");
		printBinTreeOff( off+6, tree->right);
		indent( off-2);
		printf( "}\n");
	} else {
		printf( "NULL\n");
	}
}

/* void printBinTree( avltree *tree)
 * Prints the tree. Sets the offset at which to print the tree.
 * The offset is set to 0 for the root node.
 *  Inputs: The indent offset to begin printing at.
 *  Output: None.
 */
void printBinTree( avltree *tree)
{
	printBinTreeOff( 0, tree);
}

/* avltree *mergeBinTrees( avltree *tree1, avltree *tree2)
 * Combines two trees into one tree. Does this by inserting tree2's leafs into tree1 recursively.
 *  Inputs: The two trees to be combined.
 *  Output: The new meged avltree.
 */
avltree *mergeBinTrees( avltree *tree1, avltree *tree2)
{
	if(tree2 != NULL) {
		mergeBinTrees(tree1, tree2->left);
		mergeBinTrees(tree1, tree2->right);
		tree1 = insertKey(tree2->key,tree2->value,tree1);
	}
	return tree1;
}
/* avltree *insertKey(int key, int value, avltree *tree)
 * Inserts a key into a node, if the tree contains the key already then
 * it simply changes the value of the key, if the key is not in the tree
 * then it adds a new node with the new key value at the bottom of the tree
 * and then fixes the height of the tree and calls the balance function
 * returns the new node.
 *  Inputs: The key of the node to be inserted. The tree to insert the node into.
 *  Output: The new tree after insertion.
 */
avltree *insertKey(int key, int value, avltree *tree)
{

	if (tree == NULL) {
		return(mkNode(key, value, NULL, NULL));
	}
	else if (key < tree->key) {
		tree->left = insertKey(key,value, tree->left);
	}
	else if (key > tree->key) {
		tree->right = insertKey(key,value, tree->right);
	}
	else if(key == tree->key) tree->value = value;

	updateHeight(tree);
	tree = balance(tree);
}

/* avltree *deleteKey(int key, avltree *tree)
 * Removes the node with the key provided.
 * Recurse in order to find the key of the node that is being deleted.
 * it then connects the children of the deleted node to its parent node in
 * order to maintain the tree. The height is then updated and the tree rebalanced
 *  Inputs: The key of the node to be deleted. The tree to delete the node from.
 *  Outputs: The new tree after deletion.
 */
avltree *deleteKey(int key, avltree *tree)
{
	if (tree == NULL)
		return tree;
	if (key < tree->key)
		tree->left = deleteKey(key,tree->left);
	else if( key > tree->key)
		tree->right = deleteKey(key,tree->right);
	else{
		if( (tree->left == NULL) || (tree->right == NULL) ) {
			avltree *temp = tree->left ? tree->left : tree->right;

			if(temp == NULL)
			{
				temp = tree;
				tree = NULL;
			}
			else
				*tree = *temp;

			free(temp);
		}
		else{
			avltree* temp = minValueNode(tree->right);

			tree->key = temp->key;

			tree->right = deleteKey(temp->key,tree->right);
		}

	}

	if (tree == NULL)
		return tree;

	updateHeight(tree);

	tree = balance(tree);
	return tree;

}
/* avltree* minValueNode(avltree* node)
 * Loops down the tree to find the left most node (smallest value node).
 *  Input: The tree to find the minimum value for.
 *  Output: The minimum value node.
 */
avltree* minValueNode(avltree* node)
{
	avltree* current = node;

	while (current->left != NULL)
		current = current->left;

	return current;
}

/* int findKey( int key, bintree* tree)
 * Searches recursively through the tree to find the
 * key. If the key is not found it returns 0.
 *  Inputs: The key to find and the tree to find it in.
 *  Output: The value of the node with key found.
 *
 */
int findKey( int key, avltree* tree)
{
	if (tree == NULL) {
		return 0;
	}
	if (tree->key == key) {
		return tree->value;
	}
	if (key < tree->key) {
		return findKey(key, tree->left);
	}else{
		return findKey(key, tree->right);
	}
}

/* int height(avltree * tree)
 * Return the height for a given tree.
 *  Input: The tree to get the height for.
 *  Output: The height of the tree (integer)
 */
int height(avltree * tree){
	if (tree == NULL)
		return 0;
	return tree->height;
}
/* int get_difference(avltree *tree)
 * Get the diffrence in height for the two branches of a node.
 *      Inputs: The node to find the diffrence for.
 *      Outputs: The difference as an integer.
 */
int get_difference(avltree *tree)
{
	if(tree == NULL) return 0;

	int left_height=height(tree->left);
	int right_height=height(tree->right);
	int b_factor= left_height - right_height;
	return b_factor;
}




/* avltree* balance(avltree *tree)
 * Balances a tree so that each branch has an equal height or a difference of 1 for odd amounts of nodes.
 *      Inputs: The tree to balance
 *      Outputs: The balanced tree
 */
avltree* balance(avltree *tree)
{
	int difference= get_difference(tree);
	// If this node becomes unbalanced, then there are 4 cases

	// Left Left Case
	if (difference > 1 && get_difference(tree->left) >= 0)
		return rightRotate(tree);

	// Left Right Case
	if (difference > 1 && get_difference(tree->left) < 0)
	{
		tree->left =  leftRotate(tree->left);
		return rightRotate(tree);
	}

	// Right Right Case
	if (difference < -1 && get_difference(tree->right) <= 0)
		return leftRotate(tree);

	// Right Left Case
	if (difference < -1 && get_difference(tree->right) > 0)
	{
		tree->right = rightRotate(tree->right);
		return leftRotate(tree);
	}
	return tree;
}

/* avltree *rightRotate(avltree *tree)
 * Rotates the AVL tree right by one.
 *  Input: The tree ro rotate.
 *  Output: The rotated tree.
 */
avltree *rightRotate(avltree *tree)
{
	avltree *x = tree->left;
	avltree *T2 = x->right;
	// Perform rotation
	x->right = tree;
	tree->left = T2;
	// Update heights
	updateHeight(tree);
	updateHeight(x);
	// Return new root
	return x;
}

/* avltree *leftRotate(avltree *tree)
 * Rotates the AVL tree left by one.
 *  Input: The tree ro rotate.
 *  Output: The rotated tree.
 */
avltree *leftRotate(avltree *tree)
{
	avltree *y = tree->right;
	avltree *T2 = y->left;

	// Perform rotation
	y->left = tree;
	tree->right = T2;

	//  Update heights
	updateHeight(y);
	updateHeight(tree);

	// Return new root
	return y;
}
