#pragma once
//stdafx를 여기서 include하지 않고 cpp에서 include -> 상호참조 회피
#include "singletonBase.h"
#include <bitset>  //bitset은 set과 vector의 cross? 참조:http://mayple.tistory.com/entry/CSTL-8장-set-multiset-bitset

using namespace std;

#define KEYMAX 256

class keyManager : public singletonBase<keyManager>
{
private:
	bitset<KEYMAX> _keyDown;
	bitset<KEYMAX> _keyUp; //여기서의 keyUp은 눌렸다가 다시 올라가는 순간만을 지칭
						   //따라서 아무것도 안눌린 기본 상태는 false여야 한다.

public:
	HRESULT init(void); //HRESULT 참조:http://skmagic.tistory.com/286
	void release(void);

	bool isOnceKeyDown(int key);
	bool isOnceKeyUp(int key);
	bool isStayKeyDown(int key);
	bool isToggleKey(int key);

	bitset<KEYMAX> getKeyDown(void) { return _keyDown; };
	bitset<KEYMAX> getKeyUp(void) { return _keyUp; };

	void setKeyDown(int key, bool state) { _keyDown.set(key, state); };
	void setKeyUp(int key, bool state) { _keyUp.set(key, state); };

	keyManager();
	~keyManager();
};