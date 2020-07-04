#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

typedef struct node* nodePtr;
typedef struct node {
	nodePtr leftChild,rightChild,parent;
	int key,cnt;
}node;
nodePtr root;

void rotate(nodePtr x);
void splay(nodePtr x);
void Insert(int key);
int search(int key);
void Delete(int key);

//x를 기준으로 회전
void rotate(nodePtr x) {
	nodePtr p = x->parent; //회전할 노드의 부모 노드
	nodePtr b = NULL; // x 노드의 같이 갈 수 없는 노드 저장 예정
	if (!p) return; //x가 루트 노드이면 종료
	
	/*x 노드를 루트 노드로 이동시킴*/
	if (x == p->leftChild) {
		p->leftChild = b = x->rightChild;
		x->rightChild = p;
	}
	else {
		p->rightChild = b = x->leftChild;
		x->leftChild = p;
	}

	//부모 재설정
	x->parent = p->parent;
	p->parent = x;
	if (b) b->parent = p;


	//꺽인 트리의 부모 노드의 자식 결정
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
	/*x의 부모가 존재 x == x 는 루트가 될 때까지 반복*/
	while (x->parent) {
		nodePtr p = x->parent;
		nodePtr g = p->parent;
		
		/*g가 존재할때 만약 꺽여있다면 RL 혹은 LR 회전*/
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

void Insert(int key) {
	nodePtr p = root, * pp;
	nodePtr x = (nodePtr)malloc(sizeof(node));
	x->key = key;
	x->cnt = 1;
	x->leftChild->rightChild->parent = NULL;

	if (!p) {
		root = x;
		return;
	}

	while (1) {
		if (key == p->key) {
			free(x);
			return;
		}
		//키 값이 더 작은 경우
		if (key < p->key) {
			//삽입 위치를 찾았을 때
			if (!p->leftChild) {
				pp = &p->leftChild;
				break;
			}
			p = p->leftChild;
		}
		else {
			//삽입 위치를 찾았을 때
			if (!p->rightChild) {
				pp = &p->rightChild;
				break;
			}
			p = p->rightChild;
		}
	}

	//노드를 필요한 위치에 삽입
	*pp = x;
	x->parent = p;
	splay(x);
}

int search(int key) {
	nodePtr p = root;
	//데이터가 없는 경우
	if (!p) return 0;
	while (p) {
		if (key == p->key) break;
		if (key < p->key) {
			if (!p->leftChild) break;
			p = p->leftChild;
		}
		else {
			if (!p->rightChild) break;
			p = p->rightChild;
		}
	}
	splay(p);
	return key == p->key;
}

void Delete(int key) {
	/*if (!search(key)) {
		printf("ignore\n");
		return;
	}*/
	nodePtr p = root;
	nodePtr x;
	if (p->leftChild) {
		//왼쪽 오른쪽 모두 존재하는 경우
		if (p->rightChild) {
			/*현재 삭제될 노드가 최상단 노드에 있음*/
			root = p->leftChild;
			root->parent = NULL;
			/*왼쪽에서 가장 큰 값을 찾아 감*/
			x = root;
			while (x->rightChild) x = x->rightChild;
			/*그 다음 root가 될 노드가 트리 최상단으로 올라감*/
			x->rightChild = p->rightChild;
			p->rightChild->parent = x;
			splay(x);
			free(p);
			return;
		}

		/*자식이 하나이거나 없는 경우 그냥 삭제해주면 됨*/
		root = p->leftChild;
		root->parent = NULL;
		free(p);
		return;
	}
	/*오른쪽에 자식이 하나인 경우*/
	if (p->rightChild) {
		root = p->rightChild;
		root->parent = NULL;
		free(p);
		return;
	}

	/*마지막 노드인 경우*/
	free(p);
	root = NULL;
}

int main(void) {
	FILE* fp1 = fopen("input.txt", "r");
	FILE* fp2 = fopen("output.txt", "w");

	char op;
	int x;
	while (1) {
		fscanf(fp1, "%c", &op);
		if (op == EOF) break;
		fscanf(fp1, "%d", &x);

		if (op == 'i') {
			if (search(x)) (root->cnt)++;
			else insert(x);
		}
		else if (op == 'd') {
			if (search(x)) {
				(root->cnt)--;
				if (root->cnt == 0) Delete(x);
			}
			else {
				printf("ignore\n");
			}
		}
		else if (op == 's') {
			if (search(x)) {
				printf("true\n");
			}
			else {
				printf("false\n");
			}
		}
	}
}