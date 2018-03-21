#include "stdafx.h" //stdafx는 참조를 모아놓은 헤더파일이므로 다른곳에서 stdafx를 참조할 경우 상호참조가 
					//아니더라도 안전하게 cpp에 적어놓기
#include "gameNode.h"

//void gameNode::setBackBuffer()
//{
//	_backBuffer = new image;
//	_backBuffer->init(WINSIZEX, WINSIZEY);
//	//_backBuffer는 개별 파일을 담을 버퍼와 다르게 빈 비트맵 초기화 함수 사용
//}

HRESULT gameNode::init(void) //WM_CREATE (윈도우프로시저 콜백함수의 switch case에 해당하는 메시지)
{
	_hdc = GetDC(_hWnd); //_hWnd (윈도우 핸들번호를 담고있는 _hWnd를 인풋으로 넣으면 그 윈도우의 DC를 내보냄)
	
	_managerInit = false;

	return S_OK; //S_OK : 때때로 Boolean TRUE 값(0X0)으로 S_FALSE와 함께 사용되며 함수가 성공하였음을 의미한다.
}

HRESULT gameNode::init(bool managerInit)
{
	_hdc = GetDC(_hWnd); //_hWnd (윈도우 핸들번호를 담고있는 _hWnd를 인풋으로 넣으면 그 윈도우의 DC를 내보냄)
	//GetDC와 ReleaseDC를 사용하면 WM_PAINT에서 쓰던 BeginPaint와 EndPaint를 쓰지 않아도 된다.
	_managerInit = managerInit;

	if (_managerInit)
	{
		//SetTimer(_hWnd, 1, 10, NULL); //_hWnd는 이 프로젝트에서 사용하는 HWND의 인스턴스 이름, 1은 타이머 이름, 10은 10ms
									  //WM_TIMER에서 기준으로 사용되는 timer 설정
		KEYMANAGER->init(); //KEYMANGER는 keyManager::getSingleton()
		IMAGEMANAGER->init();
		TIMEMANAGER->init();
		SCENEMANAGER->init();
		TXTDATA->init();
	}

	return S_OK;
}

void gameNode::release(void) //WM_DESTROY
{
	if (_managerInit)
	{
		//KillTimer(_hWnd, 1);
		KEYMANAGER->releaseSingleton(); //releaseSingleton는 keyManger엔 없고 부모인 singletoneBase에 정의되어있음
		IMAGEMANAGER->releaseSingleton();
		TIMEMANAGER->releaseSingleton();
		SCENEMANAGER->releaseSingleton();
		TXTDATA->releaseSingleton();
		
		//RND->releaseSingleton(); 커리큘럼에서는 하고 있지 않음
	}

	ReleaseDC(_hWnd, _hdc);
}

void gameNode::update(void) //WM_TIMER
{
	//InvalidateRect(_hWnd, NULL, true);
	//InvalidateRect(_hWnd, NULL, false); //더블 버퍼링을 사용하므로 이제는 false를 쓴다
	//timeManager 추가하면서 이부분 주석처리
	//WM_PAINT는 창 내부에 무효화 영역이 생기면 들어오는 메시지(창의 크기를 조정한다거나, 다른 창에 가려지거나)
	//무효화 영역이 없으면 WM_PAINT 안들어온다
	//WM_TIMER에서 InvalidateRect를 실행하면, 그 지정영역이 무효화되고 강제로 WM_PAINT 메시지가 생성됨
	//==> TIMER 지정시간마다 무효화 ==> WM_PAINT 메시지 생성
	//timeManager 추가 후 render를 WM_PAINT 메시지 생성시가 아니라 아무런 메시지가 들어오지 않을 때 실행하므로
	//InvalidateRect가 필요없어짐
	//InvalidateRect 참조:http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=636
	//무효화영역을 그릴때 
	// 무효화 영역(Invalid Region)에 대한 정보를 담기 위한 PAINTSTRUCT 구조체 변수를 선언합니다
	//PAINTSTRUCT ps;
	//
	// BeginPaint 함수의 역할은 무효화 영역(Invalid Region)만을 그릴 수 있는 DC와
	// 미리 선언한 PAINTSTRUCT 구조체 변수에 무효화 영역(Invalid Region)의 정보를 
	// 담아 주는 역할을 합니다.
	// BeginPaint 함수의 호출을 시작으로 그리기 작업이 끝나면 EndPaint 함수를 호출하여
	// 그리기 작업이 끝났음을 알려줍니다.
	//HDC h_dc = BeginPaint(hWnd, &ps);
	//
	// ..... 무효화 영역(Invalid Region)을 다시 그려주는 작업 ....
	//
	//EndPaint(hWnd, &ps);

}

void gameNode::render()
{

}

LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	//HWND HDC 참조:https://m.blog.naver.com/PostView.nhn?blogId=winterwolfs&logNo=10165689957&proxyReferer=https%3A%2F%2Fwww.google.co.kr%2F
	//hWnd : 어느 윈도우에서 메시지가 발생하는지 체크한다 //핸들 윈도우의 약자, 한 프로그램에서 여러개의 창을 띄울 수 있다(지금은 Theta API창 한개).
	//즉 여러개중에서 윈도우창마다 배정된 번호들(HWND)을 통해 구분한다.
	//iMessage : 메시지 구분 번호
	//wParam : 키보드 입력 && 마우스 클릭 메시지 전달
	//lParam : 마우스 좌표를 메시지로 전달

	//WndProc의 인수는 모두 4개이며 MSG 구조체의 멤버 4개와 동일하다.hWnd는 메시지를 받을 윈도우의 핸들이며 iMessage는 어떤 종류의 메시지인가, 
	//즉 어떤 변화가 발생했는가에 관한 정보를 가진다.iMessage가 WM_MOVE이면 윈도우의 위치가 변경되었음을 알리고 WM_DESTROY이면 윈도우가 파괴되었음을 알리는 것이다.

	//wParam, lParam은 iMessage의 메시지에 따른 부가적인 정보를 가진다.예를 들어 마우스 버튼이 눌러졌다는 WM_LBUTTONDOWN 메시지가 입력되었다면 
	//화면의 어디쯤에서 마우스 버튼이 눌러졌는가, 그때의 키보드 상황(Shift, Ctrl, Alt)은 어떠한가에 관한 정보가 추가로 필요하며 WM_CHAR 메시지, 
	//즉 키보드로부터 키가 입력되었다는 메시지가 입력되었다면 어떤 키가 입력되었는가에 관한 추가적인 정보가 필요하다.이런 추가 정보들이 wParam, lParam으로 전달된다.
	//wParam, lParam에 실제로 어떤 값을 전달되는지는 메시지별로 다르다.

	//PAINTSTRUCT ps; //그림을 그려주는 붓같은 구조체
	//HDC hdc; //Handle Device Context. Device Context는 출력에 필요한 정보를 가지는 데이터 구조체로 좌표, 색, 굵기 등 출력에 필요한 모든 정보를 담고 있다. %%HDC는 포인터가 아니다 (무슨 의미?)
	//%%더이상 WM_PAINT에서 BeginPaint, EndPaint로 그리지 않고 외부(mainGame의 render)에서 그리므로 위 두변수는 필요없다.

	////HDC hMemDC; //메모리 속에서 미리 그림을 그려놓을 때 사용할 것
	////HBITMAP hBitmap, OldBitmap; //HBITMAP은 일종의 비트맵 도화지.  참조:http://soen.kr/lecture/win32api/lec6/lec6-4-2.htm 참조2:http://hyeongjin-kim.blogspot.kr/2013/05/ddb-dib.html (DDB(HBITMAP)와 DIB 차이)
	//////RECT windowRect; //윈도우 창의 크기를 담아놓을 RECT //WINSIZEX와 WINSIZEY를 대신 사용해보기

	switch (iMessage)
	{
		//윈도우가 처음 켜지면 딱 한 번 실행되는 곳(초기화)
		//윈도우가 처음 켜졌다는 신호가 WM_CREATE
	case WM_CREATE:
	{
		//this->init(); 이부분에서 init()을 실행해주지 않아도 
		//winMain의 FAILED(_mg.init())가 알아서 실행시켜준다
		break;
	}

	//timeManager 도입할 때 빠짐
	//case WM_PAINT:
	//{
	//	//더블버퍼링 참조1:http://3dmpengines.tistory.com/1520
	//	//참조2:http://heart4u.co.kr/tblog/330
	//	//BitBlt, TransparentBlt 등의 대한 내용 참조:http://skmagic.tistory.com/255
	//
	//
	//	//BeginPaint : 초기화 함수? 또는 시작 정보를 넣어주는 함수
	//	hdc = BeginPaint(hWnd, &ps); //&ps는 ps가 아니라 ps의 주소 //예: int a = 33; int *b = &a; 
	//
	//	//////GetClientRect(hWnd, &windowRect); //hWnd의 창 범위를 windowRect에 저장한다.
	//	////
	//	////hMemDC = CreateCompatibleDC(hdc); //hMemDC에 기존 DC와 동일한 좌표, 색, 굵기등의 정보를 넣기
	//	////hBitmap = CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY); //hdc의 정보(색, 굵기 등)를 가지고 windowRect 크기대로 비트맵 도화지 생성
	//	////														   //hdc와 호환되는 포맷의 비트맵을 windowRect.righ, windowRect.bottom 크기로 만든다. 여기서 "호환된다"는 말은 색상 포맷이 같다는
	//	////														   //뜻이며 더 정확하게 표현하자면 색상면과 픽셀당 비트수가 같다는 뜻이다.
	//	////														   //즉 화면이 256모드면 비트맵도 256비트맵이 되고 트루 컬러 화면이면 비트맵도 트루 컬러로 만들어 진다는 뜻이다. http://hyeongjin-kim.blogspot.kr/2013/05/ddb-dib.html
	//	////
	//	////OldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap); //SelectObject는 HBITMAP과 같은 GDI 오브젝트들의 핸들(네임) 값을 변경하는 함수 참조:http://blog.naver.com/PostView.nhn?blogId=tipsware&logNo=220988055352&categoryNo=57&parentCategoryNo=0&viewDate=&currentPage=1&postListTopCurrentPage=1&from=postView
	//	////													//SelectObject(HDC 교체할 오브젝트를 담은 DC,HGDIOBJ 새롭게 설정할 GDI 오브젝트의 핸들값) bitmap, pen, brush, font, region 변경가능
	//	////													//이 함수는 GDI 오브젝트 교체 작업이 성공하면, DC가 이전에 사용하고 있던 GDI 오브젝트의 핸들값을 반환하고 (여기서 그 값을 OldBitmap에 저장하고있다.) 실패하면 NULL 값을 반환한다.
	//	////													//예: 새 Brush 객체의 핸들값이 0x10000012이고 기존에 DC에 저장되어 있던 Brush 객체 핸들 값이 0x30000412인 경우, 
	//	////													//함수를 사용하면 DC에 저장되어있던 brush 핸들 값은 0x10000012로 변경되고 0x30000412를 반환한다.    
	//	////
	//	////													//형변환을 하는 이유는 두번째 인수의 타입이자 함수가 내보내는 Return 값의 자료형인 HGDIOBJ이 HBITMAP,HPEN, HBRUSH, HFONT를 포함하는 상위자료형이기때문에
	//	////													//HGDIOBJ h_gdiobj = h_brush; 는 가능하지만 반대로 HBRUSH h_brush = h_gdiobj는 에러가 난다. ==>HBRUSH h_brush = (HBITMAP)h_gdiobj으로 형변환을 할 필요가 있다.
	//	////
	//	//////위에서 BITMAP (도화지)를 교체하였으므로, 도화지를 하얗게 칠한다. (비트맵은 기본 검정색 배경을 가짐)
	//	////HBRUSH BkBrush = CreateSolidBrush(RGB(255, 255, 255));
	//	////RECT windowRect = { WINSIZEX , WINSIZEY };
	//	////FillRect(hMemDC, &windowRect, BkBrush);
	//	////DeleteObject(BkBrush);
	//	//////delete &windowRECT;
	//	////
	//	////this->render(hMemDC); //여기서 this는 자손에서 MainProc을 사용할 경우 그 자손을 지칭 
	//	//////더블 버퍼링을 사용하므로 이제 hdc가 아니라 hMemDC를 인풋으로 넣는다
	//	////BitBlt(hdc, 0, 0, WINSIZEX, WINSIZEY, hMemDC, 0, 0, SRCCOPY); //hMemDC안에 0,0 좌표에서부터, windowRect.right의 너비, windowRect.bottom의 높이를 가지는 부분을 hdc의 0,0좌표에 SRCCOPY방식으로 복사한다.
	//	////															  //SRCCOPY : 소스 영역을 대상 영역에 복사한다.
	//	////															  //DSTINVERT: 화면을 반전시킨다.
	//	////															  //MERGECOPY: 소스 비트맵과 대상 화면을 AND 연산한다.
	//	////															  //MERGEPAINT: 소스 비트맵과 대상 화면을 OR 연산한다.
	//	////															  //WHITENESS: 대상영역을 흰색으로 채운다.
	//	////															  //BLACKNESS: 대상영역을 검정색으로 가득 채운다.
	//	////															  //참조:http://soen.kr/lecture/win32api/lec6/lec6-4-3.htm
	//	////
	//	////DeleteObject(SelectObject(hMemDC, OldBitmap)); //SelectObject는 오브젝트를 교체함과 동시에 교체되어 나온 오브젝트를 리턴하므로 OldBitmap을 hMemDC에 다시 되돌리고 hBitmap을 내보낸다
	//	////											   // 바로 밑에서 DC 정보를 지워주는데 오브젝트를 바꾸고 지울 필요가 있을까?
	//	////											   // 이게 없으면 리소스 리크가 일어난다? https://stackoverflow.com/questions/8875395/win32-double-buffering-drawing-black-background 의 댓글
	//	////											   // %%%%% hMemDC에는 SelectObject에 의해 OldBitmap이 들어가서DeleteDC(hMemDC);으로 삭제되지만, hBitmap는 이제 hMemDC밖으로 나와 남아있게된다. 반드시 DeleteObject로 메모리 할당을 풀어줘야 한다.
	//	////DeleteDC(hMemDC);
	//	//////ReleaseDC()는 메모리에 할당된 화면 DC를 해제하기 위한 용도로 사용됩니다.
	//	//////화면 DC 역시 메모리에 할당된 DC의 핸들을 가지고 있기 때문에 반드시 해제해야 합니다.
	//	//////DeleteDC()는 메모리에 할당되고 메모리상에서 사용하는 DC를 해제하기 위한 용도로 사용됩니다.
	//	//////간단하기 말하자면, 화면에 보이는 것은 ReleaseDC()로 화면에 보이지 않고 메모리상에만 할당된
	//	//////DC는 DeleteDC()로 해제해야 합니다. 참조:http://myfreechild.tistory.com/m/37
	//
	//
	//	this->render();
	//	EndPaint(hWnd, &ps); //hdc를 GetDC로 받아오지 않고 BeginPaint로 사용했으므로
	//						 //ReleaseDC대신 EndPaint 사용
	//	break;
	//}
	//
	//case WM_TIMER:
	//{
	//	this->update();
	//	break;
	//}

	case WM_MOUSEMOVE:
	{
		_ptMouse.x = static_cast<float>LOWORD(lParam);
		_ptMouse.y = static_cast<float>HIWORD(lParam);

		//_ptMouse.x = LOWORD(lParam); 
		//_ptMouse.y = HIWORD(lParam);
		//이 둘을 형변환해서 사용중 .

		//c++ 형변환
		//static_cast
		//reinterpret_cast
		//쓰지마 밑에것 2개 공부만해라
		//dynamic_cast
		//const_cast

		break;
	}

	//키보드 이벤트 관련 : http://jurang5.tistory.com/
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			PostMessage(hWnd, WM_DESTROY, 0, 0); //hWnd의 iMessage에 WM_DESTROY 메세지를 보내줌
			break;
		}
		}
		break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

gameNode::gameNode()
{
}

gameNode::~gameNode()
{
}
