#include "stdafx.h"
#include "sceneManager.h"
#include "gameNode.h" //전방선언을 한 gameNode를 cpp파일에서 참조

//헤더에서 선언된 static 변수들 정의하기
gameNode* sceneManager::_currentScene = NULL;
gameNode* sceneManager::_loadingScene = NULL;
gameNode* sceneManager::_readyScene = NULL;

HRESULT sceneManager::init(void)
{
	_currentScene = NULL;
	_loadingScene = NULL;
	_readyScene = NULL;

	return S_OK;
}

void sceneManager::release(void)
{
	mapSceneIter miSceneList = _mSceneList.begin();

	for (; miSceneList != _mSceneList.end();)
	{
		if (miSceneList->second != NULL) //map의 두번째 value 즉 gameNode*가 NULL 이 아니면
		{
			if (miSceneList->second == _currentScene) //현재 사용하는 씬일 경우
			{
				miSceneList->second->release(); //씬 사용중에 할당된 메모리들을 release
				//%%포인터인 second의 자료형은 gameNode*이므로 second가 gameNode의 자손이고,
				//함수를 오버라이드한다 해도(자손에서 release를 새로이 정의한다해도)
				//일반적인 경우는 포인터 자료형을 기준으로 호출되어 gameNode의 release를 실행 (싱글톤들을 다 풀어버림)
				//그러나 함수 정의 제일 앞에 virtual을 써넣어주면 포인터 자료형이 아니라
				//포인터가 가리키는 클래스의 자료형을 기준으로 호출해준다.
				//http://blog.eairship.kr/175
			}

			SAFE_DELETE(miSceneList->second);

			miSceneList = _mSceneList.erase(miSceneList); //map::erase는 인풋 iterator 주소값 위치를 삭제하고 다음 주소를 가진 iterator를 return한다
		}
		else
		{
			++miSceneList;
		}

		_mSceneList.clear();
	}
}

void sceneManager::update(void)
{
	if (_currentScene) _currentScene->update();
}

void sceneManager::render(void)
{
	if (_currentScene) _currentScene->render();
}

gameNode * sceneManager::addScene(string sceneName, gameNode * scene)
{
	//해당 씬이 없으면 (포인터가 비어있는 값이면)
	if (!scene)
	{
		return NULL;
	}

	_mSceneList.insert(make_pair(sceneName, scene));

	return scene; //인풋으로 받은 scene을 그대로 아웃풋할 필요가 있는가?
}

gameNode * sceneManager::addLoadingScene(string loadingSceneName, gameNode * scene)
{
	if (!scene)
	{
		return NULL;
	}

	_mSceneList.insert(make_pair(loadingSceneName, scene));

	return scene;
}

HRESULT sceneManager::changeScene(string sceneName)
{
	//이터레이터에 sceneName을 first(string)값으로 갖는 노드를 찾아 주소 대입
	mapSceneIter find = _mSceneList.find(sceneName);

	//맵에 값이 존재하지 않으면 (end()는 데이터가 들어가있는 마지막칸 다음의 주소)
	if (find == _mSceneList.end())
	{
		return E_FAIL;
	}

	//찾는 씬이 현재 씬이면
	if (find->second == _currentScene)
	{
		return S_OK; //교체할 필요 없이 성공 메시지 리턴
	}

	//찾은 씬 교체
	if (SUCCEEDED(find->second->init())) //FAILED의 반대역할을 하는 SUCCEEDED
	{
		if (_currentScene)
		{
			_currentScene->release(); //초기화
		}

		_currentScene = find->second; //씬 교체

		return S_OK;

	}

	return E_FAIL;
}

DWORD CALLBACK loadingThread(LPVOID prc)
{
	//사용될 씬(체인지가 일어날 씬)을 셋팅해놓고
	sceneManager::_readyScene->init();

	// 현재 씬에 대입해준다
	sceneManager::_currentScene = sceneManager::_readyScene;

	sceneManager::_loadingScene->release();
	sceneManager::_loadingScene = NULL;
	sceneManager::_readyScene = NULL;

	return 0;
}

//씬을 교체하면서 중간에 로딩씬이 있는경우
HRESULT sceneManager::changeScene(string sceneName, string loadingSceneName)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	//맵에 값이 존재하지 않으면 (end()는 데이터가 들어가있는 마지막칸 다음의 주소)
	if (find == _mSceneList.end())
	{
		return E_FAIL;
	}

	//찾는 씬이 현재 씬이면
	if (find->second == _currentScene)
	{
		return S_OK; //교체할 필요 없이 성공 메시지 리턴
	}

	//sceneName이 존재하고 교체도 해야할 것이 확인 되었으므로 로딩씬 찾기
	mapSceneIter findLoading = _mLoadingSceneList.find(loadingSceneName);

	if (findLoading == _mLoadingSceneList.end()) 
	{
		return changeScene(loadingSceneName);
	}

	//찾은 씬 교체
	if (SUCCEEDED(find->second->init())) //FAILED의 반대역할을 하는 SUCCEEDED
	{
		if (_currentScene)
		{
			_currentScene->release(); //초기화
		}

		_loadingScene = findLoading->second;

		_readyScene = find->second;

		CloseHandle(CreateThread(NULL, 0, loadingThread, NULL, 0, &_loadingThreadID));
	}
	
	return E_FAIL;
}

sceneManager::sceneManager()
{
}

sceneManager::~sceneManager()
{
}