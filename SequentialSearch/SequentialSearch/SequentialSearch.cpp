#define _CRT_OBSOLETE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

int main(void) {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	ofstream out("lab1.data"); //데이터 저장 위치
	clock_t start, end; //시간 측정 함수
	int len; //동적 할당용 배열 길이

	cout << "Input: ";
	cin >> len;

	while (len) { //len이 0이 아닌 이상 프로그램 시행
		long long* d = new long long[len + 1]; // 동적할당 배열 선언
		int snum;
		for (int i = 0; i < len; ++i) {
			d[i]= rand() % len; // 숫자는 무조건 len 이하의 숫자만 입력
		}

		//찾고자 하는 값 검색
		cout << "Search number: ";
		cin >> snum;

		//함수 시작
		start = clock();
		int index = -1; // 만약 찾는 값이 없다면 -1 출력
		for (int i = 0; i < len; ++i) {
			_sleep(0.1); //작동시간의 가시적 측정을 위해 딜레이 함수 추가
			if (d[i] == snum) {
				index = i;
				break;
			}
		}

		//함수 결과 출력
		cout << "Searched: " << index<<'\n';
		end = clock();
		cout << "Time: " << ((double)(end - start)) / CLOCKS_PER_SEC<<"sec \n";
		out << ((double)(end - start)) / CLOCKS_PER_SEC<<"\n";

		//후처리
		delete(d);
		cout << "Input: ";
		cin >> len;
	}
}