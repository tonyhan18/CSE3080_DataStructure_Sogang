#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define ll long long

typedef struct node* nodePtr;
typedef struct node {
	nodePtr leftChild, rightChild, parent;
	ll data;
	int count;
}node;
nodePtr root;

void rotate(nodePtr x);
void splay(nodePtr x);
void insert(int data);
int search(int data);
void deleteNode(int data);

//x를 기준으로 회전
void rotate(nodePtr x) {
	nodePtr p = x->parent; //회전할 노드의 부모 노드
	nodePtr b = NULL; // x 노드의 같이 갈 수 없는 노드 저장 예정
	if (!p) return; //x가 루트 노드이면 종료

	if (x == p->leftChild) {
		p->leftChild = b = x->rightChild;
		x->rightChild = p;
	}
	else {
		p->rightChild = b = x->leftChild;
		x->leftChild = p;
	}

	x->parent = p->parent;
	p->parent = x;
	if (b) b->parent = p;

	if (x->parent) {
		if (p == x->parent->leftChild) {
			x->parent->leftChild = x;
		}
		else {
			x->parent->rightChild = x;
		}
	}
	else {
		root = x;
	}
}

void splay(nodePtr x) {
	while (x->parent) {
		nodePtr p = x->parent;
		nodePtr g = p->parent;

		if (g) {
			//zig-zig
			if ((x == p->leftChild) == (p == g->leftChild)) rotate(p);
		}
		//zig-zag
		else rotate(x);

		//zig
		rotate(x);
	}
}

int modifiedsearch(int data) {
	nodePtr p = root;
	if (!p) return 0;

	while (p) {
		if (data == p->data) return 1;
		if (data < p->data) {
			if (!p->leftChild) break;
			p = p->leftChild;
		}
		else {
			if (!p->rightChild) break;
			p = p->rightChild;
		}
	}

	return 0;
}

int search(int data) {
	nodePtr p = root;
	if (!p) return 0;

	while (p) {
		if (data == p->data) break;
		if (data < p->data) {
			if (!p->leftChild) break;
			p = p->leftChild;
		}
		else {
			if (!p->rightChild) break;
			p = p->rightChild;
		}
	}

	splay(p);
	return data == p->data;
}

void insert(int data) {
	nodePtr p = root, * pp;
	nodePtr x = (nodePtr)malloc(sizeof(node));
	x->data = data;
	x->count = 1;
	x->leftChild=x->rightChild=x->parent = NULL;

	if (!p) {
		root = x;
		return;
	}

	while (1) {
		if (data == p->data) {
			p->count++;
			free(x);
			return;
		}
		if (data < p->data) {
			if (!p->leftChild) {
				pp = &p->leftChild;
				break;
			}
			p = p->leftChild;
		}
		else {
			if (!p->rightChild) {
				pp = &p->rightChild;
				break;
			}
			p = p->rightChild;
		}
	}

	*pp = x;
	x->parent = p;
	splay(x);
}



void deleteNode(int data) {
	nodePtr p = root;
	nodePtr x;
	if (p->leftChild) {
		if (p->rightChild) {
			root = p->leftChild;
			root->parent = NULL;
			x = root;
			while (x->rightChild) x = x->rightChild;
			x->rightChild = p->rightChild;
			p->rightChild->parent = x;
			splay(x);
			free(p);
			return;
		}

		root = p->leftChild;
		root->parent = NULL;
		free(p);
		return;
	}
	if (p->rightChild) {
		root = p->rightChild;
		root->parent = NULL;
		free(p);
		return;
	}

	free(p);
	root = NULL;
}

int main(int argc,char * argv[]) {
	FILE* fp1 = fopen(argv[1], "r");
	//FILE* fp1 = fopen("input.txt", "r");
	FILE* fp2 = fopen("db_result.txt", "w");

	char op;
	int x;
	clock_t start_time = clock();
	while (1) {
		if ((op = fgetc(fp1)) == EOF) break;
		fscanf(fp1,"%d", &x);


		//만약 중복 데이터라면 노드의 count 수만 증가
		if (op == 'i') {
			insert(x);
		}
		//데이터를 찾고 갯수 감소후 찾은 데이터의 갯수가 0이라면 삭제
		else if (op == 'd') {
			if (search(x)) {
				(root->count)--;
				if (root->count == 0) deleteNode(x);
			}
			else {
				fprintf(fp2,"ignore\n");
			}
		}
		//데이터 찾기
		else if (op == 's') {
			if (modifiedsearch(x)) {
				fprintf(fp2,"true\n");
			}
			else {
				fprintf(fp2,"false\n");
			}
		}
	}
	printf("%lf\n", (double)(clock()- start_time) / CLOCKS_PER_SEC);

	fclose(fp1);
	fclose(fp2);
}