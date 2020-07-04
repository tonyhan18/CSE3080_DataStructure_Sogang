#ifndef __RANGE_ARRAY_H__
#define __RANGE_ARRAY_H__
#include "Array.h"

class RangeArray :public Array {
protected:
	int low; //배열의 첫번쨰 인덱스 저장
	int high; //배열의 마지막 인덱스 저장
public:
	RangeArray(int, int); 
	//배열의 공간을 high-low+1 만큼 Array클래스의 생성자를 호출하여 동적할당, 
	//공간을 한 개 더 추가하는 이유는 추후 인덱스를 넘어가는 위치의 값을 요구할 경우 반환할 데이터 공간을 위함
	~RangeArray();
	//동적할당 공간을 해제하는 함수

	int baseValue(); //low값을 반환하는 함수
	int endValue(); //high값을 반환하는 함수

	//배열의 참조 및 삽입용 함수
	int& operator[](int);
	int operator[](int) const;
	//알고리즘 : Array클래스의 연산자 오퍼레이터를 호출하되, 호출할 인덱스의 값을 low만큼의 값을 빼서 호출
	//low만큼의 값을 뺌으로써 배열관련 연산을 특별한 함수 정의 없이 할 수 있음.
};

#endif