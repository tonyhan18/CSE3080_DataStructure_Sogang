#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_EXPR_SIZE 100

typedef enum { lparen, rparen, plus, minus, times, divide, mod, eos, operand }
precedence;

//연결리스트를 위한 node 구조체
typedef struct node {
	precedence data;
	struct node* link;
}node;

//연결리스트를 관리하는 포인터
node* stack;

char expr[MAX_EXPR_SIZE]; //방정식이 저장된 배열

int isp[] = { 0, 19, 12, 12, 13, 13, 13, 0 }; // 교과서 3.6절 pp. 136 참고
int icp[] = { 20, 19, 12, 12, 13, 13, 13, 0 };

//스택이 비어 있는지 확인
int IsEmpty() {
	return (stack->data == eos)? 1 : 0;
}

//최상단 노드 삭제, 출력
precedence pop() {
	if (IsEmpty() == 1) return eos;

	precedence data = stack->data;
	node* temp = stack;
	stack = stack->link;

	free(temp);
	return data;
}

//최상단 노드 출력
precedence top(void) {
	if (IsEmpty() == 0) return stack->data;
	else return eos;
}

//연산자를 스택에 저장
void push(precedence d) {
	node* newNode = (node*)malloc(sizeof(node));

	//링크드 리스트형 스택으로 공간이 있는 경우에만 노드 할당
	if (newNode != NULL) {
		newNode->data = d;
		newNode->link = NULL;

		//머리 노드인지 확인
		if (stack != NULL) newNode->link = stack;

		stack = newNode;
	}
}

/*연산자 출력*/
void printToken(precedence t) {
	switch (t) {
	case lparen:
		printf("(");
		break;
	case rparen:
		printf(")");
		break;
	case plus:
		printf("+");
		break;
	case minus:
		printf("-");
		break;
	case times:
		printf("*");
		break;
	case divide:
		printf("/");
		break;
	case mod:
		printf("%");
		break;
	}
}

/*postfix로 고치는 함수*/
void postfix() {
	char symbol; //getToken에서 switch 판별 변수
	precedence token; //읽어낸 문자를 저장
	int n = 0; //읽은 문자 갯수

	//stack의 가장 마지막 노드에 eos 저장
	push(eos);

	/*매번 방정식에 있는 문자를 읽어드려서 처리*/
	for (token = getToken(&symbol, &n); token != eos; token = getToken(&symbol, &n)) {
		//그냥 문자라면 바로 출력
		if (token == operand) printf("%c", symbol);
		//문자가 아닌 '('인 경우
		else if (token == rparen) {
			// ')' 가 stack에서 나올때까지 pop
			while (top(stack)!= lparen) {
				printToken(pop());
			}

			//마지막 ')' 출력
			pop();

		//괄호가 아닌 다른 연산자들
		}else{
			//우선순위가 저장된 배열을 기준으로 stack에 있는 문자 출력
			while (isp[top(stack)] >= icp[token]) {
				printToken(pop());
			}
			//이번에 읽어들인 문자 push
			push(token);
		}
	}

	//스택에 남아있는 모든 연산자들 출력
	while ((token = pop()) != eos) {
		printToken(token);
	}
	printf("\n");
}

//방정식에서 하나의 수식만을 가져오는 함수
precedence getToken(char* s, int* n) {
	*s = expr[(*n)++];
	switch (*s) {
	case '(': return lparen;
	case ')': return rparen;
	case '+': return plus;
	case '-': return minus;
	case '*': return times;
	case '/': return divide;
	case '%': return mod;
	case ' ': return eos;
	default: return operand; // 피연산자
	}
}

/*파일에서 방정식을 읽어오는 함수*/
void rExpr(FILE* name, char* expr) {
	freopen(name, "r", stdin);

	char temp;
	int i = 0;

	//수식을 계속 입력 받음
	while (scanf("%c", &temp) != EOF) {
		if (temp == ' ') continue;
		else expr[i++] = temp;
	}
	//수식 입력이 종료되었음을 알리는 빈칸
	expr[i] = ' ';
}

/*방정식이 정상적으로 만들어 졌는지 확인하는 함수*/
int check() {
	int ans=0;

	// '(' 이면 +1 , ')' 이면 -1
	for (int i = 0; expr[i] != ' '; ++i) {
		if (expr[i] == '(') ans++;
		else if (expr[i] == ')') ans--;
	}

	//만약 ans가 0이 아니라면 수식을 만들 수 없는 식을 의미
	return (ans == 0) ? 1 : 0;
}
int main(void) {
	rExpr("expr.txt",expr);
	if (!check()) printf("No Solution\n");
	else postfix();
}