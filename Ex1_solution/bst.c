#include<stdio.h>
#include<stdlib.h>
#include "bst.h"


Node* insertNode(Node *root, int value){
	// Create a new node
	Node *newNode = malloc(sizeof(Node));
	if(newNode==NULL) {
		printf("Cannot create new node\n");
		return NULL;
	}
	newNode->data = value;
 	newNode->left = NULL;
	newNode->right = NULL;

	Node *temp;
	Node *prev = NULL;

	// Traverse to find the place of the new node.
	temp = root;
	while(temp) {
		prev = temp;
		if (temp->data < newNode->data)
			temp = temp->right;
		else
			temp = temp->left;
	}
	if (prev==NULL) // If there is no other node
		root = newNode;
	else {
		if (prev->data<newNode->data)
			prev->right = newNode;
		else
			prev->left = newNode;
	}

	return root;
}

// Free recursively
Node* freeSubtree(Node *N){
	if (N==NULL)
		return N;
	if (N->left)
		freeSubtree(N->left);
	if (N->right)
		freeSubtree(N->right);
	free(N);
	N = NULL; // Essential to set it to NULL for proper execution of other functions such as printSubtree(), countNodes() 
	return N;  	
}

// Traverse recursively
void printSubtree(Node *N){
	if (N==NULL) {
        	return ;
	}
	printSubtree(N->left);
	printf("%d\n", N->data);
	printSubtree(N->right);
}

// Traverse recursively
int countNodes(Node *N){
	int count=0;
	if(N==NULL)
		return 0;
	count = count + countNodes(N->left);
	count = count + countNodes(N->right);
	count = count + 1;
	return count;
}	

// Traverse to a leaf node, then swap the values, and finally delete the leaf node.
Node* deleteNode(Node* root, int value){

	// Ensure root isn't null.
	if (root == NULL) {
    		return NULL;
	}
  	
	if (value < root->data) {  // Value is in the left sub-tree.
		root->left = deleteNode(root->left, value);
  	} 
	else if (value > root->data) { // Value is in the right sub-tree.
		root->right = deleteNode(root->right, value);
	} 
	else { 	// Found the correct node with value
		// Check the three cases - no child, 1 child, 2 child...
		// No Children
		if (root->left == NULL && root->right == NULL) {
			free(root);
			root = NULL;
		}
		// 1 child (on the left)
		else if (root->left == NULL) {
			Node *temp = root; // save current node
			root = root->right;
			free(temp);
		}
		// 1 child (on the right)
		else if (root->right == NULL) {
			Node *temp = root; // save current node
			root = root->left;
			free(temp);
		}
		// Two children
		else {
		// find minimal value of right sub tree
			Node *temp = root->right; 
			while(temp->left != NULL) {
				temp = temp->left;
			}
			root->data = temp->data; // duplicate the node
        		root->right = deleteNode(root->right, root->data); // delete the duplicate node
     		}
	}
	return root; // parent node can update reference
}

