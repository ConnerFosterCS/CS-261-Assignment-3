#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include "avl.h"


int FindMinPath(struct AVLTree *tree, TYPE *path);
void printBreadthFirstTree(struct AVLTree *tree);


/* -----------------------
The main function
  param: argv = pointer to the name (and path) of a file that the program reads for adding elements to the AVL tree
*/
int main(int argc, char** argv) {

	FILE *file;
	int len, i;
	TYPE num; /* value to add to the tree from a file */
	struct timeval stop, s; /* variables for measuring execution time */
	int pathArray[50];  /* static array with values of nodes along the min-cost path of the AVL tree -- as can be seen, the tree cannot have the depth greater than 50 which is fairly sufficient for out purposes*/

	struct AVLTree *tree;
	
	tree = newAVLTree(); /*initialize and return an empty tree */
	
	file = fopen(argv[1], "r"); 	/* filename is passed in argv[1] */
	assert(file != 0);

	/* Read input file and add numbers to the AVL tree */
	while((fscanf(file, "%i", &num)) != EOF){
		addAVLTree(tree, num);		
	}
	/* Close the file  */
	fclose(file);
	
	printf("\nPrinting the tree breadth-first : \n");
	printBreadthFirstTree(tree);

	gettimeofday(&s, NULL);

	/* Find the minimum-cost path in the AVL tree*/
	len = FindMinPath(tree, pathArray);
	
	gettimeofday(&stop, NULL);

	/* Print out all numbers on the minimum-cost path */
	printf("\nThe minimum-cost path is: \n");
	for(i = 0; i < len; i++)
		printf("%d ", pathArray[i]);
	printf("\n");

	printf("\nYour execution time to find the mincost path is %f microseconds\n", (double) (stop.tv_usec - s.tv_usec));

    /* Free memory allocated to the tree */
	deleteAVLTree(tree); 
	
	return 0;
}

void preorder(struct AVLnode* cur, int* minCost, int totalSum, int* len, int* clen, TYPE* path, TYPE* cPath, TYPE pVal) 
{
	int i;

	cPath[*clen] = cur->val;
	(*clen)++;
	if (!(pVal < 0)) {
		totalSum += abs(pVal - cur->val);
	}
	if (totalSum >= *minCost) {
		return;
	}
	if (cur->left == 0 && cur->right == 0) {
		if (totalSum < *minCost) {
			*minCost = totalSum;
			for (i = 0; i < *clen; i++) {
				path[i] = cPath[i];
			}
			*len = *clen;
		}
	}
	else {
		if (cur->left) {
			preorder(cur->left, minCost, totalSum, len, clen, path, cPath, cur->val);
		}
		if (cur->right) {
			preorder(cur->right, minCost, totalSum, len, clen, path, cPath, cur->val);
		}
	}
}
  
/* --------------------
Finds the minimum-cost path in an AVL tree
   Input arguments: 
        tree = pointer to the tree,
        path = pointer to array that stores values of nodes along the min-cost path, 
   Output: return the min-cost path len 

   pre: assume that
       path is already allocated sufficient memory space 
       tree exists and is not NULL
*/
int FindMinPath(struct AVLTree *tree, TYPE *path)
{
	struct AVLnode* cur = tree->root;
	int len = 0;
	int minCost = (TYPE)10 ^ 6 * tree->cnt;
	int clen = 0;
	TYPE cPath[256];

	path[len] = tree->root->val;
	len++;

	if (tree->cnt > 1) {
		preorder(cur, &minCost, 0, &len, &clen, path, cPath, cur->val);
	}
	return len;
}

/* -----------------------
Printing the contents of an AVL tree in breadth-first fashion
  param: pointer to a tree
  pre: assume that tree was initialized well before calling this function
*/
void printBreadthFirstTree(struct AVLTree *tree)
{
	struct AVLnode** queue;
	struct AVLnode* cur = tree->root;
	int s = 0, e = 0;
	queue = (struct AVLnode**) malloc(100 * sizeof(struct AVLnode));

	queue[e++] = cur;
	while (e != s) {
		cur = queue[s++];
		if (cur->right != 0)
			queue[e++] = cur->right;
		if (cur->left != 0)
			queue[e++] = cur->left;
		printf("%i ", cur->val);
	}
	free(queue);
}