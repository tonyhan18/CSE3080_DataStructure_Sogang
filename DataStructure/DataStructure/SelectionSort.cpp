#define _CRT_OBSOLETE_NO_WARNINGS
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;

//변수 바꾸는 함수
void swap(long long* a, long long* b) {
	long long temp = *a;
	*a = *b;
	*b = temp;
}

//선택정렬
void selectSort(long long* d, int n) {
	int i, j=0;
	for (i = 0; i < n - 1; ++i) {
		int min = i;
		for (j = i + 1; j < n; ++j) {
			_sleep(1); // 시간 측정용 부하
			if (d[min] > d[j]) min = j;
		}
		swap(&d[min], &d[i]);
	}

	//출력부
	cout << "Output : ";
	for (i = 0; i < n; ++i) {
		cout << d[i];
		if (i != n - 1) cout << " ";
	}
}

int main(void) {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	ifstream File("lab1.dat");
	ofstream out("result1.dat");
	clock_t start,end;

	//파일 확인
	if (!File) {
		cout << "NOT OPEN" << '\n';
		return 0;
	}

	//선택 정렬 시간 측정 알고리즘
	for (int n = 1; n <= 100; ++n) {
		cout << "Input : ";
		long long* d = new long long[n+1];
		for (int i = 0; i < n; ++i) {
			File >> d[i];
			cout << d[i];
			if (i != n - 1) cout << ' ';
		}
		cout << '\n';


		//함수 시작, 시간 측정
		start = clock();
		selectSort(d, n);
		
		// 작동 시간 초단위 계산
		double temp = ((double)(clock() - start)) / CLOCKS_PER_SEC;
		out << temp<<'\n';
		cout << '\n'<<"Time : "<< temp << "s"<<'\n'<<'\n';
		delete(d);
	}
}