#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include "bst.h"

/*

Place for the BST functions from Exercise 1.

*/  

Node* insertNode(Node *root, int value) {

	Node *newNode = (Node*) malloc(sizeof(struct Node));

	if (newNode == NULL) {

		return NULL;

	}

	newNode->data = value;

	newNode->left = NULL;

	newNode->right = NULL;


	if (root == NULL) {

		root = newNode;

		return root;

	}


	Node *currentNode = root;

	Node *parentNode;

	while (1 == 1) {

		if (newNode->data < currentNode->data) {

			parentNode = currentNode;

			currentNode = currentNode->left;

			if (currentNode == NULL) {

				currentNode = newNode;

				parentNode->left = newNode;

				return root;

			}

		} else if (newNode->data > currentNode->data) {

			parentNode = currentNode;

			currentNode = currentNode->right;

			if (currentNode == NULL) {

				currentNode = newNode;

				parentNode->right = newNode;

				return root;

			}

		}

	}

	return root;

}


Node* deleteNode(Node* root, int value){

	if (root == NULL) {

    		return NULL;

	}


	if (value < root->data) {

		root->left = deleteNode(root->left, value);

  	}

	else if (value > root->data) {

		root->right = deleteNode(root->right, value);

	}

	else {

		if (root->left == NULL && root->right == NULL) {

			free(root);

			root = NULL;

		}

		else if (root->left == NULL) {

			Node *temp = root;

			root = root->right;

			free(temp);

		}

		else if (root->right == NULL) {

			Node *temp = root;

			root = root->left;

			free(temp);

		}

		else {

			Node *temp = root->right;

			while(temp->left != NULL) {

				temp = temp->left;

			}

			root->data = temp->data;

        		root->right = deleteNode(root->right, root->data);

     		}

	}

	return root;

}


void printSubtree(Node *N) {


	Node *currentNode = N;

	if (currentNode != NULL) {

		printSubtree(currentNode->left);

		printf("%d\n", currentNode->data);

		printSubtree(currentNode->right);

	}

}


int countNodes(Node *N){

	int count=0;

	if(N==NULL)

		return 0;

	count = count + countNodes(N->left);

	count = count + countNodes(N->right);

	count = count + 1;

	return count;

}


Node* freeSubtree(Node *N){

	if(N == NULL){

		return N;

	}

	Node *currentNode = N;

	if(currentNode != NULL){

		freeSubtree(currentNode->left);

		freeSubtree(currentNode->right);

		free(currentNode);

		currentNode = NULL;

	}

	return currentNode;

}


int sumSubtree(Node *N)
{

	// TODO: Implement this function
	int sum = 0;

	Node *currentNode = N;

		if(currentNode == NULL){

			return 0;

		}

		else{

			sum = sum + sumSubtree(currentNode->left);

			sum = sum + currentNode->data;

			sum = sum + sumSubtree(currentNode->right);

		}


		return sum;
}


// This functions converts an unbalanced BST to a balanced BST 

int i = 0;

int *getNodeArray(int a[], Node* currentNode){

	if(currentNode != NULL){

		getNodeArray(a,currentNode->left);

		a[i++] = currentNode->data;

		getNodeArray(a,currentNode->right);

	}

	return a;

}


Node* getBalanceTree(int a[], int arrayLength, Node *root) {

	int leftArrayLength = arrayLength/2;

	int leftArray[leftArrayLength];

	int n1;
	for(n1 = 0;n1 < leftArrayLength;n1++){

		leftArray[n1] = a[n1];

	}


	int rightArrayLength = (arrayLength-1)/2;

	int rightArray[rightArrayLength];

	int n2;
	for(n2 = 0; n2 < rightArrayLength;n2++){

		rightArray[n2] = a[(arrayLength/2)+1+n2];

	}


	if(arrayLength != 0){

		root = insertNode(root, a[arrayLength/2]);

		getBalanceTree(leftArray, leftArrayLength, root);

		getBalanceTree(rightArray, rightArrayLength, root);

	}

	return root;

}

int *nodeArray;
Node* balanceTree(Node* root) 
{

	// TODO: Implement this function
	Node *newRoot = NULL;

	int treeSize = countNodes(root);

	int tempArray[treeSize];

	nodeArray = getNodeArray(tempArray, root);

	i = 0;

	newRoot = getBalanceTree(nodeArray, treeSize, newRoot);

	return newRoot;

}






