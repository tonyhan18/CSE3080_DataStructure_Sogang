#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX_VERTICES 50
#define MIN2(x,y) ((x)<(y)?(x):(y))
short int dfn[MAX_VERTICES];
short int low[MAX_VERTICES];
int num;
int n; //sizeof graph


typedef struct node* nodePointer;
typedef struct node {
	int vertex;
	struct node* link;
}node;
nodePointer graph[MAX_VERTICES];

typedef struct stack {
	int current;
	int nextNode;
	struct stack* next;
}stack;

typedef stack* stackPointer;
stackPointer head;

void init();
void bicon(int, int);

int main(void) {

	nodePointer temp;
	nodePointer* cNode;
	char ch;


	freopen("input.txt","r",stdin);

	scanf("%d", &n);
	getchar();


	for (int i = 0; i < n; ++i) {
		cNode = &graph[i];

		getchar();
		while ((ch=getchar()) != '\n' && ch!=EOF) {
			if (ch == ' ') continue;

			temp = (node*)malloc(sizeof(node));
			temp->vertex = ch-'0';
			temp->link = NULL;

			/*헤드 노드가 비어있는 경우*/
			if (graph[i] == NULL) {
				*cNode = temp;
			}
			/*헤드 노드가 비어있지 않을 때*/
			else {
				(*cNode)->link = temp;
				cNode = &(*cNode)->link;
			}
		}
	}

	init();
	bicon(0, -1);


	printf("\nDFN : ");
	for (int i = 0; i < n; ++i) {
		printf("%d ", dfn[i]);
	}

	printf("\nLOW : ");
	for (int i = 0; i < n; ++i) {
		printf("%d ", low[i]);
	}
}


/*연결리스트로 구현한 스택의 push*/
void push(int current, int nextNode) {
	stackPointer temp = (stackPointer)malloc(sizeof(stack));
	temp->current = current;
	temp->nextNode = nextNode;

	/*head 가 비어있는 경우*/
	if (head == NULL) {
		temp->next = NULL;
		head = temp;
	}
	/*스택에 적어도 하나의 값이 들어가 있는 경우*/
	else {
		temp->next = head;
		head = temp;
	}
}

/*스택에서 가장 위쪽에 있는 데이터 뽑아내기*/
void pop(int *x, int *y) {
	stackPointer chead = head;

	/*x와 y에 최상단 엣지의 정보를 저장합니다.*/
	*x = chead->current;
	*y = chead->nextNode;

	head = head->next;
	free(chead);
}

/*모든 dfn과 low를 -1 그리고 dfn 을 카운트할 num 을 0으로 초기화 합니다.*/
void init() {
	int i;
	for (i = 0; i < n; ++i) {dfn[i] = low[i] = -1;}
	num = 0;
}

void dfnlow(int u, int v) {
	nodePointer ptr;
	int w;
	dfn[u] = low[u] = num++;

	for (ptr = graph[u]; ptr; ptr = ptr->link) {
		w = ptr->vertex;
		if (dfn[w] < 0) {
			dfnlow(w, u);
			low[u] = MIN2(low[u], low[w]);
		}
		else if (w != v) {
			low[u] = MIN2(low[u], dfn[w]);
		}
	}
}

/*current 지점과 부모 노드의 노드 값을 가지고 함수를 시작합니다. */
void bicon(int current, int ancestor) {
	nodePointer ptr;
	/*w : x : y : */
	int nextNode, x, y;

	/*우선적으로 dfn과 low에 값을 먹입니다.*/
	dfn[current] = low[current] = num++;

	for (ptr = graph[current]; ptr; ptr = ptr->link) {
		nextNode = ptr->vertex;

		/*다음 방문할 노드가 이전에 방문한 노드가(ex> 2 : 1-4 로 이어져 있기에 2-1 은 성립하지 않습니다.)
		아니면서 그 다음 노드가 방문하지 않은 경우 수행*/
		if (ancestor != nextNode && dfn[nextNode] < dfn[current]) {
			push(current, nextNode);

			/*아직 방문안한 노드 인경우 다음 노드로 DFS*/
			if (dfn[nextNode] < 0) {
				bicon(nextNode, current);

				/*DFS 가 끝나는 시점 : 그다음 노드와 현재 노드의 dfn 비교시 그다음 노드의 dfn이 더 큰 경우*/
				/*알고리즘에서 min(low(w)) 에 해당하는 부분*/
				low[current] = MIN2(low[current], low[nextNode]);

				/*low[nextNode] >= dfn[current] 인 경우는 사이클이 생기거나 Articulation Point 인 경우이기 때문*/
				/*알고리즘에서 low(u) = min{low(u),min{low(w)},min{dfn(w)}} 에 해당하는 부분*/
				if (low[nextNode] >= dfn[current]) {
					printf("NEW biconnexted component :");

					/*부모를 만날때까지 계속 출력해주면 하나의 BCC를 수행하게 됩니다.*/
					do {
						pop(&x, &y);
						printf(" <%d, %d>", x, y);
					} while (!((x == current) && (y == nextNode)));
					printf("\n");
				}
			}
			/*다음 노드가 방문했고, 그 노드가 이전 노드가 아닌 경우 : 
			Back Edge이기 때문에 Back Edge를 타고 올라가 가장 작은 dfn을 찾아 low[current]에 삽입*/
			//알고리즘에서 min{dfn(w) | (u,w)는 백 간선} 에 해당하는 부분
			else if (nextNode != ancestor) low[current] = MIN2(low[current], dfn[nextNode]); 
		}
	}
}