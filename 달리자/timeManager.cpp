#include "stdafx.h"
#include "timeManager.h"

HRESULT timeManager::init()
{
	_timer = new timer;
	_timer->init();

	return S_OK;
}

void timeManager::release()
{
	if (_timer != NULL)
	{
		SAFE_DELETE(_timer);
		_timer = NULL;
	}
}

void timeManager::update(float lock)
{
	if (_timer != NULL)
	{
		_timer->tick(lock);
	}
}

void timeManager::render(HDC hdc)
{
	char str[256];
	string frameRate;
	SetBkMode(hdc, TRANSPARENT);
	//폰트 백그라운드 정의
	//TRANSPARENT	: 투명
	//OPAQUE		: 불투명

//#선처리 조건문으로 디버그와 릴리스 구분
#ifdef _DEBUG
	{
		//FPS
		wsprintf(str, "단위시간당 프레임갯수 (FPS) : %d", _timer->getFrameRate()); //_timer->getFrameRate() 인수 str*을 빈자리로 두어 NULL 상태 -> 함수 내부의 wsprintf 실행하지 않고 바로 FPS 값 return만 함
		TextOut(hdc, 0, 0, str, strlen(str));

		//WorldTime
		sprintf_s(str, "총가동시간 : %f", _timer->getWorldTime());
		TextOut(hdc, 0, 20, str, strlen(str));

		//ElapsedTime
		sprintf_s(str, "경과시간(이전프레임기준) : %f", _timer->getElapsedTime());
		TextOut(hdc, 0, 40, str, strlen(str));
	}
#else //릴리즈
	{
		//FPS
		wsprintf(str, "단위시간당 프레임갯수 (FPS) : %d", _timer->getFrameRate());
		TextOut(hdc, 0, 0, str, strlen(str));
	}
#endif
}

timeManager::timeManager()
	: _timer(NULL) //생성자 실행 시점에 _timer 포인터 초기화
{
}

timeManager::~timeManager()
{
}
