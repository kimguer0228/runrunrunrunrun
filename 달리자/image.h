#pragma once
//image 헤더는 stdafx가 아니라 gameNode에서 참조함
class image
{
public:
	enum IMAGE_LOAD_KIND //아직 사용안함
	{
		LOAD_RESOURCE,     //리소스로부터 로드
		LOAD_FILE,         //파일로부터
		LOAD_EMPTY,        //Make Empty BMP file
		LOAD_END
	};

	typedef struct tagImageInfo
	{
		//DWORD는 호환이 아예 불가능하다 4Byte 딱 그 이상 그 이하도 아님
		//Long은 32비트까지는 호환이 가능한 4Byte다
		//int는 64비트까지 호환이 가능한 4Byte다
		//당연하겠지만 호환이 가능하다면 불안하기 때문에 컴퓨터는
		//호환을 하지 않는 DWORD를 좋아한다

		DWORD		resID;				//이 구조체의 인스턴스 이름을 담을 DWORD?
		HDC			hMemDC;				//이 image 클래스의 그림도구
		HBITMAP		hBit;				//이미지 인포 안에 비트맵 도화지가 들어가있음
		HBITMAP		hOBit;				//이미지 인포 안에 비트맵 도화지가 들어가있음
		float x;						//BackDC로 복사될때 사용할 x좌표
		float y;						//BackDC로 복사될때 사용할 y좌표
		int			width;				//hBit의 가로길이 값 저장
		int			height;				//hBit의 세로길이 값 저장
		int			currentFrameX;		//프레임 이미지 번호(가로) (열에 대한 정보)
		int			currentFrameY;		//프레임 이미지 번호(세로) (행에 대한 정보)
		int			maxFrameX;			//프레임 최대 갯수(가로)
		int			maxFrameY;			//프레임 최대 갯수(세로)
		int			frameWidth;			//프레임 이미지 한장 가로크기
		int			frameHeight;		//프레임 이미지 한장 세로크기
		BYTE		loadType;

		//이 스트럭트의 생성자
		tagImageInfo()
		{
			resID = 0;
			hMemDC = NULL;
			hBit = NULL;
			hOBit = NULL;
			x = 0;
			y = 0;
			width = 0;
			height = 0;
			currentFrameX = 0;
			currentFrameY = 0;
			maxFrameX = 0;
			maxFrameY = 0;
			frameWidth = 0;
			frameHeight = 0;
			loadType = LOAD_RESOURCE;
		} 
		//*LPIMAGE_INFO를 해놓는 이유는 이미지를 여러 곳에 할당할 수있게
		//하기 위함(접근용의)
	}IMAGE_INFO, *LPIMAGE_INFO; //*가 붙어서 LPIMAGE_INFO는 tagImageInfo 스트럭트를 가리키는 포인터가 됨

private:
	LPIMAGE_INFO    _imageInfo;		// 이미지정보 구조체의 포인터 tagImageinfo == *LPIMAGE_INFO
	CHAR*		    _fileName;		//파일이름 _fileName = new CHAR[len+1];로 런타임에 array 사이즈 결정함
	BOOL			_trans;			//특정 색을 복사 시 무시할지 여부
	COLORREF		_transColor;	//특정 색 레퍼런스

	BLENDFUNCTION	_blendFunc;		//알파블렌드 함수 얼마나 투명할지를 결정할 알파 값이 렌더 때 여기에 들어감
	LPIMAGE_INFO	_blendImage;	//알파블렌드 이미지, 알파블렌드 시에 배경과 이미지를 혼합할 새로운 memDC를 포함함

public: 
	image();
	~image();

	//빈 비트맵 초기화
	HRESULT init(int width, int height);
	//파일 비트맵 초기화
	HRESULT init(const char* fileName, int width, int height, BOOL trans = FALSE, COLORREF transColor = FALSE);
	//함수 사용할 때, 4번째 5번째 인수를 적지 않아도 기본값 FALSE가 들어감
	//다른 값을 사용하고 싶을때만 명시적으로 집어넣어주면 됨
	
	//이미지 DC의 그림 시작 좌표 (x,y) 지정가능 init함수 
	//(엄밀히는 이미지 DC의 좌표가 바뀌진 않고 imageinfo 안의 x,y 값만 바뀐다) %%나중에 setX,setY로 정해도 됨
	HRESULT init(const char* fileName, float x, float y, int width, int height,
		BOOL trans = FALSE, COLORREF transColor = FALSE);
	//프레임 렌더 imageinfo (x,y) 좌표설정형
	HRESULT init(const char* fileName, float x, float y, int width, int height,
		int frameX, int frameY, BOOL trans, COLORREF transColor);
	//프레임 렌더 기본형 imageinfo (x,y) = (0,0)
	HRESULT init(const char* fileName, int width, int height,
		int frameX, int frameY, BOOL trans, COLORREF transColor);

	//이미지 해제
	void release(void);

	//무시할 특정 색 설정 여부
	void setTransColor(BOOL trans, COLORREF transColor);

	void render(HDC hdc, int destX, int destY);
	//렌더               뿌려줄 곳X 뿌려줄 곳Y   가져올 곳X 가져올 곳Y  sourX,Y로부터 크기(가로, 세로)
	void render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);
	//렌더               뿌려줄 곳X 뿌려줄 곳Y  destX,Y로부터 가로세로       가져올 곳X 가져올 곳Y  sourX,Y로부터 크기(가로, 세로)
	void render(HDC hdc, int destX, int destY, int destWidth, int destHeight, int sourX, int sourY, int sourWidth, int sourHeight);
	//프레임 렌더 기본형 (imageinfo currentFrame값들을 변경하지 않고 그대로 사용)
	void frameRender(HDC hdc, int destX, int destY);
	//프레임 렌더 imageinfo currentFrame 변경형
	void frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);
	//프레임 렌더 imageinfo x,y 값으로 destX,destY 대체
	void frameRender(HDC hdc);
	//배경 루프
	void loopRender(HDC hdc, const LPRECT drawArea, int offSetX, int offSetY);

	//알파 블렌드
	//alpha 는 1 byte = 8 bits => 2^8 = 256 => 0~255 까지 값 0:투명, 255:완전불투명
	void alphaRender(HDC hdc, BYTE alpha); //기본 알파블렌드
	void alphaRender(HDC hdc, int destX, int destY, BYTE alpha); //목적지 지정
	void alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha); //목적지, 원본 내 좌표 지정


	//DC(그림도구모음)을 내보내주는 함수
	inline HDC getMemDC(void) { return _imageInfo->hMemDC; };
	//LPIMAGE_INFO    _imageInfo는 tagImageInfo의 포인터이므로 ->을 사용해야함

	//========================================================
	// 이미지 조작을 간편하게 위한 접근,설정자
	//========================================================

	inline void setX(float x) { _imageInfo->x = x; }
	inline float getX(void) { return _imageInfo->x; }

	inline void setY(float y) { _imageInfo->y = y; }
	inline float getY(void) { return _imageInfo->y; }

	inline int getWidth(void) { return _imageInfo->width; }
	inline int getHeight(void) { return _imageInfo->height; }

	inline void setCenter(float x, float y)
	{
		_imageInfo->x = x - (_imageInfo->width / 2);
		_imageInfo->y = y - (_imageInfo->height / 2);
	}

	inline float getCenterX()
	{
		return _imageInfo->maxFrameX <= 0 ? _imageInfo->x + (_imageInfo->width / 2) :
			_imageInfo->x + (_imageInfo->frameWidth / 2);
	}

	inline float getCenterY()
	{
		return _imageInfo->maxFrameY <= 0 ? _imageInfo->y + (_imageInfo->height / 2) :
			_imageInfo->y + (_imageInfo->frameHeight / 2);
	}

	inline void setFrameX(int frameX)
	{
		_imageInfo->currentFrameX = frameX;
		if (_imageInfo->maxFrameX < frameX) _imageInfo->maxFrameX = frameX;
	}

	inline void setFrameY(int frameY)
	{
		_imageInfo->currentFrameY = frameY;
		if (_imageInfo->maxFrameY < frameY) _imageInfo->maxFrameY = frameY;
	}

	inline int getMaxFrameX(void) { return _imageInfo->maxFrameX; }
	inline int getMaxFrameY(void) { return _imageInfo->maxFrameY; }

	inline int getFrameX(void) { return _imageInfo->currentFrameX; }
	inline int getFrameY(void) { return _imageInfo->currentFrameY; }

	inline int getFrameWidth(void) { return _imageInfo->frameWidth; }
	inline int getFrameHeight(void) { return _imageInfo->frameHeight; }

};