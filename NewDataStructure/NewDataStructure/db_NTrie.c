#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
	//왼-오 자식
	int child[2];
	//데이터 input 확인
	int count;
} NODE;
typedef NODE* NODEPtr;
//토리의 위치를 가리키는 지시자
NODEPtr head;

//csize 는 현재 필요한 사이즈, size는 총 사이즈
int csize, size = 2;

void checksize() {

	//현재 필요한 배열보다 작다면 더 큰 크기의 배열 할당
	if (csize + 1 >= size) {
		head = (NODEPtr)realloc(head, sizeof(NODE) * size * 2);
		size *= 2;
	}
}

//배열 초기화 : 현재 위치만
void initate(int cPos) {
	checksize();
	head[cPos].child[0] = head[cPos].child[1] = -1;
	head[cPos].count = 0;
}

//숫자의 비트 따라 트리를 내려가며 데이터 탐색
int Search(int data) {
	int cbit, index = 0, i;
	for (i = 31; i >= 0; i--) {
		cbit = (data >> i) & 1;

		//만약 자식중 하나라도 -1이 있다면 해당 숫자는 아직 들어오지 않음
		if (head[index].child[cbit] == -1) return 0;
		if (head[index].count < 1) return 0;
		index = head[index].child[cbit];
	}

	//자식 중 하나라도 있다면 마지막 인덱스는 0보다 큰 숫자
	return 1;
}

void Insert(int data) {
	int i,index = 0,cbit;

	for (i = 31; i >= 0; i--) {

		//비트의 31번째부터 0번쨰 까지 검사하며 child 탐색
		cbit = (data >> i) & 1;


		//아직 자식이 초기화 되지 않은 경우 - 크기 조절 및 자식위치 결정
		if (head[index].child[cbit] == -1) {
			checksize();
			initate(++csize);

			head[csize].count++;
			index = head[index].child[cbit] = csize;
		}
		//이미 자식이 놓여진 경우 그 다음 자식으로 이동
		else {
			head[index].count++;
			index = head[index].child[cbit];
		}
	}
}

int Delete(int data) {
	//못 찾은 경우
	if (!Search(data)) {
		return 1;
	}

	//찾은 경우 - 자식따라 내려가며 count 수 줄이기
	int  i,cbit, index = 0;
	for (i = 31; i >= 0; i--) {
		cbit = (data >> i) & 1;
		index = head[index].child[cbit];
		head[index].count--;
	}
	return 0;
}

int main(int argc,char *argv[]) {
	char ch,a;
	int x;
	csize = 0;

	FILE* fp1 = fopen(argv[1], "r");
	FILE* fp2 = fopen("db_result.txt", "w");
	

	time_t start = clock();
	head = (NODE*)malloc(sizeof(NODE) * size);
	initate(0);

	while(fscanf(fp1,"%c %d\n", &ch, &x)!=EOF) {
		if (ch == 'i') Insert(x);
		else if (ch == 'd') { if (Delete(x)) fprintf(fp2, "ignore\n");}
		else if (ch == 's') {
			if (Search(x)) fprintf(fp2, "true\n");
			else fprintf(fp2, "false\n");
		}
	}	

	fclose(fp1);
	fclose(fp2);

	//모든 게 끝나면 동적 해제
	free(head);

	printf("%.6lf\n", (double)(clock()-start)/CLOCKS_PER_SEC);
	
	return 0;
}
