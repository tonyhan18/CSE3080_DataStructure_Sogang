#include "StarWars.h"

int main(void) {
	int t;
	scanf_s("%d", &t);

	while (t--) {
		long long len;
		unsigned long long d[MAX_ARR] = { 0 };

		scanf_s("%lld", &len);
		countStar(d, len);
		printArr(d);
	}
}