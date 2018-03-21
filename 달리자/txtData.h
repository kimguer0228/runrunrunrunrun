#pragma once
#include "singletonBase.h"
#include <vector>
#include <string>

class txtData : public singletonBase<txtData>
{
public:
	HRESULT init();
	void release();

	//세이브
	void txtSave(const char* saveFileName, vector<string> vStr); //vStr의 정보를 파일에 담아주는 함수
	char* vectorArrayCombine(vector<string> vArray);			 //vStr의 정보를 한줄의 문자열로 만들어주는 함수

	//로드
	vector<string> txtLoad(const char* loadFileName);
	vector<string> charArraySeparation(char charArray[]);

	txtData();
	~txtData();
};