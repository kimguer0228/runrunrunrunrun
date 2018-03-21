#pragma once

inline void LineMake(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL); // hdc 도화지 위에 (x1,y1)을 시작점으로 정한다
	LineTo(hdc, x2, y2); //정해진 시작점에서 (x2,y2)로 선을 그어준다
}


inline RECT RectMake(int x, int y, int width, int height) //시작점, 너비, 높이를 받아서 구조체(클래스) RECT를 내보내는 함수
{
	RECT rc = { x, y, x + width, y + height };


	//RECT rc;
	//rc.left = x;
	//rc.top = y;
	//rc.right = x + width;
	//rc.bottom = y + height; 와 같이 하나하나 하지않고 위와 같이 간단히 해도 된다.

	return rc;
}

//RECT rc가 정의된 후에 Rectangle을 만들때에는 rc.left등으로 정보를 불러들여 사용하면 된다.

// 중점좌표, 너비, 높이를 주면 RECT을 내보내는 함수
inline RECT RectMakeCenter(int x, int y, int width, int height)
{
	RECT rc = { x - (width / 2), y - (height / 2), x + (width / 2), y + (height / 2) };

	return rc;
}

//사각형을 hdc 도화지에 그리는 함수
inline void RectangleMake(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x, y, x + width, y + height);
}


inline void RectangleMakeCenter(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x - (width / 2), y - (height / 2), x + (width / 2), y + (height / 2));
}

//원을 hdc 도화지에 그리는 함수 
inline void EllipseMake(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x, y, x + width, y + height);
}

inline void EllipseMakeCenter(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x - (width / 2), y - (height / 2), x + (width / 2), y + (height / 2));
}