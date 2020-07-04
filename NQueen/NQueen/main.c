#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int n; //사용자가 원하는 체스판 크기 저장
int *b; //체스판을 저장하는 배열

//여왕말을 위치시킬 수 있는지 확인하는 함수
int Can_Pos(int line){
    //첫번째 말부터 현재 이전까지 말까지의 이동가능 영역에 걸리는지 확인
    for (int i = 0; i < line; i++){
        if (b[i] == b[line] || abs(b[line] - b[i]) == line - i) return 0;
    }

    //걸리는 게 없다면 true 반환
    return 1;
}

//BackTracking 이 적용된 DFS 알고리즘
int DFS(int line){
    //만약 모든 line에 대해 말을 두었다면 놓은 위치를 출력
    if (line == (n)){
        for (int i = 0; i < n; ++i) {
            printf("%d ", b[i]);
        }
        //완성되었음을 알리며 true 반환
        return 1;
    }

    //모든 라인이 두어지지 않았다면 이번 라인의 행을 하나식 탐색
    for (int i = 0; i < n; i++) {

        /*스택의 메커니즘을 따르는 배열*/
        b[line] = i; //말을 둔 열의 행을 배열에 저장

        //말을 둘 수 있는 위치인 경우
        if (Can_Pos(line) == 1) {
            //그다음 말로 이동, 만약 모든 말을 두었다면 true를 반환
            if (DFS(line + 1)) return 1;
        }
    }
    
    //아예 방법이 나오지 않았다면 false반환
    return 0;
}

int main(void){

    printf("Input N: ");
    scanf("%d", &n);
    if(n>0) b = (int*)malloc(sizeof(int) * n); //사용자가 요청한 만큼의 체스판 생성

    //체스판이 완성안된경우 No solution 출력
    if(DFS(0) == 0 || n<=0) printf("No solution\n");

    return 0;
}