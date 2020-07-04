#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_TERMS 100

typedef struct treeNode* treePtr;
typedef struct treeNode {
	treePtr lchild;
	int data;
	treePtr rchild;
}treeNode;

void inorder(treePtr);
void preorder(treePtr);
void insert(treePtr*, int);
treePtr modifiedSearch(treePtr, int);

/*Remove를 위한 함수들*/
treePtr RemoveLeftSubTree(treePtr bt);
treePtr RemoveRightSubTree(treePtr bt);
void ChangeLetSubTree(treePtr main, treePtr sub);
void ChangeRightSubTree(treePtr main, treePtr sub);
int CheckLevel(treePtr bt);
treePtr delete(treePtr* pRoot, int target);

/*ascending을 위한 함수들*/
void rangeSearch(treePtr, int, int);

int main(void) {
	int i, n, A[MAX_TERMS];
	treePtr tree = NULL;
	FILE* fp = fopen("input.txt", "r");
	fscanf(fp, "%d", &n);
	for (i = 0; i < n; ++i) {fscanf(fp, "%d", &A[i]);}

	for (i = 0; i < n; ++i) {insert(&tree, A[i]);}

	/*완성 된 트리 출력*/
	printf("inorder : ");
	inorder(tree);
	printf("\npreorder : ");
	preorder(tree);

	/*2 삭제 이후*/
	delete(&tree, 2);
	printf("\ninorder : ");
	inorder(tree);
	printf("\npreorder : ");
	preorder(tree);

	/*5 삭제 이후*/
	delete(&tree, 5);
	printf("\ninorder : ");
	inorder(tree);
	printf("\npreorder : ");
	preorder(tree);

	/*범위에서 출력*/
	printf("\nascending : ");
	rangeSearch(tree, 3, 10);
}

void inorder(treePtr tree) {
	if (tree) {
		inorder(tree->lchild);
		printf("%d ", tree->data);
		inorder(tree->rchild);
	}
}

void rangeSearch(treePtr tree, int s, int e) {
	/*start 값이 end 값보다 큰 경우*/
	if (s > e) {
		fprintf(stderr, "Start is over than end number\n");
		return;
	}
	/*그외의 경우는 inorder 함수를 그대로 따릅니다.*/
	if (tree) {
		rangeSearch(tree->lchild, s, e);
		if(tree->data>=s && tree->data<=e) printf("%d ", tree->data);
		rangeSearch(tree->rchild, s, e);
	}
}

void preorder(treePtr tree) {
	if (tree) {
		printf("%d ", tree->data);
		preorder(tree->lchild);
		preorder(tree->rchild);
	}
}

treePtr modifiedSearch(treePtr tree, int data) {
	treePtr temp = NULL;
	while (tree) {
		temp = tree;
		//왼쪽 오른쪽
		if (data < tree->data) tree = tree->lchild;
		else if (tree->data < data) tree = tree->rchild;
		//값이 같은 노드면 NULLL
		else if (tree->data == data) return NULL;
	}
	//노드를 삽입할 곳의 부모 노드를 반환
	return temp;
}

void insert(treePtr* tree, int data) {
	treePtr ptr; //노드 생성용
	treePtr temp = modifiedSearch(*tree, data); //삽위 위치 탐색용 함수

	//tree가 NULL 이거나 temp 위치를 찾은 경우 시작
	if (!(*tree) || temp) {
		/*노드 생성, 초기화*/
		ptr = (treePtr)malloc(sizeof(treeNode));
		ptr->data = data;
		ptr->lchild = ptr->rchild = NULL;


		//트리에 노드가 존재하는 경우
		if (*tree) {
			if (data < temp->data) temp->lchild = ptr;
			else temp->rchild = ptr;
		}
		//트리에 노드가 존재하지 않는 경우
		else *tree = ptr;
	}
}


/*/////////////////////////
//// Remove Function ////
/////////////////////////*/

/*왼쪽 노드를 삭제하는 연산*/
treePtr RemoveLeftSubTree(treePtr bt) {
	treePtr dNode=NULL;

	if (bt != NULL) {
		dNode = bt->lchild;
		bt->lchild = NULL;
	}
	return dNode;
}

/*오른쪽 노드를 삭제하는 연산*/
treePtr RemoveRightSubTree(treePtr bt) {
	treePtr dNode=NULL;

	if (bt != NULL) {
		dNode = bt->rchild;
		bt->rchild = NULL;
	}
	return dNode;
}

/*왼쪽 노드를 바꾸는 연산*/
void ChangeLetSubTree(treePtr main, treePtr sub) {
	main->lchild = sub;
}

/*오른쪽 연산을 바꾸는 연산*/
void ChangeRightSubTree(treePtr main, treePtr sub) {
	main->rchild = sub;
}

/*트리의 노드 수를 확인하는 함수*/
int CheckLevel(treePtr bt) {
	int max = 0;

	/*NULL 인경우 마지막에 도달했기 때문에 return 1*/
	if (bt == NULL) {
		return 1;
	}
	/*왼쪽과 오른쪽 노드로 계속 이동하며 노드의 수를 누적하고 반환합니다.*/
	max += CheckLevel(bt->lchild);
	max += CheckLevel(bt->rchild);
	return max;
}

/*삭제 함수*/
treePtr delete(treePtr* pRoot, int target) {
	/*pRoot 함수가 삭제되는 경우를 위한 pRoot 를 가리키는 포인터*/
	treePtr pVRoot = (treePtr)malloc(sizeof(treeNode));

	/*cNode는 root 노드이고 pNode는 cNode가 삭제 될 경우의 부모 노드*/
	treePtr pNode = pVRoot;
	treePtr cNode = *pRoot;
	/*dNode는 삭제되는 노드를 가리킵니다.*/
	treePtr dNode;

	/*자식이 하나일때를 위한 변수들*/
	treePtr dcNode;

	/*자식이 두개 일때를 위한 변수들*/
	int r;
	int lMax;
	int rMax;
	treePtr mNode;
	treePtr mpNode;
	int delData;

	/*가상 노드의 오른쪽에 root 노드를 놓고 시작합니다.*/
	ChangeRightSubTree(pVRoot, *pRoot);

	/*원하는 노드를 찾을 때까지 트리를 내려갑니다.*/
	while (cNode!=NULL && cNode->data != target) {
		pNode = cNode;
		if (target > cNode->data) cNode = cNode->rchild;
		else cNode = cNode->lchild;
	}
	
	/*찾이 못했다면 에러 메시지를 출력합니다.*/
	if (cNode == NULL) {
		fprintf(stderr, "\nThere is No removable date : %d",target);
		return NULL;
	}

	/*삭제 노드 dNode에 찾아낸 노드 cNode를 저장하고 삭제를 시작합니다.*/
	dNode = cNode;

	/*자식이 두개다 없는 leaf Node 인 경우*/
	if (dNode->lchild == NULL && dNode->rchild == NULL) {
		/*자식 노드만 바꾸어주는 연산을 수행합니다.*/
		if (pNode->lchild == dNode) RemoveLeftSubTree(pNode);
		else RemoveRightSubTree(pNode);
	}
	/*자식 중 하나만 있는 경우*/
	else if (dNode->lchild == NULL || dNode->rchild == NULL) {

		/*삭제 되는 노드의 좌/우 노드를 따로 저장해 둡니다.*/
		if (dNode->lchild != NULL) dcNode = dNode->lchild;
		else dcNode = dNode->rchild;

		/*dNode 의 부모노드에 dNode의 자식을 붙입니다.*/
		if (pNode->lchild == dNode) ChangeLetSubTree(pNode, dcNode);
		else ChangeRightSubTree(pNode, dcNode);
	}
	/*자식이 모두 있는 경우*/
	else {
		/*우선적으로 좌우 서브트리의 노드수를 확인합니다.*/
		lMax = CheckLevel(dNode->lchild);
		rMax = CheckLevel(dNode->rchild);

		mpNode = dNode;

		/*왼오 중 삭제할 위치를 결정하는 알고리즘*/
		//1. 왼쪽과 오른쪽 자식의 subtree가 같은 경우 랜덤선택
		if (lMax == rMax) {
			srand((unsigned int)time(NULL));
			r = rand() % 2;
			mNode = (r == 0) ? dNode->lchild : dNode->rchild;
		}
		/*더 노드가 많은 곳을 기준으로 삭제되는 노드에 들어갈 노드를 찾습니다.*/
		else { 
			mNode = (lMax > rMax) ? dNode->lchild : dNode->rchild; 
		}

		/*왼쪽으로 내려가는 경우 가장 큰 값을 찾아서 트리를 내려갑니다*/
		if (mNode == dNode->lchild) {
			while (mNode->rchild != NULL) {
				mpNode = mNode;
				mNode = mNode->rchild;
			}

			delData = dNode->data;
			dNode->data = mNode->data;

			/*찾은 노드의 왼쪽 자식을 mpNode의 왼/오에 붙입니다.*/
			if (mpNode->lchild == mNode) ChangeLetSubTree(mpNode, mNode->lchild);
			else ChangeRightSubTree(mpNode, mNode->lchild);
		}
		/*오른쪽으로 내려가는 경우 가장 작은 값을 찾아서 트리를 내려감*/
		else {
			while (mNode->lchild != NULL) {
				mpNode = mNode;
				mNode = mNode->lchild;
			}

			delData = dNode->data;
			dNode->data = mNode->data;

			/*찾은 노드의 오른쪽 자식을 mpNode의 왼/오에 붙입니다.*/
			if (mpNode->lchild == mNode) ChangeLetSubTree(mpNode, mNode->rchild);
			else ChangeRightSubTree(mpNode, mNode->rchild);
		}

		/*실재 삭제되는 노드를 없애주는 것이 아니기 때문에 dNode에 옮긴 노드를 넣어줍니다.*/
		dNode = mNode;
		dNode->data = delData;
	}

	/*마지막으로 만약 root 노드가 삭제된 경우에는 기존 트리의 루트를 처음 가상으로 만든 노드로 바꾸어 줍니다.*/
	if (pVRoot->rchild != *pRoot) *pRoot = pVRoot->rchild;
	free(pVRoot);
	return dNode;
}
