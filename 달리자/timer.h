#pragma once //참조는 cpp에 
//QueryPerformanceFrequency, QueryPerformanceCounter 참조:http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=735

class timer
{
private:
	bool _isGoodHardware; //메인보드가 자체 고해상도 타이머를 지원하는가? (아니면 _timeScale = 0.001f로 고정)

	float _timeScale; //cpu의 한클럭 단위 시간 (예: 3.6Ghz cpu면 1초당 3.6Ghz진동 -> 한클럭 시간 1초/3.6 * 10^9 (기가))
	float _timeElapsed; //경과시간 (한 프레임당 경과시간, 고정 fps시 1/60)

	__int64 _curTime; //이 순간 cpu 클럭 수 //Time이라는 명칭은 엄밀하지 않음
	__int64 _lastTime; //이전 저장시점 cpu 클럭 수
	__int64 _periodFrequency; //단위시간 동안 발생한 cpu 클럭수 (주파수)

	unsigned long _frameRate;
	unsigned long _FPSFrameCount;
	float _FPSTimeElapsed;
	float _worldTime;

public:
	HRESULT init();
	void tick(float lockFPS = 0.0f); 
	//while문 이용을 이용해 _lastTime에서 1초/lockFPS 초가 지날때까지 tick함수가 실행됨
	//예: 1초/lockFPS 초 가 1/10 초 일경우, 0초에 update가 실행되어 0.05초에 update 연산이 끝나고 tick 함수가 실행되면
	//while 안에서 0.1초가 될때까지 잡아둔 후 나와 update가 실행된다. => update 연산이 0.1초 보다 짧으면
	//항상 0.1초 간격으로 update가 실행된다. (update 연산이 더 걸리면 tick의 while문을 바로 빠져나와서 update 연속으로 실행)
	
	unsigned long getFrameRate(char* str = NULL) const; //함수 선언 뒤의 const 키워드 : http://felixblog.tistory.com/70 , http://simplesolace.tistory.com/entry/const-멤버-함수
														//const 함수는 자기가 속한 객체의 변수들을 변경할 수 없고 읽기만 할 수 있다.
	
	inline float getElapsedTime() const { return _timeElapsed; }; //inline, template는 한 파일에 선언 정의 다 되어야 사용가능
	inline float getWorldTime() const { return _worldTime; };

	timer();
	~timer();
};