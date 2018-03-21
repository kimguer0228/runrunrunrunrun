//#pragma once 왜 여기에는 전처리기에 한번만 읽으라고 알려줄 필요가 없는가? 지속적으로 사용해서?

#include "singletonBase.h"
#include <time.h>
#include <stdlib.h> //srand와 rand를 담은 헤더
#include <sysinfoapi.h> //GetTickCount를 담은 헤더

//랜덤함수

class randomFunction : public singletonBase<randomFunction> //template T부분에 randomFunction이 들어가서
{															// singleton이 T*에서 randomFunciont*이 된다.
public:
	randomFunction(void) //randomFunction 클래스의 생성자
	{
		srand(GetTickCount());  //GetTickCount는 32비트 49.7일이 지나면 0으로 돌아옴 GetTickCount64 (vista 이후 os) 는
	}							//2135일 사용 가능

	~randomFunction(void) {};

	//0 ~ num-1 사이에 수를 랜덤하게 받아온다
	inline int getInt(int num)
	{
		return rand() % num;
	}
	//범위 내에서 랜덤한 수를 뽑아내는 함수(ex 50, 100) 50부터 100사이
	inline int getFromIntTo(int fromNum, int toNum)
	{
		return rand() % (toNum - fromNum + 1) + fromNum; //50~100 사이의 모든 수가 나옴
	}
};