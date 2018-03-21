#include "stdafx.h"
#include "imageManager.h"

HRESULT imageManager::init()
{
	return S_OK;
}

void imageManager::release()
{
}

//빈 비트맵 이미지
image * imageManager::addImage(string strKey, int width, int height)
{
	image* img = findImage(strKey); //먼저 strKey의 key값을 가진 image가 존재하는지 확인 (_mImageList) 에서

	if (img) //만약 img가 null이 아니면 == _mImageList에 이미 존재하면
	{
		return img;
	}

	//img 가 null이면
	img = new image;

	if (FAILED(img->init(width, height))) //img 초기화
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//map에서 insert할때 1)make_pair(first, second) <--- 알아서 형을 찾아서 추가
	//					 2)pair(자료형, 자료형)<first, second> <-- 형 지정하고 추가
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

//파일 이미지 1
image * imageManager::addImage(string strKey, const char * fileName, int width, int height, BOOL trans, COLORREF transColor, BOOL reset)
{
	image* img = findImage(strKey);

	if (img) //만약 img가 null이 아니면 == _mImageList에 이미 존재하면
	{
		if (reset)
		{
			deleteImage(strKey);
		}
		else
		{
			return img;
		}
	}


	//img 가 null이면
	img = new image;

	if (FAILED(img->init(fileName, width, height, trans, transColor))) //img 초기화
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//map에서 insert할때 1)make_pair(first, second) <--- 알아서 형을 찾아서 추가
	//					 2)pair(자료형, 자료형)<first, second> <-- 형 지정하고 추가
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

//파일 이미지 2
image * imageManager::addImage(string strKey, const char * fileName, float x, float y, int width, int height, BOOL trans, COLORREF transColor)
{
	image* img = findImage(strKey);

	if (img) //만약 img가 null이 아니면 == _mImageList에 이미 존재하면
	{
		return img;
	}

	//img 가 null이면
	img = new image;

	if (FAILED(img->init(fileName, x, y, width, height, trans, transColor))) //img 초기화
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//map에서 insert할때 1)make_pair(first, second) <--- 알아서 형을 찾아서 추가
	//					 2)pair(자료형, 자료형)<first, second> <-- 형 지정하고 추가
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image * imageManager::newAddImage(string strKey, const char * fileName, int width, int height, BOOL trans, COLORREF transColor, BOOL reset)
{
	image* img = findImage(strKey);

	if (img) //만약 img가 null이 아니면 == _mImageList에 이미 존재하면
	{
		if (reset)
		{
			deleteImage(strKey);
		}
		else
		{
			return img;
		}
	}


	//img 가 null이면
	img = new image;

	if (FAILED(img->init(fileName, width, height, trans, transColor))) //img 초기화
	{
		SAFE_DELETE(img);

		return NULL;
	}

	COLORREF color;
	int r;
	int g;
	int b;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			color = GetPixel(img->getMemDC(), i, j);
			r = GetRValue(color);
			g = GetGValue(color);
			b = GetBValue(color);

			if (r > 230 && g < 25 && b > 230)
			{
				SetPixel(img->getMemDC(), i, j, RGB(255, 0, 255));
			}
		}
	}

	//map에서 insert할때 1)make_pair(first, second) <--- 알아서 형을 찾아서 추가
	//					 2)pair(자료형, 자료형)<first, second> <-- 형 지정하고 추가
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

//프레임 이미지 1
image * imageManager::addFrameImage(string strKey, const char * fileName, float x, float y, int width, int height, int frameX, int frameY, BOOL trans, COLORREF transColor)
{
	image* img = findImage(strKey);

	if (img) //만약 img가 null이 아니면 == _mImageList에 이미 존재하면
	{
		return img;
	}

	//img 가 null이면
	img = new image;

	if (FAILED(img->init(fileName, x, y, width, height, frameX, frameY, trans, transColor))) //img 초기화
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//map에서 insert할때 1)make_pair(first, second) <--- 알아서 형을 찾아서 추가
	//					 2)pair(자료형, 자료형)<first, second> <-- 형 지정하고 추가
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

//프레임 이미지 2
image * imageManager::addFrameImage(string strKey, const char * fileName, int width, int height, int frameX, int frameY, BOOL trans, COLORREF transColor)
{
	image* img = findImage(strKey);

	if (img) //만약 img가 null이 아니면 == _mImageList에 이미 존재하면
	{
		return img;
	}

	//img 가 null이면
	img = new image;

	if (FAILED(img->init(fileName, width, height, frameX, frameY, trans, transColor))) //img 초기화
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//map에서 insert할때 1)make_pair(first, second) <--- 알아서 형을 찾아서 추가
	//					 2)pair(자료형, 자료형)<first, second> <-- 형 지정하고 추가
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image * imageManager::findImage(string strKey)
{
	mapImageIter iter = _mImageList.find(strKey); //find 메소드는 key값으로 map을 순회하는 함수

	//key가 맵의 끝이 아니라면 == 키 값을 찾았으면 (end()는 항상 마지막칸 다음의 빈칸을 가리킴)
	if (iter != _mImageList.end())
	{
		return iter->second; //map의 iterator에는 ->first와 ->second로 key와 value값에 접근 가능하다
	}

	return NULL;
}

BOOL imageManager::deleteImage(string strKey)
{
	mapImageIter iter = _mImageList.find(strKey);

	if (iter != _mImageList.end())
	{
		iter->second->release(); //image클래스의 release함수 실행
		SAFE_DELETE(iter->second);
		_mImageList.erase(iter);

		return true;
	}

	return false;
}

BOOL imageManager::deleteAll(void)
{
	mapImageIter iter = _mImageList.begin();

	for (/*위에 iter값 초기화함*/; iter != _mImageList.end(); /*for loop안에서 iter값 더할지 안할지 결정*/)
	{
		if (iter->second != NULL)
		{
			iter->second->release(); //단일 삭제처럼 이부분을 넣어줘야 하는가?
			SAFE_DELETE(iter->second);
			iter = _mImageList.erase(iter); 
			//iter 부분을 삭제하므로 iter++를 하지않는다.
			//대신에 erase(iter)의 return값(지워진 값의 바로 다음)을 iter에 할당
			//map::erase는 인풋 iterator 주소값 위치를 삭제하고 다음 주소를 가진 iterator를 return한다

			//Q: container 내부의 데이터가 사라지므로 iterator가 꼬이는 것 아닌가?
			//A: A nice property of a C++ Standard Template Library (STL) map, multimap, set or multiset is that you can insert or erase elements without invalidating existing iterators.
			//참조: http://llg.cubic.org/docs/stlerase.html
		}
		else
		{
			++iter; 
		}
	}

	_mImageList.clear();

	return true;
}

void imageManager::render(string strKey, HDC hdc, int destX, int destY)
{
	image* img = findImage(strKey);

	if (img)
	{
		img->render(hdc, destX, destY);
	}
}

void imageManager::render(string strKey, HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	image* img = findImage(strKey);

	if (img)
	{
		img->render(hdc, destX, destY, sourX, sourY, sourWidth, sourHeight);
	}
}

imageManager::imageManager()
{
}

imageManager::~imageManager()
{
}
