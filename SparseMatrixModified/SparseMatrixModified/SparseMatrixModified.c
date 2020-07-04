/*
FastTranspose와 msub를 연결리스트 형태로 구현하는 것은 어렵다.
그렇기 때문에 기존의 연결리스트로 되어 있던 구조를 제거하고
이차원 배열형태의 Matrix의 FastTranspose와 msub를 이용하는 것이
훨씬 속도면에서 빠르고 편리하다.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 101

typedef enum { head, entry }tagfield;
typedef struct matrix_node* matrix_pointer;

//일반 노드 구조체
typedef struct entry_node {
	int row;
	int col;
	int value;
}entry_Node;

//링크드리스트 구조체
typedef struct matrix_node {
	matrix_pointer down;
	matrix_pointer right;
	tagfield tag;

	//추가된 부분
	/*entry_Node를 멤버로 가짐으로 mSub와 mtranspose의 기능이 더 이루어지기 쉬워짐*/
	entry_Node* mT;


	union {
		matrix_pointer next; //head 일때 사용
		entry_Node entry; //node 일때 사용
	}u;
}matrixNode;


matrix_pointer hdnode[MAX_SIZE];

/*이차원 행렬을 일차원 배열로 고치는 함수*/
entry_Node* matrixRead(FILE* fp) {
	/*일차원 배열 node*/
	entry_Node* node = (entry_Node*)malloc(sizeof(entry_Node) * (MAX_SIZE));
	int rows, cols, i, j, val, cnt = 0;

	//기본 rows와 cols 읽어오기
	fscanf(fp, "%d %d", &rows, &cols);

	node[0].row = rows;
	node[0].col = cols;
	node[0].value = 0;

	//2차원 행열을 하나씩 돌면서 카운트시작
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			fscanf(fp, "%d", &val);

			//읽어낸 값이 0이 아니라면 일차원 배열에 추가
			if (val != 0) {
				node[0].value++;
				node[node[0].value].row = i;
				node[node[0].value].col = j;
				node[node[0].value].value = val;
			}
		}
	}

	//만들어진 일차원 배열 반환
	return node;
}

//새로운 matrixNode 동적할당 공간을 반환하는 함수
matrix_pointer new_node() {
	return (matrix_pointer)malloc(sizeof(matrixNode));
}

matrix_pointer mMake(matrix_pointer node, entry_Node* preT) {
	int num_rows, num_cols, num_terms = 0, num_heads, i;
	int row, col, value, current_row;
	matrix_pointer temp, last;

	/*만들어진 일차원 배열 반환*/
	entry_Node* t = preT;
	num_rows = t[0].row; num_cols = t[0].col; num_terms = t[0].value;

	/*더 큰값을 기준으로 헤더 노드 리스트를 생성*/
	num_heads = (num_cols > num_rows) ? num_cols : num_rows;

	node->mT = t; // 추가된 부분


	node->tag = entry;
	node->u.entry.row = num_rows;
	node->u.entry.col = num_cols;

	if (!num_heads) node->right = node;
	else {
		/*헤더 노드 리스트에 대한 헤더 노드들의 생성 및 초기화*/
		for (i = 0; i < num_heads; i++) {
			temp = new_node();
			hdnode[i] = temp; hdnode[i]->tag = head;
			hdnode[i]->right = temp; hdnode[i]->u.next = temp;
		}

		//값 저장 시작 위치 및 가장 마지막 노드를 가르키는 변수 초기화
		current_row = 0;
		last = hdnode[0];

		/*일차원 sparse matrix에서부터 데이터를 읽어드리어 연결리스트로 만듬*/
		for (i = 1; i <= num_terms; i++) {

			//일차원 배열의 값을 가지고 연결리스트를 계속 이어주기
			row = node->mT[i].row; col = node->mT[i].col; value = node->mT[i].value;

			if (row > current_row) {
				last->right = hdnode[current_row];
				current_row = row; last = hdnode[row];
			}

			temp = new_node(); temp->tag = entry;
			temp->u.entry.row = row; temp->u.entry.col = col;
			temp->u.entry.value = value; last->right = temp; last = temp;
			hdnode[col]->u.next->down = temp;
			hdnode[col]->u.next = temp;
		}

		// 마지막으로 일차원 배열에서 남은 값들을 저장
		last->right = hdnode[current_row];
		for (i = 0; i < num_cols; i++)
			hdnode[i]->u.next->down = hdnode[i];

		for (i = 0; i < num_heads - 1; i++)
			hdnode[i]->u.next = hdnode[i + 1];

		hdnode[num_heads - 1]->u.next = node;
		node->right = hdnode[0];
	}

	return node;
}

matrix_pointer mread(FILE* fp) {
	int num_rows, num_cols, num_terms = 0, num_heads, i;
	int row, col, value, current_row;
	matrix_pointer temp, last, node = NULL;

	/*만들어진 일차원 배열 반환*/
	entry_Node* t = matrixRead(fp);

	/*새로운 노드를 만든다음 연결리스트화 하기*/
	node = new_node();
	node = mMake(node, t);
	
	return node;
}

// sparse matrix를 출력하는 함수
void mwrite(matrix_pointer node) {
	int i;
	matrix_pointer temp, head = node->right;

	printf("\n numRows = %d, numCols = %d\n",
		node->u.entry.row, node->u.entry.col);

	/*한 행의 끝 열까지 출력 후 그 다음 행으로 이동 */
	for (i = 0; i < node->u.entry.row; i++) {
		for (temp = head->right; temp != head; temp = temp->right)
			printf("%5d%5d%5d \n",
				temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);

		/*한 행이 모두 출력된 경우 그 다음 행으로 이동*/
		head = head->u.next;
	}
}

/*모든 연결리스트 노드들을 삭제하는 함수*/
void merase(matrix_pointer* node) {
	int i, num_heads;
	matrix_pointer x, y, head = (*node)->right;

	/*한 행의 모든 열을 돌면서 모든 노드들을 동적해제*/
	for (i = 0; i < (*node)->u.entry.row; i++) {
		y = head->right;
		while (y != head) {
			x = y; y = y->right; free(x);
		}
		x = head; head = head->u.next; free(x);
	}

	// 마지막으로 남은 헤드 노드들을 삭제
	y = head;
	while (y != *node) {
		x = y; y = y->u.next; free(x);
	}

	//entry 노드를 삭제
	free(*node);

	//다음을 위한 새 노드 할당
	*node = new_node();
	node[0]->u.entry.row = node[0]->u.entry.col = 0;
}


/*
////////////////////////////////////////////
////////////////////////////////////////////
///         Subtraction Function         ///
////////////////////////////////////////////
////////////////////////////////////////////
*/
matrix_pointer msub(matrix_pointer nodeA, matrix_pointer nodeB) {
	//만약 차원이 다르다면 return 
	if (nodeA->mT[0].col != nodeB->mT[0].col || nodeA->mT[0].row != nodeB->mT[0].row) {
		fprintf(stderr, "Can't subtraction\n");
		return NULL;
	}


	//nodeA 와 nodeB의 인덱스를 하나씩 참조할 변수
	int idxA, idxB,idxD;

	/*행렬 뺄샘을 받을 행렬 생성. 초기화*/
	matrix_pointer d = new_node();
	d->mT = (entry_Node*)malloc(sizeof(entry_Node) * MAX_SIZE);

	d->mT[0].col = nodeA->mT[0].col;
	d->mT[0].row = nodeA->mT[0].row;


	/*
	1. nodeA와 nodeB의 행값을 보고 더 작은 행값을 가진 행렬의 정보를 새로운 행렬 d에 삽입
	2. nodeA와 nodeB의 열값을 보고 더 작은 열값을 가진 열의 정보를 d에 삽입
	3. 행과 열이 같을때
	 3-1. nodeA와 nodeB의 차를 d에 삽입
	 3-2. nodeA와 nodeB의 차가 0이라면 삽입하지 않고 그 다음으로 이동
	*/
	for (idxA=1,idxB=1,idxD=1; idxA <= nodeA->mT[0].value && idxB<= nodeB->mT[0].value; idxD++) {

		/*행의 위치가 다른 경우*/
		if (nodeA->mT[idxA].row < nodeB->mT[idxB].row) {
			d->mT[idxD].row = nodeA->mT[idxA].row;
			d->mT[idxD].col = nodeA->mT[idxA].col;
			d->mT[idxD].value = nodeA->mT[idxA].value;
			idxA++;
			continue;
		}
		else if (nodeA->mT[idxA].row > nodeB->mT[idxB].row) {
			d->mT[idxD].row = nodeB->mT[idxB].row;
			d->mT[idxD].col = nodeB->mT[idxB].col;
			d->mT[idxD].value = -(nodeB->mT[idxB].value);
			idxB++;
			continue;
		}


		/*열의 위치가 다른 경우*/
		if (nodeA->mT[idxA].col < nodeB->mT[idxB].col) {
			d->mT[idxD].row = nodeA->mT[idxA].row;
			d->mT[idxD].col = nodeA->mT[idxA].col;
			d->mT[idxD].value = nodeA->mT[idxA].value;
			idxA++;
			continue;
		}
		else if (nodeA->mT[idxA].col > nodeB->mT[idxB].col) {
			d->mT[idxD].row = nodeB->mT[idxB].row;
			d->mT[idxD].col = nodeB->mT[idxB].col;
			d->mT[idxD].value = -(nodeB->mT[idxB].value);
			idxB++;
			continue;
		}

		/*행과 열의 위치가 일치하는 경우*/
		//차이가 0이 아닌 이상 실행
		if (((nodeA->mT[idxA].value) - (nodeB->mT[idxB].value)) != 0) {
			d->mT[idxD].row = nodeA->mT[idxA].row;
			d->mT[idxD].col = nodeA->mT[idxA].col;
			d->mT[idxD].value = ((nodeA->mT[idxA].value) - (nodeB->mT[idxB].value));
			idxA++;
			idxB++;
		}
		else {
			idxD--;
			idxA++;
			idxB++;
		}
	}



	/*남은 값들 모두 저장*/
	while (idxA <= nodeA->mT[0].value) {
		d->mT[idxD].row = nodeA->mT[idxA].row;
		d->mT[idxD].col = nodeA->mT[idxA].col;
		d->mT[idxD].value = nodeA->mT[idxA].value;
		idxD++;
		idxA++;
	}

	while (idxB <= nodeB->mT[0].value) {
		d->mT[idxD].row = nodeB->mT[idxB].row;
		d->mT[idxD].col = nodeB->mT[idxB].col;
		d->mT[idxD].value = -(nodeB->mT[idxB].value);
		idxD++;
		idxB++;
	}

	/*행렬 d를 연결리스트로 구현*/
	d->mT[0].value = (idxD-1);
	d=mMake(d,d->mT);

	//제작된 sparse_matrix를 반환
	return d;
}


/*
////////////////////////////////////////////
////////////////////////////////////////////
///          Transpose Function          ///  
////////////////////////////////////////////
////////////////////////////////////////////
*/
void fastTranspose(matrix_pointer a, matrix_pointer b) {
	int rowTerms[MAX_SIZE], startingPos[MAX_SIZE];
	int i, j, numCols = a->mT[0].col, numTerms = a->mT[0].value;

	b->mT[0].row = numCols;
	b->mT[0].col = a->mT[0].row;
	b->mT[0].value = numTerms;

	if (numTerms > 0) { /*0이 아닌 경우에만 수행*/
		for (i = 0; i < numCols; ++i) rowTerms[i] = 0;
		for (i = 1; i <= numTerms; ++i) rowTerms[a->mT[i].col]++;

		startingPos[0] = 1;

		//행렬 값이 들어갈 위치를 저장
		for (i = 1; i < numCols; ++i) {
			startingPos[i] = startingPos[i - 1] + rowTerms[i - 1];
		}

		//행렬 값의 transpose를 새로운 행렬 b에 저장
		for (i = 1; i <= numTerms; ++i) {
			j = startingPos[a->mT[i].col]++;
			b->mT[j].row = a->mT[i].col; 
			b->mT[j].col = a->mT[i].row;
			b->mT[j].value = a->mT[i].value;
		}
	}	
}


matrix_pointer mtranspose(matrix_pointer node) {
	/*Traspose 된 행렬을 받을 새로운 행렬 생성*/
	matrix_pointer newMatrix = new_node();
	newMatrix->mT = (entry_Node*)malloc(sizeof(entry_Node) * MAX_SIZE);
	
	//fastTranspose 활용
	fastTranspose(node, newMatrix);

	//연결리스트 SparseMatrix 구현
	newMatrix=mMake(newMatrix,newMatrix->mT);
	
	//제작된 Matrix 반환
	return newMatrix;
}




int main(void) {
	matrix_pointer a, b, d;// = new_node();

	FILE* fp1 = fopen("A.txt", "r");
	FILE* fp2 = fopen("B.txt", "r");

	/*Sparse Matrix*/
	a = mread(fp1);
	b = mread(fp2);
	mwrite(a);
	mwrite(b);


	d = msub(a, b);
	mwrite(d);
	merase(&d);
	d = mtranspose(a);
	
	/*제공해주신 main에서는 mtranspose의 결과를 볼 수 없기에
	이 부분의 주석을 해제해 주시면 결과를 보실 수 있으십니다.*/
	//mwrite(d);
	

	/*출력 위치*/
	mwrite(a);
	merase(&d);
	merase(&a);
	merase(&b);

	mwrite(a);
	mwrite(b);
	mwrite(d);

	return 0;
}