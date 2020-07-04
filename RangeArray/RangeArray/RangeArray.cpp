#include "RangeArray.h"

/* 알고리즘 : 배열의 할당을 받는 것과 연산은 기존 Array 클래스의 기능을 이용하되
   호출시 사용하는 인자만 연산을 통해 호출*/
RangeArray::RangeArray(int low, int high) 
	: Array(high-low+1){ 
	//배열의 할당은 high-low+1 형태로 Array 클래스에서 할당했던것과 동일하게 한 칸 더 추가
	this->low = low;
	this->high = high;
}

RangeArray::~RangeArray() {}

int RangeArray::baseValue() {
	return low;
}
int RangeArray::endValue() {
	return high;
}

/*배열의 특정 인덱스 참조를 위해 호출한 인덱스의 값만을 연산을 통해 매개변수로 보냄
  이렇게 함으로써, Array 클래스의 기능은 그대로 이용하되, 코드는 더 간단해짐*/
int& RangeArray::operator[](int index) {
	return Array::operator[] (index - low);
}
int RangeArray::operator[](int index) const {
	return Array::operator[] (index - low);
}