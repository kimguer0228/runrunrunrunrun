#include "stdafx.h" //stdafx에서 뱅글뱅글 돌아가는 상호참조 문제를 회피하기 위해 cpp에 넣기
#include "keyManager.h" //(stdafx 헤더에서도 keyManager 헤더가 include됨) 상호참조: http://jrabbit.tistory.com/35, http://enginius.tistory.com/6

HRESULT keyManager::init(void)
{
	//처음상태 : 모든 키가 눌려져 있지않음
	for (int i = 0; i < KEYMAX; i++)
	{
		this->setKeyUp(i, false); //keyManager는 동적할당을 하는 singletonBase의 자식이므로 this. 가 아니라 this-> 사용
		this->setKeyDown(i, false);
	}
	return S_OK; //S_OK : 때때로 Boolean TRUE 값(0X0)으로 S_FALSE와 함께 사용되며 함수가 성공하였음을 의미한다.
}

void keyManager::release(void)
{
	//현재 keyManager는 release의 기능이 필요없다 
}

bool keyManager::isOnceKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) //& 0x8000이 없으면 이전에 키가 눌렸으나 현재 키가 눌리진 않은 0x0001 (즉 1)도 참이 되어 버리므로 0x0001은 제외하기 위해 0x8000과 논리곱을 실행. 참조:http://blog.eairship.kr/156
	{	//getKeyDown은 인풋없이 _keyDown을 내보내는 함수 ==> getKeyDown()[key] == _keyDown[key]
		if (!this->getKeyDown()[key]) //이부분이 없으면 계속 키가 눌리고 있는 상황에서도 true가 return되므로 once의 목적과 다르다
		{
			this->setKeyDown(key, true);
			return true;
		}
	}
	else
	{
		this->setKeyDown(key, false);
	}

	return false;
}

bool keyManager::isOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) //눌리지 않다가 이 함수가 실행되는 순간에 key 버튼이 눌린경우 참
	{
		this->setKeyUp(key, true); //key가 눌린 순간에 isOnceKeyDown 함수에서 setKeyDown(key, true)이 실행되면서
								   //setKeyUp(key, true)도 실행됨 <== 한번 눌리면 언젠가는 다시 올라갈 것이므로 여기서 keyup을 true로
	}
	else //key 버튼이 이제는 눌려있지 않음
	{
		if (this->getKeyUp()[key]) // 이것이 true라면 이전에 한번 눌렸다는 것
		{
			this->setKeyUp(key, false);
			return true;
		}
	}

	return false;
}

bool keyManager::isStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		return true;
	}

	return false;
}

bool keyManager::isToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001) //0x0001: 이전에 키가 눌렸으나 현재 키가 눌리진 않음
	{
		return true;
	}

	return false;
}

keyManager::keyManager()
{
}

keyManager::~keyManager()
{
}
