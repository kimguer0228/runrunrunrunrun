#pragma once
#include "image.h"

//이제 gameNode를 상속받는 클래스들이 복수이므로 class 바깥에 static으로 선언
static image* _backBuffer = IMAGEMANAGER->addImage("backBuffer", WINSIZEX, WINSIZEY);
//hdc의 비트맵 도화지로 개별 이미지들을 바로 복사하지 않고 _backBuffer의 hMemDC의 비트맵도화지로 일단 먼저 복사함
//그 후 다 모아진 다음에 hMemDC의 비트맵 도화지에서 hdc의 비트맵 도화지로 복사

class gameNode
{
private:
	HDC _hdc;
	bool _managerInit; //gameNode의 init에서 싱글톤 매니저들의 init 함수들을 실행하는가?

	//image* _backBuffer;	
	//hdc의 비트맵 도화지로 개별 이미지들을 바로 복사하지 않고 _backBuffer의 hMemDC의 비트맵도화지로 일단 먼저 복사함
	//그 후 다 모아진 다음에 hMemDC의 비트맵 도화지에서 hdc의 비트맵 도화지로 복사
	
	//void setBackBuffer(); 이미지매니저 생긴 후에는 필요없음

public:
	virtual HRESULT init(void);    //WM_CREATE  FAILED(_mg.init())를 사용하기 위해서는 HRESULT를 리턴해야함
	virtual HRESULT init(bool managerInit); //새 init함수
	virtual void release(void);    //WM_DESTROY
	virtual void update(void);     //WM_TIMER
	virtual void render();  //WM_PAINT
	//부모에서 virtual을 적으면 자손클래스에서는 virtual을 안써도 되지만 관례상, 보기좋도록 적어준다
	//상속, 오버라이딩, virtual에 대한 참조:http://blog.eairship.kr/175

	image* getBackBuffer(void) { return _backBuffer; }; //_backBuffer 접근(get) 함수

	HDC getMemDC() { return _backBuffer->getMemDC(); };
	HDC getHDC() { return _hdc; };

	LRESULT MainProc(HWND, UINT, WPARAM, LPARAM);

	gameNode();
	virtual ~gameNode(); //소멸자를 virtual로 해두면 소멸자에서 메모리 해제할 때
};						 //gameNode* 타입 포인터에 자손클래스가 들어가있을 경우
						 //그 포인터를 통해 소멸자가 실행되어도 알아서 자손클래스의 소멸자가 실행된다