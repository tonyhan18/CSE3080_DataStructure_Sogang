#include<stdio.h>
#include<stdlib.h>

typedef struct threadedTree* threadedPointer;

typedef struct threadedTree {
	short int leftThread;
	threadedPointer leftChild;
	char data;
	threadedPointer rightChild;
	short int rightThread;
}threadedTree;

void construct_tree(threadedPointer tree);
threadedPointer insucc(threadedPointer tree);
void tinorder(threadedPointer tree);
void insertRight(threadedPointer s, threadedPointer r);
void insert(threadedPointer s, char data);

int main(){
	/* initialize a head node */
	// type your code 
	threadedPointer root = (threadedPointer)malloc(sizeof(threadedTree));

	//root 노드의 오른쪽은 무조건 자기자신을 가리키도록 초기화
	root->rightThread = 1;
	root->rightChild = root;

	/* 노드 E는 C오른쪽, F는 D 오른쪽, G는 B와 D 사이에 삽입 */
	construct_tree(root);
	insert(root->leftChild->rightChild, 'E');
	insert(root->leftChild->leftChild->rightChild, 'F');
	insert(root->leftChild->leftChild, 'G');

	/* 중위 순회 출력 */
	tinorder(root);
	getchar();
	return 0;
}

void construct_tree(threadedPointer tree)
{
	threadedPointer temp, temp_root;

	temp_root = tree;
	/* A를 root 왼쪽 노드에 추가 */
	temp = (threadedPointer)malloc(sizeof(threadedTree));
	temp->leftThread = temp->rightThread = 1;
	temp->leftChild = temp->rightChild = temp;
	temp->data = 'A';

	temp_root->leftChild = temp;
	temp_root->leftThread = 0;

	temp_root = tree->leftChild;
	/* B를 A 왼쪽 노드에 추가 */
	temp = (threadedPointer)malloc(sizeof(threadedTree));
	temp->rightThread = temp->leftThread = 1;
	temp->leftChild = tree;
	temp->rightChild = temp;
	temp->data = 'B';

	temp_root->leftChild = temp;
	temp_root->leftThread = 0;

	/* C를 A 오른쪽 노드에 추가 */
	temp = (threadedPointer)malloc(sizeof(threadedTree));
	temp->leftThread = temp->rightThread = 1;
	temp->leftChild = temp_root;
	temp->rightChild = tree;
	temp->data = 'C';

	temp_root->rightChild = temp;
	temp_root->rightThread = 0;

	temp_root = temp_root->leftChild;
	/* D를 B 오른쪽 노드에 추가 */
	temp = (threadedPointer)malloc(sizeof(threadedTree));
	temp->leftThread = temp->rightThread = 1;
	temp->leftChild = temp_root;
	temp->rightChild = tree->leftChild;
	temp->data = 'D';

	temp_root->rightChild = temp;
	temp_root->rightThread = 0;
}

threadedPointer insucc(threadedPointer tree){
	threadedPointer temp = tree->rightChild;

	//오른쪽에 자식이 있다 == 오른쪽 스레드가 꺼지어 있다.
	if (!tree->rightThread) {
		//왼쪽에 자식이 있다. == 왼쪽 스레드가 꺼지어 있다.
		while (!temp->leftThread) {
			//계속 왼쪽으로 내려가면서 그 다음 노드를 찾음.
			temp = temp->leftChild;
		}
	}
	
	//찾아낸 그 다음 노드를 반환
	return temp;
}

void tinorder(threadedPointer tree){

	//inoder 순회를 위해 가장 왼쪽 자식을 찾아 내려감
	threadedPointer temp=tree->leftChild;
	while (!temp->leftThread) temp = temp->leftChild;

	for(;;) {
		//마지막으로 root로 도달하였다면 root의 오른쪽 자식은 자기자신
		if (temp == tree) break;
		//그 다음 노드의 값을 출력
		printf("%3c", temp->data);
		//계속 그 다음 노드를 찾으며 내려감
		temp = insucc(temp);
	}
}

void insert(threadedPointer s, char d){
	/*새 노드를 동적할당 및 초기화하고 insertRight 함수 호출*/
	threadedPointer temp = (threadedPointer)malloc(sizeof(threadedTree));
	temp->data = d;
	insertRight(s, temp);
}

void insertRight(threadedPointer s, threadedPointer r){
	threadedPointer temp;
	
	/*자식 노드의 스레드와 자식 초기화*/
	//오른쪽은 부모의 오른쪽 자식을 가리키도록
	r->rightThread = s->rightThread;
	r->rightChild = s->rightChild;
	
	//왼쪽은 부모를 가리키도록
	r->leftThread = 1;
	r->leftChild = s;

	//부모 노드의 오른쪽 자식은 이제 새로 추가된 자식 노드
	s->rightChild = r;
	s->rightThread = 0;


	//부모 노드의 원래 오른쪽 자식의 가장 왼쪽 노드는 새로 추가된 노드를 가리키게 하기
	if (!r->rightThread) {
		temp = insucc(r);
		temp->leftChild = r;
	}

}
