#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;
vector<int> failure;

int pmatch(string str, string pat) {
	int i, j=0;
	int lens = str.size();
	int lenp = pat.size();

	//str문자 0번째부터 시작하여 pat과 같은 문자를 찾는 알고리즘
	for (i = 0; i < lens; ++i) {
		while (str[i] != pat[j] && j > 0) j = failure[j - 1];
		/* 문자가 같지 않고 pat의 index, j가 범위를 넘지 않는 한 가장 마지막으로 일치했던 문자로 복귀*/
		if (str[i] == pat[j]) { //문자가 같은 경우
			if (j == lenp - 1) return i - lenp + 1;
			/*만약 j가 pat의 길이만큼 커지었다면 동일한 문자열이라는 말이기 때문에 시작위치 반환*/
			else j++;
			//같은 문자가 아니라면 pat문자의 다음 문자로 이동
		}
	}
	return -1; //같은 문자열이 없다면 -1 반환
}


//pmatch함수에서 어디까지 같은 문자였는지를 확인하는 배열을 만들기 위한 fail함수
void fail(string pat) {
	int i, j=0;
	int lens = pat.size();

	for (i = 1;i < lens; ++i) { //j는 pat 배열을 순회하며 접미사와 접두사의 문자 동일성을 체크합니다.
		//i = failure[j - 1]; //그 전 배열의 일치도를 저장
		while (pat[j] != pat[i] && j > 0) j = failure[j-1]; 
		//문자가 다른 경우, 가장 마지막으로 같은 문자가 있었던 index 저장, 없을 경우 -1 위치 저장
		if (pat[j] == pat[i]) failure[i] = ++j;
		//문자가 일지한 경우, failure에 몇번째 문자가 같았는지를 저장
	}
}

int main(void){
	ifstream file("kmp.txt");

	if (!file) {
		fprintf(stderr, "There is No File");
		return 0;
	}

	string str;
	string pat;

	//getline함수를 이용해서 개행문자 기준으로 문자열 읽기
	getline(file, str);
	getline(file, pat);
	
	// vector 함수의 failure을 필요한 사이즈로 고정
	failure.resize(pat.size(),0);

	//함수 시작
	fail(pat);
	int pos = pmatch(str, pat);
	cout << pos << '\n';
}