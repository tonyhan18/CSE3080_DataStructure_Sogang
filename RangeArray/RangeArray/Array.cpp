#include "Array.h"

Array::Array(int size) {
	data = new int[size+1];
	len = size;
}

Array::~Array() {
}

int Array::length() const {
	return len;
}
/*연산자 오버로딩은 매개변수가 배열의 크기를 넘을 경우
  에러메시지와 함께 일부로 만들어 놓은 배열의 마지막 위치를 반환한다
  그 외의 경우에 대해서는 사용자가 요청하는 인덱스 위치를 반환*/
int& Array::operator[](int index) {
	if (index < 0 || index >= len) {
		cout << "Array bound error!\n";
		return data[len+1];
	}
	return *(data + index);
}

int Array::operator[] (int index) const {
	if (index < 0 || index >= len) {
		cout << "Array bound error!\n";
		return data[len+1];
	}
	return data[index];
}

//배열의 처음부터 끝까지 출력
void Array::print() {
	cout << "[ ";
	for (int i = 0; i < len; ++i) {
		cout << data[i] << ' ';
	}
	cout << "]" << '\n';
}