#pragma once
//상호참조 문제를 피하기 위해 stdafx.h는 imageManager.cpp 에서 include한다.
#include "singletonBase.h"
#include "image.h"
//#include <string>
#include <map> //STL map container 사용함

class imageManager : public singletonBase<imageManager>
{
private:
	typedef map<string, image*> mapImageList; //매번 template 클래스 map에 key와 value의 타입을 적어주기
											  //귀찮으므로 typedef를 이용
	typedef map<string, image*>::iterator mapImageIter; //위와 동일
private:
	mapImageList _mImageList; //mapImageList는 key 타입이 string이고 value타입이 image*인 map

public:
	HRESULT init(); //현재는 return S_OK; 외에 아무 내용도 없음
	void release(); //현재는 아무 내용 없음

	//빈 비트맵 이미지
	image* addImage(string strKey, int width, int height);
	
	//파일 이미지
	image* addImage(string strKey, const char* fileName, int width, int height, BOOL trans = FALSE, COLORREF transColor = FALSE , BOOL reset = FALSE);
	image* addImage(string strKey, const char* fileName, float x, float y, int width, int height, BOOL trans = FALSE, COLORREF transColor = FALSE);

	image* newAddImage(string strKey, const char* fileName, int width, int height, BOOL trans = FALSE, COLORREF transColor = FALSE, BOOL reset = FALSE);

	//프레임 이미지
	image* addFrameImage(string strKey, const char* fileName, float x, float y, int width, int height, int frameX, int frameY, BOOL trans, COLORREF transColor);
	image* addFrameImage(string strKey, const char* fileName, int width, int height, int frameX, int frameY, BOOL trans, COLORREF transColor);

	image* findImage(string strKey); //_mImageList(map)에서 strKey를 key값으로 가지는 image*가 있는지 검색

	BOOL deleteImage(string strKey); //_mImageList(map)에서 strKey를 key값으로 가지는 image* 삭제

	BOOL deleteAll(void); //_mImageList(map)안의 모든 값 삭제

	void render(string strKey, HDC hdc, int destX, int destY);
	void render(string strKey, HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);

	imageManager();
	~imageManager();
};