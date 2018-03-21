#include "stdafx.h" //상호참조 문제를 해결하기 위해
#include "image.h"

#pragma comment(lib, "msimg32.lib") //라이브러리 추가 BitBlt 대신 TransparentBlt 함수 사용 가능

image::image()
	: _imageInfo(NULL),          //{} 안에서 직접 초기화하는 것과 동일한 효과 
	_fileName(NULL),			 //image.h의 private 변수들 초기화
	_trans(false),				 //예: p변수(초기값) 은 p변수 = 초기값; 과 동일
	_transColor(RGB(0, 0, 0))
{
}

image::~image()
{
}

//빈 비트맵 초기화 (BackDC를 담은 백버퍼 초기화에 사용)
HRESULT image::init(int width, int height)
{
	//혹시나 이미지 정보가 들어가 있으면 해제
	if (_imageInfo != NULL)
	{
		release();
	}

	HDC hdc = GetDC(_hWnd); 

	_imageInfo = new IMAGE_INFO; //_imageInfo는 tagImageInfo(== IMAGE_INFO)를 가리키는 포인터(LPIMAGE_INFO)
	//=> 동적할당해야함
	_imageInfo->loadType = LOAD_EMPTY; //빈 비트맵 초기화
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	//SelectObject로 hMemDC에 hBit를 집어넣고 리턴값으로 나오는 기존값을 hObit에 저장
	_imageInfo->width = width;
	_imageInfo->height = height;

	_fileName = NULL;

	_trans = FALSE;
	_transColor = RGB(0, 0, 0);

	//알파블렌드 셋팅
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;
	//

	if (_imageInfo->hBit == NULL) //초기화가 안되어있음
	{
		release();

		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc); //HDC hdc = GetDC(_hWnd); 로 가져온 것을 반드시 해제해주어야 한다.

	return S_OK;
}

//파일 비트맵 초기화
HRESULT image::init(const char * fileName, int width, int height, BOOL trans, COLORREF transColor)
{
	//파일이름이 없으면 실패 return
	if (fileName == NULL)
	{
		return E_FAIL;
	}

	//혹시나 이미지 정보가 들어가 있으면 해제
	if (_imageInfo != NULL)
	{
		release();
	}

	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO; //_imageInfo는 tagImageInfo(== IMAGE_INFO)를 가리키는 포인터(LPIMAGE_INFO)
								 //=> 동적할당해야함
	_imageInfo->loadType = LOAD_FILE; //파일 비트맵 초기화
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	//이름이 const char * fileName 인 비트맵 이미지 파일 불러오기
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	//SelectObject로 hMemDC에 hBit를 집어넣고 리턴값으로 나오는 기존값을 hObit에 저장
	_imageInfo->width = width;
	_imageInfo->height = height;

	//파일 길이를 알아오기 (파일경로)
	int len = strlen(fileName);

	_fileName = new CHAR[len + 1]; //CHAR * _fileName은 CHAR의 포인터 -> 동적할당 //NULL 값 들어갈 여분의 한칸 필요 (+1)
	strcpy_s(_fileName, len + 1, fileName); //_fileName에 이 함수 인풋 fileName을 복사
	
	_trans = trans;
	_transColor = transColor;

	//알파블렌드 셋팅
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;
	//

	//초기화 실패
	if (_imageInfo->hBit == NULL)
	{
		release();

		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc); //HDC hdc = GetDC(_hWnd); 로 가져온 것을 반드시 해제해주어야 한다.

	return S_OK;
}

//파일 비트맵 초기화 + imageinfo (x,y) 값 설정
HRESULT image::init(const char * fileName, float x, float y, int width, int height, BOOL trans, COLORREF transColor)
{
	//파일이름 없으면 실패
	if (fileName == NULL) return E_FAIL;

	//이미지 정보가 뭔가 있다면 해제해준다
	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x = x;
	_imageInfo->y = y;
	_imageInfo->width = width;
	_imageInfo->height = height;

	//파일 길이를 알아온다(파일경로)
	int len = strlen(fileName);

	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_trans = trans;
	_transColor = transColor;

	//알파블렌드 셋팅
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;
	//

	//초기화가 안되었음
	if (_imageInfo->hBit == NULL)
	{
		release();

		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

//프레임 렌더 imageinfo (x,y) 좌표설정형
HRESULT image::init(const char * fileName, float x, float y, int width, int height, int frameX, int frameY, BOOL trans, COLORREF transColor)
{
	//파일이름 없으면 실패
	if (fileName == NULL) return E_FAIL;

	//이미지 정보가 뭔가 있다면 해제해준다
	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	//_imageInfo->x = x - (width / 2); //???? 이대로하면 안됨
	//_imageInfo->y = y - (height / 2); //????
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->currentFrameX = 0;
	_imageInfo->currentFrameY = 0;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;
	_imageInfo->x = x - (_imageInfo->frameWidth / 2);
	_imageInfo->y = y - (_imageInfo->frameHeight / 2);
	_imageInfo->maxFrameX = frameX - 1; //0번프레임부터 시작하므로 -1
	_imageInfo->maxFrameY = frameY - 1;

	//파일 길이를 알아온다(파일경로)
	int len = strlen(fileName);

	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_trans = trans;
	_transColor = transColor;

	//알파블렌드 셋팅
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;
	//

	//초기화가 안되었음
	if (_imageInfo->hBit == NULL)
	{
		release();

		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

//프레임 렌더 기본형 imageinfo (x,y) = (0,0)
HRESULT image::init(const char * fileName, int width, int height, int frameX, int frameY, BOOL trans, COLORREF transColor)
{
	//파일이름 없으면 실패
	if (fileName == NULL) return E_FAIL;

	//이미지 정보가 뭔가 있다면 해제해준다
	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->currentFrameX = 0;
	_imageInfo->currentFrameY = 0;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;

	//파일 길이를 알아온다(파일경로)
	int len = strlen(fileName);

	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_trans = trans;
	_transColor = transColor;

	//알파블렌드 셋팅
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->resID = 0;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;
	//

	//초기화가 안되었음
	if (_imageInfo->hBit == NULL)
	{
		release();

		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

void image::release(void) //이미지 해제 함수
{
	//이미지 정보가 있다면
	if (_imageInfo)
	{
		SelectObject(_imageInfo->hMemDC, _imageInfo->hOBit);
		DeleteObject(_imageInfo->hBit);
		DeleteDC(_imageInfo->hMemDC);

		//_imageInfo와 _fileName 둘다 포인터 변수
		SAFE_DELETE(_imageInfo); 
		SAFE_DELETE(_fileName);

		_trans = false;
		_transColor = RGB(0, 0, 0);
	}
}

void image::setTransColor(BOOL trans, COLORREF transColor)
{
	_trans = trans;
	_transColor = transColor;
}

void image::render(HDC hdc, int destX, int destY)
{
	if (_trans) //true면 복사과정에서 무시할 특정 색이 있음
	{
		GdiTransparentBlt(hdc,			//복사를 받을 영역의 DC 
			destX,						//피복사 영역의 복사시작 좌표X (left)
			destY,						//피복사 영역의 복사시작 좌표Y (top)
			_imageInfo->width,			//복사될 크기 (시작좌표X부터 가로)
			_imageInfo->height,			//복사될 크기 (시작좌표Y부터 세로)
			_imageInfo->hMemDC,			//복사할 DC (이미지안의 hMemDC)
			0, 0,						//복사 시작점 좌표
			_imageInfo->width,			//width
			_imageInfo->height,			//height
			_transColor);				//복사에서 제외할 색상
	}
	else
	{
		//무시할 색상이 없으므로 전체 고속 복사
		BitBlt(hdc, destX, destY, _imageInfo->width, _imageInfo->height, _imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}
void image::render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (_trans)
	{
		GdiTransparentBlt(hdc,		//복사될 영역의 DC		== backDC
			destX,					//복사될 좌표 X(left		== backDC의 영역의 좌표
			destY,					//복사될 좌표 Y(top		== backDC의 영역의 좌표
			sourWidth,				//복사될 크기 width		== backDC의 영역의 좌표로부터 가로크기
			sourHeight,				//복사될 크기 height		== backDC의 영역의 좌표로부터 세로크기
			_imageInfo->hMemDC,		//복사할 DC				== 이미지DC
			sourX, sourY,			//복사할 X, Y			== 이미지DC의 좌표
			sourWidth,				//복사할 width			== 이미지DC의 가로크기
			sourHeight,				//복사할 height			== 이미지DC의 세로크기
			_transColor);			//복사에서 제외할 색상	== 이미지DC에서 픽셀값
	}
	else
	{
		//백버퍼에 있는 것을 앞으로 고속복사 해준다
		BitBlt(hdc, destX, destY,
			sourWidth, sourHeight,
			_imageInfo->hMemDC, sourX, sourY, SRCCOPY);
	}
}
//렌더                     뿌려줄 곳X 뿌려줄 곳Y  destX,Y로부터 가로세로        가져올 곳X 가져올 곳Y  sourX,Y로부터 크기(가로, 세로)
void image::render(HDC hdc, int destX, int destY, int destWidth, int destHeight, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (_trans)
	{
		GdiTransparentBlt(hdc,		//복사될 영역의 DC		== backDC
			destX,					//복사될 좌표 X(left	== backDC의 영역의 좌표
			destY,					//복사될 좌표 Y(top		== backDC의 영역의 좌표
			destWidth,				//복사될 크기 width		== backDC의 영역의 좌표로부터 가로크기
			destHeight,				//복사될 크기 height	== backDC의 영역의 좌표로부터 세로크기
			_imageInfo->hMemDC,		//복사할 DC				== 이미지DC
			sourX, sourY,			//복사할 X, Y			== 이미지DC의 좌표
			sourWidth,				//복사할 width			== 이미지DC의 가로크기
			sourHeight,				//복사할 height			== 이미지DC의 세로크기
			_transColor);			//복사에서 제외할 색상	== 이미지DC에서 픽셀값
	}
	else
	{
		//백버퍼에 있는 것을 앞으로 고속복사 해준다
		BitBlt(hdc, destX, destY,
			destWidth, destHeight,
			_imageInfo->hMemDC, sourX, sourY, SRCCOPY);
	}
}

//프레임 렌더 기본형 (imageinfo currentFrame값들을 변경하지 않고 그대로 사용)
void image::frameRender(HDC hdc, int destX, int destY)
{
	if (_trans)
	{
		GdiTransparentBlt(hdc,										//복사될 영역의 DC		== backDC
			destX,													//복사될 좌표 X(left	== backDC의 영역의 좌표
			destY,													//복사될 좌표 Y(top		== backDC의 영역의 좌표
			_imageInfo->frameWidth,									//복사될 크기 width		== backDC의 영역의 좌표로부터 가로크기
			_imageInfo->frameHeight,								//복사될 크기 height	== backDC의 영역의 좌표로부터 세로크기
			_imageInfo->hMemDC,										//복사할 DC				== 이미지DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,		//복사할 X			    == 이미지DC의 좌표
			_imageInfo->currentFrameY * _imageInfo->frameHeight,	//복사할 Y				== 이미지DC의 좌표
			_imageInfo->frameWidth,									//복사할 width			== 이미지DC의 가로크기
			_imageInfo->frameHeight,								//복사할 height			== 이미지DC의 세로크기
			_transColor);											//복사에서 제외할 색상	== 이미지DC에서 픽셀값
	}
	else
	{
		//백버퍼에 있는 것을 앞으로 고속복사 해준다
		BitBlt(hdc, destX, destY,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY *_imageInfo->frameHeight, SRCCOPY);
	}
}
//프레임 렌더 imageinfo currentFrame 변경형
void image::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	if (_trans)
	{
		GdiTransparentBlt(hdc,										//복사될 영역의 DC		== backDC
			destX,													//복사될 좌표 X(left	== backDC의 영역의 좌표
			destY,													//복사될 좌표 Y(top		== backDC의 영역의 좌표
			_imageInfo->frameWidth,									//복사될 크기 width		== backDC의 영역의 좌표로부터 가로크기
			_imageInfo->frameHeight,								//복사될 크기 height	== backDC의 영역의 좌표로부터 세로크기
			_imageInfo->hMemDC,										//복사할 DC				== 이미지DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,		//복사할 X				== 이미지DC의 좌표
			_imageInfo->currentFrameY * _imageInfo->frameHeight,	//복사할 Y				== 이미지DC의 좌표
			_imageInfo->frameWidth,									//복사할 width			== 이미지DC의 가로크기
			_imageInfo->frameHeight,								//복사할 height			== 이미지DC의 세로크기
			_transColor);											//복사에서 제외할 색상	== 이미지DC에서 픽셀값
	}
	else
	{
		//백버퍼에 있는 것을 앞으로 고속복사 해준다
		BitBlt(hdc, destX, destY,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY *_imageInfo->frameHeight, SRCCOPY);
	}
}
//프레임 렌더 imageinfo x,y 값으로 destX,destY 대체
void image::frameRender(HDC hdc)
{
	if (_trans)
	{
		GdiTransparentBlt(hdc,										//복사될 영역의 DC		== backDC
			_imageInfo->x,											//복사될 좌표 X(left		== backDC의 영역의 좌표
			_imageInfo->y,											//복사될 좌표 Y(top		== backDC의 영역의 좌표
			_imageInfo->frameWidth,									//복사될 크기 width		== backDC의 영역의 좌표로부터 가로크기
			_imageInfo->frameHeight,								//복사될 크기 height		== backDC의 영역의 좌표로부터 세로크기
			_imageInfo->hMemDC,										//복사할 DC				== 이미지DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,		//복사할 X			== 이미지DC의 좌표
			_imageInfo->currentFrameY * _imageInfo->frameHeight,	//복사할 Y			== 이미지DC의 좌표
			_imageInfo->frameWidth,									//복사할 width			== 이미지DC의 가로크기
			_imageInfo->frameHeight,								//복사할 height			== 이미지DC의 세로크기
			_transColor);											//복사에서 제외할 색상	== 이미지DC에서 픽셀값
	}
	else
	{
		//백버퍼에 있는 것을 앞으로 고속복사 해준다
		BitBlt(hdc, _imageInfo->x, _imageInfo->y,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY *_imageInfo->frameHeight, SRCCOPY);
	}
}

void image::loopRender(HDC hdc, const LPRECT drawArea, int offSetX, int offSetY)
{
	//offSetX = cameraX값
	if (offSetX < 0) offSetX = _imageInfo->width + (offSetX % _imageInfo->width); 
	//-2 mod 7이 5 mod 7와 동치
	//너비 10에 offset이 -2면 8로 수정해주는 형식
	if (offSetY < 0) offSetY = _imageInfo->height + (offSetY % _imageInfo->height);

	RECT rcSour;
	int sourWidth;
	int sourHeight;

	RECT rcDest;

	//그려주는 영역을 잡아준다
	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawArea->left;
	int drawAreaH = drawArea->bottom - drawArea->top;

	//for 이중루프이므로 먼저 안에 있는 x축으로 배경을 여러장 복사한 후
	//(장수는 y < drawAreaH와 x < drawAreaW에 의해 결정되므로 무한정으로 복사하지 않는다)
	//(예: 배경이 api창 크기와 같을 경우, offSet만큼 잘린 배경 1장, 새로운 배경 1장)
	//그 연장된 그림을 y축으로 복사
	//Y축부터
	for (int y = 0; y < drawAreaH; y += sourHeight) //
	{
		rcSour.top = (y + offSetY) % _imageInfo->height; //offSet값만큼 이미지를 잘라주기 위해서 
		rcSour.bottom = _imageInfo->height;				 //top과 bottom 값 설정

		sourHeight = rcSour.bottom - rcSour.top; 
		//rcSour.top = rcSour.bottom(==_imageInfo->height) - sourHeight이므로
		//두번째 루프에서 y값에 sourHeight가 더해지면서 rcSour.top = rcSour.bottom % _imageInfo->height (==rcSour.bottom)
		//즉 rcSour.top의 값이 0이 된다. => 두번째 루프에서 sourHeight == _imageInfo->height이 된다.
		//=> 두번째 루프부터는 sourHeight == _imageInfo->height 이므로 y + sourHeight > drawAreaH 이 아닌한
		//완전한 배경 1장이 그려진다

		//그려지는 영역의 높이보다 Y축이 크면
		if (y + sourHeight > drawAreaH)
		{
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		//뿌려주는 영역
		rcDest.top = y + drawAreaY; //y=0 일때 처음 화면 (drawAreaY == 0) 그리고
									//그 다음부터는 y값이 sourHeight만큼 더해지면서 그려짐
		rcDest.bottom = rcDest.top + sourHeight;

		//X 축
		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			rcSour.left = (x + offSetX) % _imageInfo->width;
			rcSour.right = _imageInfo->width;

			sourWidth = rcSour.right - rcSour.left;

			if (x + sourWidth > drawAreaW)
			{
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			render(hdc, rcDest.left,
				rcDest.top,
				rcSour.left,
				rcSour.top,
				rcSour.right - rcSour.left,
				rcSour.bottom - rcSour.top);
		}
	}
}

void image::alphaRender(HDC hdc, BYTE alpha)
{
	_blendFunc.SourceConstantAlpha = alpha; //블렌드펌션 구조체에 alpha 값 입력

	if (_trans)
	{	//인풋으로 들어오는 hdc는 게임노드의 백버퍼DC
		//백버퍼DC를 먼저 블렌드이미지 DC에 복사하고 (배경을 블렌드 이미지에 넣어주기 위해함)
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, hdc, (int)_imageInfo->x, (int)_imageInfo->y, SRCCOPY);

		//이미지DC를 마젠타색을 지우면서 블렌드이미지 DC에 복사 
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _transColor);

		//알파블렌드는 목적 DC의 그림과 블렌드 하는 그림의 RGB값을 적절히 섞어줌
		//배경부분은 BitBlt에서 받은 배경을 똑같은 백버퍼의 배경과 겹치므로 rgb값이 그대로 -> 배경은 투명해지지 않음
		AlphaBlend(hdc, (int)_imageInfo->x, (int)_imageInfo->y, _imageInfo->width, _imageInfo->height, _blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
	else
	{
		AlphaBlend(hdc, (int)_imageInfo->x, (int)_imageInfo->y, _imageInfo->width, _imageInfo->height, _blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
}

void image::alphaRender(HDC hdc, int destX, int destY, BYTE alpha) 
{
	_blendFunc.SourceConstantAlpha = alpha; //블렌드펌션 구조체에 alpha 값 입력

	if (_trans)
	{	//인풋으로 들어오는 hdc는 게임노드의 백버퍼DC
		//백버퍼DC를 먼저 블렌드이미지 DC에 복사하고 (배경을 블렌드 이미지에 넣어주기 위해함)
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, hdc, destX, destY, SRCCOPY);

		//이미지DC를 마젠타색을 지우면서 블렌드이미지 DC에 복사 
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _transColor);

		//알파블렌드는 목적 DC의 그림과 블렌드 하는 그림의 RGB값을 적절히 섞어줌
		//배경부분은 BitBlt에서 받은 배경을 똑같은 백버퍼의 배경과 겹치므로 rgb값이 그대로 -> 배경은 투명해지지 않음
		AlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height, _blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
	else
	{
		AlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height, _blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
}

void image::alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha)
{
	_blendFunc.SourceConstantAlpha = alpha; //블렌드펌션 구조체에 alpha 값 입력

	if (_trans)
	{	//인풋으로 들어오는 hdc는 게임노드의 백버퍼DC
		//백버퍼DC를 먼저 블렌드이미지 DC에 복사하고 (배경을 블렌드 이미지에 넣어주기 위해함)
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, hdc, destX, destY, SRCCOPY);

		//이미지DC를 마젠타색을 지우면서 블렌드이미지 DC에 복사 
		GdiTransparentBlt(_blendImage->hMemDC, 0, 0, sourWidth, sourHeight, _imageInfo->hMemDC, sourX, sourY, sourWidth, sourHeight, _transColor);

		//알파블렌드는 목적 DC의 그림과 블렌드 하는 그림의 RGB값을 적절히 섞어줌
		//배경부분은 BitBlt에서 받은 배경을 똑같은 백버퍼의 배경과 겹치므로 rgb값이 그대로 -> 배경은 투명해지지 않음
		AlphaBlend(hdc, destX, destY, sourWidth, sourHeight, _blendImage->hMemDC, 0, 0, sourWidth, sourHeight, _blendFunc);
	}
	else
	{
		AlphaBlend(hdc, destX, destY, sourWidth, sourHeight, _blendImage->hMemDC, 0, 0, sourWidth, sourHeight, _blendFunc);
	}
}
