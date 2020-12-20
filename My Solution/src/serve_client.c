
pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

void *downtime(){

	Node *tempNode = NULL;

	sleep(1);
	//TODO: 1st downtime: Do balanceTree here
	pthread_rwlock_wrlock(&lock);
	tempNode = root;
	root = balanceTree(root);
	tempNode = freeSubtree(tempNode);
	pthread_rwlock_unlock(&lock);

	sleep(1);
	//TODO: 2nd downtime: Do balanceTree here
	pthread_rwlock_wrlock(&lock);
	tempNode = root;
	root = balanceTree(root);
	tempNode = freeSubtree(tempNode);
	pthread_rwlock_unlock(&lock);

	sleep(1);
	//TODO: 3rd downtime: Do balanceTree here
	pthread_rwlock_wrlock(&lock);
	tempNode = root;
	root = balanceTree(root);
	tempNode = freeSubtree(tempNode);
	pthread_rwlock_unlock(&lock);

	return NULL;
}


void* ServeClient(char *client){

	// TODO: Open the file and read commands line by line

	// TODO: match and execute commands

	// TODO: Handle command: "insertNode <some unsigned int value>"
	// print: "[ClientName]insertNode <SomeNumber>\n"
	// e.g. "[client1_commands]insertNode 1\n"

	// TODO: Handle command: "deleteNode <some unsigned int value>"
	// print: "[ClientName]deleteNode <SomeNumber>\n"
	// e.g. "[client1_commands]deleteNode 1\n"

	// TODO: Handle command: "countNodes"
	// print: "[ClientName]countNodes = <SomeNumber>\n"
	// e.g. "[client1_commands]countNodes 1\n"


	// TODO: Handle command: "sumSubtree"
	// print: "[ClientName]sumSubtree = <SomeNumber>\n"
	// e.g. "[client1_commands]sumSubtree 1\n"
	char ch[20];
	FILE *fp;
	fp = fopen(client,"r");
	if(fp==NULL){
		printf("Cannot open input file\n");
	}

	fscanf(fp,"%s",ch);
	while(getc(fp)!=EOF){
		pthread_rwlock_wrlock(&lock);
		if(strcmp("insertNode",ch)==0){
			fscanf(fp,"%s",ch);
			int value = atoi(ch);
			root = insertNode(root,value);
			printf("[%s]insertNode %d\n", client, value);
		}
		else if(strcmp("deleteNode",ch)==0){
			fscanf(fp,"%s",ch);
			int value = atoi(ch);
			root = deleteNode(root,value);
			printf("[%s]deleteNode %d\n", client, value);
		}
		else if(strcmp("countNodes",ch)==0){
			int value = countNodes(root);
			printf("[%s]countNodes %d\n", client, value);
		}
		else if(strcmp("sumSubtree",ch)==0){
			int value = sumSubtree(root);
			printf("[%s]sumSubtree %d\n", client, value);
		}
		fscanf(fp,"%s",ch);

		pthread_rwlock_unlock(&lock);
	}

	fclose(fp);


	return NULL;
}
