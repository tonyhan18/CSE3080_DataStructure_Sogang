#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <time.h>
using namespace std;


int main(void) {
	freopen("input100.txt", "w", stdout);
	srand((unsigned int)time(NULL));

	char d[3] = { 'i','d','s' };
	int n = 100;
	while (n--) {
		printf("%c %d\n", d[rand() % 3], rand() % 10000);
	}
}