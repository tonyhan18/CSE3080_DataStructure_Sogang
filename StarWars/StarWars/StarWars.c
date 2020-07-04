#include "StarWars.h"

void countStar(unsigned long long* ans, int len) {
	for (int i = 1; i <= len; ++i) {
		for (int j = i; j > 0; j /= 10) {
			ans[j % 10] += 1;
		}
	}
}

void printArr(unsigned long long* ans) {

	for (int i = 0; i < 10; ++i) {
		printf("%d", ans[i]);
		if (i != 9) printf(" ");
	}
	printf("\n");
}