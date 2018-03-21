#pragma once
#include "singletonBase.h"
#include "timer.h"

class timeManager : public singletonBase<timeManager>
{
private:
	timer* _timer; 

public:
	HRESULT init();
	void release();
	void update(float lock = 0.0f); //1초당 고정 frame수(lock)을 받아 tick을 실행함
	void render(HDC hdc); //디버그 모드일 경우 프레임레이트, 기준시간, 프레임 사이 경과시간을 화면에 표시해줌 

	inline float getElapsedTime() const { return _timer->getElapsedTime(); }; //unity의 Time.deltaTime처럼 사용가능
	inline float getWorldTime() const { return _timer->getWorldTime(); };

	timeManager();
	~timeManager();
};