#include "stdafx.h"
#include "timer.h"
#include <mmsystem.h> //TimeGetTime함수 참조

//QueryPerformanceFrequency, QueryPerformanceCounter 참조:http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=735

#pragma comment(lib, "winmm.lib") //안쓰면 LNK2019/LNK1120 에러 발생 (timeGetTime가 선언되지 않은 함수라고 에러)

HRESULT timer::init()
{
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency)) 
	{//__int64인 _periodFrequency에 &(앰퍼션트)를 달아 __int64*인 포인터 주소 -> LARGE_INTEGER* 로 포인터 형변환
	 //QueryPerformanceFrequency는 메인보드 타이머가 지원되면 참을 반환하며 _periodFrequency에 cpu 주파수를 저장하고
	 //타이머가 지원되지않으면 거짓을 반환하고 _periodFrequency에 0을 저장
		_isGoodHardware = true; //타이머를 지원하여 QueryPerformanceFrequency가 참값을 반환했으므로
		
		QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime); 
		//QueryPerformanceCounter는 _lastTime에 현재 cpu 클럭수를 저장함 (함수 bool 반환값은 QueryPerformanceFrequency와 동일) 

		_timeScale = 1.0f / _periodFrequency; //진동 한번 (한 클럭)에 걸리는 시간 = 1초당 진동수의 역수
	}
	else
	{
		_isGoodHardware = false; //메인보드 타이머 지원안함

		_lastTime = timeGetTime(); //이 함수는 오차가 10ms 넘게 날 수 있다고 한다. 부정확
		_timeScale = 0.001f;
	}

	//변수 초기화
	_frameRate = 0;
	_FPSFrameCount = 0;
	_FPSTimeElapsed = 0;
	_worldTime = 0;

	return S_OK;
}

void timer::tick(float lockFPS)
{
	if (_isGoodHardware)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
		//_curTime에 이 순간의 cpu 클럭수를 저장
	}
	else
	{
		_curTime = timeGetTime();
	}

	_timeElapsed = (_curTime - _lastTime) * _timeScale;
	//_curTime - _lastTime은 이전 시점과 현 시점 사이에 클럭 갯수 차
	//_timeScale 는 진동 한번에 걸리는 시간 (한클럭) 값이므로 (클럭 갯수) * (1클럭당 시간) = 경과시간

	//핵심부분
	if (lockFPS > 0.0f)
	{
		while (_timeElapsed < (1.0f / lockFPS)) //이전에 tick 함수가 종료된 시점으로부터 경과시간이 (1.0f / lockFPS)보다 작을 경우
		{
			if (_isGoodHardware)
			{
				QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
			}
			else
			{
				_curTime = timeGetTime();
			}

			_timeElapsed = (_curTime - _lastTime) * _timeScale;
		} //경과시간이 (1.0f / lockFPS) 값이 되면 while을 빠져나와 
		  //프레임 worldTime 등등을 수정하고 tick을 종료한다
	}
	//%% tick 함수가 종료되고 update와 render가 실행되고나서 다시 tick함수가 실행될 때 이전에 tick 함수가 종료된 시점
	//%% 기준으로 이미 _timeElapsed >= (1.0f / lockFPS) 일경우 while문은 실행되지 않고 바로 tick함수를 빠져나간다.
	//%% ==> update + render 연산시간이 (1.0f / lockFPS) 길 경우 일정한 간격이 지켜지지 않는다.

	_lastTime = _curTime;
	_FPSFrameCount++; //tick함수 한번 종료될때, update와 render가 한번씩 실행됨 -> 한프레임
	_FPSTimeElapsed += _timeElapsed; //이 값은 1초마다 리셋됨 (_FPSTimeElapsed 가 1이 될동안 _FPSFrameCount값을 계속 더해 1초동안 frame 갯수를 얻는다)
	_worldTime += _timeElapsed;

	if (_FPSTimeElapsed > 1.0f) 
	{
		_frameRate = _FPSFrameCount; //_FPSTimeElapsed(1초) 동안 frame 갯수가 _FPSFrameCount이므로 _frameRate = _FPSFrameCount / (_FPSTimeElapsed ~= 1초)
		
		//frameRate 계산이 끝났으므로 count와 fps 계산용 기준시간 초기화
		_FPSFrameCount = 0;
		_FPSTimeElapsed = 0.0f;
	}
	//_FPSTimeElapsed(fps 계산용 기준시간)은 한 프레임마다 _timeElapsed 만큼씩 더해진다
	//업데이트+렌더 연산시간이 기준 경과시간인 (1.0f / lockFPS)보다 짧은 이상적인 경우에는 _timeElapsed = (1.0f / lockFPS) 이므로
	//_FPSTimeElapsed 가 1초가 될때는 _FPSFrameCount = lockFPS이 된다 (1초 = _timeElapsed * lockFPS)
	//%% 그러나 update + render 연산시간이 (1.0f / lockFPS) 길 경우 일정한 간격이 지켜지지 않으므로
	//%% _timeElapsed 가 (1.0f / lockFPS) 보다 커지게 되고 따라서 1초 =  _timeElapsed * x , x < lockFPS가 되어
	//%% _FPSFrameCount = x 즉, lockFPS 보다 프레임레이트가 작아진다.
}

unsigned long timer::getFrameRate(char * str) const
{
	if (str != NULL)
	{
		wsprintf(str, "FPS : %d", _frameRate); //인풋으로 들어온 str 포인터에 "FPS : 프레임레이트값"을 넣어줌
	}
	return _frameRate;
}

timer::timer()
{
}

timer::~timer()
{
}
