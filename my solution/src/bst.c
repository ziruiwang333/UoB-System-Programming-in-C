#include<stdio.h>
#include<stdlib.h>
#include "bst.h"


// Your source code

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

Node* deleteNode(Node *root, int value) {
	if (root == NULL) {
		return NULL;
	}
	if(root->data == value){
		if(root->left == NULL&&root->right == NULL){
			root = NULL;
			free(root);
			return root;
		}
		else if(root->left != NULL&&root->right == NULL){
			Node *tempNode = root->left;
			root->data = tempNode->data;
			root->left = tempNode->left;
			root->right = tempNode->right;
			free(tempNode);
			return root;
		}
		else if(root->left == NULL&&root->right != NULL){
			Node *tempNode = root->right;
			root->data = tempNode->data;
			root->left = tempNode->left;
			root->right = tempNode->right;
			free(tempNode);
			return root;
		}
	}
	Node *currentNode = root;
	Node *parentNode;
	Node *deleteNode;
	while (currentNode != NULL) {
		if (value < currentNode->data) {
			parentNode = currentNode;
			currentNode = currentNode->left;
		} else if (value > currentNode->data) {
			parentNode = currentNode;
			currentNode = currentNode->right;
		} else {
			deleteNode = currentNode;
			if (deleteNode->left == NULL && deleteNode->right == NULL) {
				if (deleteNode->data < parentNode->data) {
					parentNode->left = NULL;
					free(deleteNode);
					return root;
				} else if (deleteNode->data > parentNode->data) {
					parentNode->right = NULL;
					free(deleteNode);
					return root;
				}
			} else if (deleteNode->left != NULL && deleteNode->right == NULL){
				if(deleteNode->data < parentNode->data){
					parentNode->left = deleteNode->left;
					deleteNode = NULL;
					free(deleteNode);
					return root;
				}
				else if(deleteNode->data > parentNode->data){
					parentNode->right = deleteNode->left;
					deleteNode = NULL;
					free(deleteNode);
					return root;
				}
			}
			else if(deleteNode->left == NULL && deleteNode->right != NULL){
				if(deleteNode->data < parentNode->data){
					parentNode->left = deleteNode->right;
					deleteNode = NULL;
					free(deleteNode);
					return root;
				}
				if(deleteNode->data > parentNode-> data){
					parentNode->right = deleteNode->right;
					deleteNode = NULL;
					free(deleteNode);
					return root;
				}
			}
			else {
				currentNode = currentNode->right;
					if(currentNode->left == NULL){
						deleteNode->data = currentNode->data;
						deleteNode->right = currentNode->right;
						free(currentNode);
						return root;
					}
					else if(currentNode->left != NULL){
						Node *parentCurrentNode;
						while(currentNode->left!= NULL){
							parentCurrentNode = currentNode;
							currentNode = currentNode->left;
						}
						deleteNode->data = currentNode->data;
						parentCurrentNode->left = NULL;
						free(currentNode);
						return root;
					}

			}

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

int nodeNumber = 0;
int countNodes(Node *N){
	Node *currentNode = N;
	if(currentNode != NULL){
		countNodes(currentNode->left);
		countNodes(currentNode->right);
		nodeNumber++;
	}
	return nodeNumber;
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
	}
	return NULL;
}


