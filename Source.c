//spec slucajevi ZA PARNE POTEZE NE SME U ONAJ KOJI NEMA SINA(NULL), 
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
	int info;
	struct node* next;
}NODE;

struct node* createNode(int info) {
	struct node* newnode = malloc(sizeof(struct node));
	newnode->info = info;
	newnode->next = NULL;
	return newnode;
}

struct linkedList {
	struct node* head;
}LINKEDLIST;

struct graph {
	int numofNodes;
	struct linkedList* adjList;
}GRAPH;

struct graph* createGraph(int size) {
	struct graph* mainGraph = (struct graph*)malloc(sizeof(struct graph));
	mainGraph->numofNodes = size;
	mainGraph->adjList = (struct linkedList*)malloc(size * sizeof(struct linkedList)); //lista ulancanih listi

	for (int i = 0; i < size; i++) {
		mainGraph->adjList[i].head = NULL; //sve liste na pocetku su prazne
	}
	return mainGraph;
}

void printGraph(struct graph* graph, int* arrBool)
{
	for (int i = 0; i < graph->numofNodes; i++) {
		if (arrBool[i] == 1)continue;
		struct node* p = graph->adjList[i].head;
		printf("cvor br = %d\n", i);
		while (p != NULL) {
			printf("-> %d", p->info);
			p = p->next;
		}
		putchar('\n');
	}
}

void addBranch(struct graph* mainGraph, int from, int to) {
	struct node* newnode = createNode(to);
	if (mainGraph->adjList[from].head == NULL) {
		newnode->next = mainGraph->adjList[from].head;
		mainGraph->adjList[from].head = newnode;
	}
	else {
		struct node* curr = mainGraph->adjList[from].head;
		struct node* prev = NULL;
		while (curr != NULL && curr->info < to) {
			prev = curr;
			curr = curr->next;
		}
		if (prev == NULL) {//prvo
			newnode->next = curr;
			mainGraph->adjList[from].head = newnode;
			return mainGraph;
		}
		if (curr == NULL) {//poslednje
			prev->next = newnode;
			newnode->next = NULL;
			return mainGraph;
		}
		prev->next = newnode;
		newnode->next = curr;
	}
	return mainGraph;
}


void freeGraphNode(struct node* remove) {
	free(remove);
}

void removeNode(struct graph* maing, struct node** remove, int f) {
	struct node* p = maing->adjList[f].head;
	struct node* n = (*remove)->next;
	if (maing->adjList[f].head == NULL)return NULL;
	if (((*remove)->next) == NULL && (*remove) == maing->adjList[f].head) {
		maing->adjList[f].head = NULL;
		freeGraphNode((*remove));
		(*remove) = NULL;
		return maing->adjList[f].head;
	}
	while (p->next != (*remove)) {
		p = p->next;
	}

	p->next = n;
	if (maing->adjList[f].head == (*remove)) {
		maing->adjList[f].head = n;
	}
	freeGraphNode((*remove));
	(*remove) = n;
	return maing->adjList[f].head;
}

void addGraphNode(struct graph* maingraph, int* arr) {
	int n = maingraph->numofNodes;
	maingraph->adjList = realloc(maingraph->adjList, sizeof(struct linkedList) * (n + 1));
	maingraph->numofNodes = n + 1;
	maingraph->adjList[n].head = NULL;
	arr = realloc(arr, sizeof(int) * (n + 1));
	arr[n] = 0;
}
//remove ide je indeks u listi zaglavlja, a info je cvor koji svuda brisemo(uklanjamo ga iz grafa)
struct graph* removeGraphNode(struct graph* mainGraph, int* arrBool, int remove, int info) {
	struct node* curr = mainGraph->adjList[remove].head;
	if (curr == NULL && remove != info)return mainGraph;
	if (remove == info) {
		arrBool[info] = 1;
		return mainGraph;
	}
	while (curr->info != info) {
		if (curr->next == NULL)return mainGraph;
		curr = curr->next;
	}
	removeNode(mainGraph, &curr, remove);
	arrBool[info] = 1;
	return mainGraph;
}
//ako taj cvor nema grana 
void removeBranch(struct graph* mainGraph, int from, int to) {
	struct node* curr = mainGraph->adjList[from].head;
	struct node* prev = NULL;
	while (curr->info != to) {
		prev = curr;
		curr = curr->next;
	}
	if (curr == mainGraph->adjList[from].head) {//prvi el
		mainGraph->adjList[from].head = curr->next;
		freeGraphNode(curr);
	}
	else {//bilo gde
		prev->next = curr->next;
		freeGraphNode(curr);
	}
}

void deleteGraph(struct graph* maingraph, int* arr) {
	int n = maingraph->numofNodes;
	for (int i = 0; i < n; i++) {
		if (arr[i] != 0) {
			while (maingraph->adjList[i].head != NULL) {
				maingraph->adjList[i].head = maingraph->adjList[i].head->next;
				freeGraphNode(maingraph->adjList[i].head);
			}
		}
	}
	free(maingraph->adjList);
	free(maingraph);
}

struct Qnode {
	int* info;
	int stepFlag;
	int len;
	int numofmoves;
	struct Qnode* next;
}QNODE;

struct queue {
	struct Qnode *front;
	struct Qnode *rear;
}QUEUE;

struct queue* createQueue() {
	struct queue* Q = malloc(sizeof(struct queue));
	Q->front = NULL;
	Q->rear = NULL;
	return Q;
}

struct Qnode* createQueueNode(int size, int flag, int num) {
	struct Qnode* newnode = malloc(sizeof(struct Qnode));
	newnode->info = malloc(sizeof(int) * (size + 1));
	newnode->next = NULL;
	newnode->len = size;
	newnode->stepFlag = flag;
	newnode->numofmoves = num;
	return newnode;
}

struct queue* insertQueue(struct queue* Q, int *info, int size, int flag, int num) {
	struct Qnode *newnode = createQueueNode(size, flag, num);
	for (int i = 0; i < size; i++) {
		newnode->info[i] = info[i];
	}
	if (Q->rear == NULL) {
		Q->front = newnode;
		Q->rear = newnode;
		return Q;
	}
	Q->rear->next = newnode;
	Q->rear = newnode;
	return Q;
}

int *popFromQueue(struct queue *Q, int size) {
	if (Q->front == NULL) {
		return NULL;
	}
	int *temp = calloc((size + 1), sizeof(int));
	for (int j = 0; j < size; j++) {
		temp[j] = Q->front->info[j];
	}
	
	Q->front = Q->front->next;
	if (Q->front == NULL) {
		Q->rear = NULL;
	}
	return temp;
}

int QueueEmpty(struct queue *q) {
	if (q->rear == NULL) {
		return 1;
	}
	return 0;
}

int isNotVisited(int x, int *path, int size)
{
	for (int i = 0; i < size; i++)
		if (path[i] == x)
			return 0;
	return 1;
}

void pathsPlayerOne(struct graph *maingraph, int source, int destination, int *move1) {
	(*move1) = INT_MAX;
	struct queue *q = createQueue();
	int *path = NULL;
	int lenght = 0;

	path = realloc(path, sizeof(int) * (lenght + 1));
	path[lenght] = source;
	lenght++;
	q = insertQueue(q, path, lenght, 0, 0);

	while (!QueueEmpty(q)) {
		lenght = q->front->len;
		path = popFromQueue(q, lenght);
		int last = path[lenght - 1];
		

		if (last == destination) {
			printf("Igrac broj 1:\n");
			if (lenght - 1 < (*move1))(*move1) = lenght - 1;
			for (int i = 0; i < lenght; i++) {
				if (i != lenght - 1) {
					printf("%d -> ", path[i]);
				}
				else {
					printf("%d\n", path[i]);
				}
			}
		
		}
		struct node *p = maingraph->adjList[last].head;
		while (p != NULL) {
			if (isNotVisited(p->info, path, lenght)) {
				int *newpath = malloc((lenght + 1)*sizeof(int));
				for (int j = 0; j < lenght; j++) {
					newpath[j] = path[j];
				}
				newpath[lenght] = p->info;
				
				q = insertQueue(q, newpath, (lenght + 1), 0, 0);
			}
			p = p->next;
		}
	}
	printf("MINMOVE : %d\n", (*move1));
}
//dodaj deo sta ako nema puta do tog cvora

void pathsPlayerTwo(struct graph* maingraph, int source, int destination, int *move2) {
	(*move2) = INT_MAX;
	int flag;
	struct queue* q = createQueue();
	int* path = NULL;
	int lenght = 0;
	int onesize = 1, twosize = 0;
	path = realloc(path, sizeof(int) * (lenght + 1));
	path[lenght] = source;
	lenght++;
	q = insertQueue(q, path, lenght, 1, 0);
	int tempMove;

	int* oneVisit = malloc(maingraph->numofNodes*sizeof(int));
	int* twoVisit = malloc(maingraph->numofNodes*sizeof(int));
	for (int i = 0; i < maingraph->numofNodes; i++) {
		oneVisit[i] = INT_MAX;
		twoVisit[i] = INT_MAX;
	}
	
	while (!QueueEmpty(q)) {
		tempMove = q->front->numofmoves;
		lenght = q->front->len;
		int flag = q->front->stepFlag;
		path = popFromQueue(q, lenght);
		int last = path[lenght - 1];
		
		if (last == destination) {
			printf("Igrac broj dva: \n");
			if (tempMove < (*move2))(*move2) = tempMove;
			for (int i = 0; i < lenght; i++) {
				if (i != lenght - 1) {
					printf("%d -> ", path[i]);
				}
				else {
					printf("%d\n", path[i]);
				}
			}
			continue;
		}
		
		if (flag) { //pomeranje za 1
			tempMove++;
			struct node* p = maingraph->adjList[last].head;
			while (p != NULL) {
				if (oneVisit[p->info] >= lenght + 1) {
					int* newpath = malloc((lenght + 1) * sizeof(int));
					for (int j = 0; j < lenght; j++) {
						newpath[j] = path[j];
					}
					newpath[lenght] = p->info;
					oneVisit[p->info] = lenght + 1;
					q = insertQueue(q, newpath, (lenght + 1), 0, tempMove);
				}
				p = p->next;
			}
		}
		else {
			tempMove++;
			struct node* temp = maingraph->adjList[last].head;
			int* visitedInthis = calloc(maingraph->numofNodes, sizeof(int));
			while (temp != NULL) {
				last = temp->info;
				struct node* p = maingraph->adjList[last].head;
				//ako je p NULL nema susede i igrac 2 je izgubio, nema gde da ode.
				while (p != NULL) {
					//za posecenost
					if ((twoVisit[p->info] >= lenght + 2) || visitedInthis[p->info]) {
						twoVisit[p->info] = lenght + 2;
						visitedInthis[p->info] = 1;

						int* newpath = malloc((lenght + 2) * sizeof(int));
						for (int j = 0; j < lenght; j++) {
							newpath[j] = path[j];
						}
						newpath[lenght] = temp->info;
						newpath[lenght + 1] = p->info;
						q = insertQueue(q, newpath, (lenght + 2), 1, tempMove);
					}
					p = p->next;
				}
				temp = temp->next;
			}
			free(visitedInthis);
		}
		
	}
	printf("MINMOVE : %d\n", (*move2));

}

int main() {
	int choice, num, i;
	int graphSize;
	printf("Unesite broj cvorova grafa :  ");
	scanf("%d", &graphSize);
	int* inGraph = calloc(graphSize, sizeof(int));
	struct graph* tryit = createGraph(graphSize);
	int player1 = 0;
	int player2 = 0;

	while (1)
	{
		printf("1. Dodaj nov cvor u graf\n");
		printf("2. Izbrisi cvor iz grafa\n");
		printf("3. Dodaj granu izmedju dva cvora\n");
		printf("4. Ukloni granu izmedju dva cvora\n");
		printf("5. Ispisi graf\n");
		printf("6. Izbrisi graf\n");
		printf("7. Ne ljuti se covece\n");
		printf("8. Kraj programa\n");
		printf("Unesite opciju :  ");
		scanf("%d", &choice);


		if (choice == 1) {
			addGraphNode(tryit, inGraph);
		}
		if (choice == 2) {
			int remove;
			printf("Unesite cvor koji zelite za izbrisete iz grafa: ");
			scanf("%d", &remove);
			if (inGraph[remove] == 1 || remove > tryit->numofNodes || remove < 0) {
				printf("Nije moguce obrisati taj cvor\n");
				continue;
			}
			for (int i = 0; i < tryit->numofNodes; i++) {
				tryit = removeGraphNode(tryit, inGraph, i, remove);
			}
		}
		if (choice == 3) {
			int from, to;
			printf("Unesite cvor OD kog polazi grana: ");
			scanf("%d", &from);
			if ((inGraph[from] == 1) || (from > tryit->numofNodes) || (from < 0)) {
				printf("Nije moguce dodati granu\n");
				continue;
			}
			printf("Unesite cvor DO kog polazi grana: ");
			scanf("%d", &to);
			if (inGraph[to] == 1 || to > tryit->numofNodes || to < 0) {
				printf("Nije moguce dodati granu\n");
				continue;
			}
			addBranch(tryit, from, to);
		}
		if (choice == 4) {
			int from, to;
			printf("Unesite cvor OD kog polazi grana: ");
			scanf("%d", &from);
			if (inGraph[from] == 1 || from > tryit->numofNodes || from < 0 || tryit->adjList[from].head == NULL) {
				printf("Nije moguce obrisati granu\n");
				continue;
			}
			printf("Unesite cvor DO kog polazi grana: ");
			scanf("%d", &to);
			if (inGraph[to] == 1 || to > tryit->numofNodes || to < 0) {
				printf("Nije moguce obrisati granu\n");
				continue;
			}
			removeBranch(tryit, from, to);
		}
		if (choice == 5) {
			printGraph(tryit, inGraph);
		}
		if (choice == 6) {
			deleteGraph(tryit, inGraph);
		}
		if (choice == 8) {
			exit(0);
		}
		if (choice == 7) {
			int cilj;
			int pocetak;
			printf("Unesite cvor OD kog se krece: ");
			scanf("%d", &pocetak);
			printf("Unesite cvor DO kog se krece: ");
			scanf("%d", &cilj);
			pathsPlayerOne(tryit, pocetak, cilj, &player1);
			putchar('\n');
			pathsPlayerTwo(tryit, pocetak, cilj, &player2);

			if (player1 > player2) {
				printf("Igrac broj 2 je pobedio\n");
			}
			else if (player1 < player2) {
				printf("Igrac broj 1 je pobedio\n");
			}
			else {
				printf("Nereseno je\n");
			}
		}
	}
	
	return 0;
}