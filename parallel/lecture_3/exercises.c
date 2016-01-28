#include <stdio.h>
#include <stdlib.h>
#include "bintree.h"
/********************************************
* Cameron A. Craig and Stuart J Thain.
*
* Updated 21-1-16 to include additional functions specified in README.md
********************************************/
struct BINTREE {
	int key;
	int value;
	bintree *left;
	bintree *right;
};
/* bintree *mkNode( int key, int value, bintree *left, bintree *right)
 * makes a new node as a tree.
 * giving it values from the input variables
 * returns the created tree
 */
bintree *mkNode( int key, int value, bintree *left, bintree *right)
{
	bintree* tree = (bintree *)malloc(sizeof(bintree));
	tree->key = key;
	tree->value = value;
	tree->left = left;
	tree->right = right;
	return tree;
}

/* void freeNode( bintree * tree)
 * frees a node from the input tree
 */
void freeNode( bintree * tree)
{
	free(tree);
}

/* void freeBinTree( bintree *tree)
 * recursively removes the nodes from the tree
 * from the leafs to the root using freeNode
 */
void freeBinTree( bintree *tree)
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
 * Input: The number of indents to make.
 * Output: No return, prints to console.
 *
 */
static
void indent( int off)
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
 *
 */
static
void printBinTreeOff( int off, bintree *tree)
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
void printBinTree( bintree *tree)
{
	printBinTreeOff( 0, tree);
}
/* avltree *mergeBinTrees( avltree *tree1, avltree *tree2)
 * Combines two trees into one tree. Does this by inserting tree2's leafs into tree1 recursively.
 *  Inputs: The two trees to be combined.
 *  Output: The new meged avltree.
 */
bintree *mergeBinTrees( bintree *tree1, bintree *tree2)
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
 * then it adds a new node with the new key value at the bottom of the tree.
 *  Inputs: The key of the node to be inserted. The tree to insert the node into.
 *  Output: The new tree after insertion.
 */
bintree *insertKey( int key, int value, bintree *tree)
{

	if (tree == NULL) {
		return(mkNode(key, value, NULL, NULL));
	}
	else {

		if (key < tree->key) tree->left = insertKey(key,value,tree->left);
		else if(key == tree->key) tree->value = value;
		else tree->right = insertKey(key,value,tree->right);
		return(tree);
	}
}
/* avltree *deleteKey(int key, avltree *tree)
 * Removes the node with the key provided.
 * Recurse in order to find the key of the node that is being deleted.
 * it then connects the children of the deleted node to its parent node in
 * order to maintain the tree.
 *  Inputs: The key of the node to be deleted. The tree to delete the node from.
 *  Outputs: The new tree after deletion.
 */
bintree *deleteKey( int key, bintree *tree)
{

	if (tree == NULL) return tree;

	if (key < tree->key)
		tree->left = deleteKey(key, tree->left);

	else if (key > tree->key)
		tree->right = deleteKey(key, tree->right);

	else
	{
		if (tree->left == NULL)
		{
			bintree *temp = tree->right;
			free(tree);
			return temp;
		}
		else if (tree->right == NULL)
		{
			bintree *temp = tree->left;
			free(tree);
			return temp;
		}


		bintree* current  = tree->right;
		while (current->left != NULL)
			current = current->left;
		bintree* temp = current;

		tree->key = temp->key;

		tree->right = deleteKey(temp->key, tree->right);
	}
	return tree;

}
/* int findKey( int key, bintree* tree)
 * Searches recursively through the tree to find the
 * key. If the key is not found it returns 0.
 *  Inputs: The key to find and the tree to find it in.
 *  Output: The value of the node with key found.
 *
 */
int findKey( int key, bintree* tree)
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
