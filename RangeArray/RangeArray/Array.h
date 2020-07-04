#ifndef __ARRRAY_H__
#define __ARRRAY_H__
#include <iostream>
using namespace std;

class Array {
protected:
	int* data; //배열을 가리키는 int형 포인터 변수 -> 배열을 일부러 한 칸 더 많이 할당
	int len; //배열의 총 길이
public:
	Array(int); //매개변수의 크기만큼 배열을 동적할당하는 함수
	~Array(); //동적할당된 공간을 해제하는 함수

	int length() const; //배열의 길이 반환 함수

	int& operator[](int); //배열에 데이터 삽입을 위한 연산자 오버로딩 함수
	int operator[] (int) const; //배열에 데이터 반환을 위한 연산자 오버로딩 함수
	//알고리즘 : 만약 매개변수의 값이 배열의 길이를 넘어가다면 에러메시지 출력하며, 해당 배열의 일부러 추가한 가장 마지막에 할당한 공간을 반환
	//만약 에러상황이 아니라면 정상적으로 사용자가 요청한 배열의 주소및 값 반환

	void print(); //배열의 처음부터 끝까지 출력
};

#endif