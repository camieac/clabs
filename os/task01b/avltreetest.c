#include <stdlib.h>
#include <stdio.h>
#include "avltree.h"
/********************************************
 * Cameron A. Craig and Stuart J Thain.
 ********************************************/  

int main()
{
  avltree *tree;

  /*tree = mkNode( 5, 5,
                 mkNode( 3, 3,
                         mkNode( 1, 1,
                                 NULL,
                                 mkNode( 4, 4, NULL, NULL)),
                         NULL),
                 NULL);*/

  //printBinTree( tree);
  //freeBinTree( tree);

  tree = NULL;
  tree = insertKey( 9,1,tree);
  tree = insertKey( 5,1,tree);
  tree = insertKey( 10,1,tree);
  tree = insertKey( 0,1,tree);
  tree = insertKey( 6,1,tree);
  tree = insertKey( 11,1,tree);
  tree = insertKey(-1,1,tree);
  tree = insertKey(1,1,tree);
  tree = insertKey(2,1,tree);
  tree = deleteKey(10,tree);

  
  //tree = deleteKey(20,tree);
  printBinTree(tree);
  //tree = deleteKey( 5, tree);
  //printBinTree( tree);

  printf( "Value for key 10: %d\n", findKey( 10, tree));
  printf( "Value for key 20: %d\n", findKey( 20, tree));

  freeBinTree( tree);
  return 0;
}


