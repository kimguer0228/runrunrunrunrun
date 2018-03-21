#pragma once
#include "gameNode.h"

class mainGame : public gameNode
{
private:
	
public:
	virtual HRESULT init(void);		//WM_CREATE 게임노드에서 상속받음 FAILED(_mg.init())를 사용하기 위해서는 HRESULT를 리턴해야함
	virtual void release(void);		//WM_DESTROY 게임노드에서 상속받음
	virtual void update(void);		//WM_TIMER 게임노드에서 상속받음
	virtual void render();			//WM_PAINT 게임노드에서 상속받음
									//상속, 오버라이딩, virtual에 대한 참조:http://blog.eairship.kr/175

	mainGame();
	~mainGame();
};