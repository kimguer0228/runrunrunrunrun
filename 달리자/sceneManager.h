#pragma once
#include "singletonBase.h"
#include <string>
#include <map>

class gameNode; //gameNode도 stdafx를 통해 sceneManager를 참조하므로 상호참조를 피하기 위해 전방선언

class sceneManager : public singletonBase<sceneManager>
{
public:
	typedef map<string, gameNode*> mapSceneList; //gameNode를 전방선언만 한 상태이므로 4byte인 포인터를 사용하지 않으면 컴파일러는 메모리 할당을 어떻게 해야 할지 몰라 에러 발생
	typedef map<string, gameNode*>::iterator mapSceneIter;

private:
	//이 3 변수들은 static이므로 선언만 여기서 하고 정의(메모리 할당)은 외부에서 한다 (sceneManager.cpp의 {} 바깥)
	//%%static 변수의 정의,초기화는 되도록이면 cpp에서 해줄것 (헤더에서 할 경우 꼬일 수도 있음?)
	static gameNode* _currentScene; //현재 사용되는 씬을 가리킬 포인터
	//아래 두개는 멀티쓰레드를 위해 추가해놓았지만 현재 커리큘럼에서 잘 안씀
	static gameNode* _loadingScene;
	static gameNode* _readyScene;

	mapSceneList _mSceneList;
	mapSceneList _mLoadingSceneList;

	DWORD _loadingThreadID;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//씬 추가하는 함수
	gameNode* addScene(string sceneName, gameNode* scene);
	
	gameNode* addLoadingScene(string loadingSceneName, gameNode* scene);

	HRESULT changeScene(string sceneName);
	//씬을 교체하면서 중간에 로딩씬이 있는경우
	HRESULT changeScene(string sceneName, string loadingSceneName);

	friend DWORD CALLBACK loadingThread(LPVOID prc);

	sceneManager();
	~sceneManager();
};