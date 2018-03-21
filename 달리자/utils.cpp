#include "stdafx.h" //상호참조를 피하기 위해 cpp파일에 stdafx.h 참조
#include "utils.h"
#include "gameNode.h"


//네임스페이스는 정의해두면 여러분이 편하게 쓸수있다
namespace THETA_UTIL
{
	float getDistance(float startX, float startY, float endX, float endY)
	{
		float x = endX - startX;
		float y = endY - startY;

		return sqrtf(x * x + y * y);
	}

	float getAngle(float x1, float y1, float x2, float y2)
	{
		float x = x2 - x1;
		float y = y2 - y1;

		float distance = sqrtf((x * x) + (y * y));

		float angle = acosf(x / distance);

		//Y축은 보정이 필요하다
		if (y2 > y1)
		{
			angle = PI * 2 - angle;
			if (angle >= PI * 2) angle -= PI * 2;
		}

		return angle;
	}
	bool pixelCol(ST_ImageAndRect * ir1, ST_ImageAndRect * ir2)
	{
		RECT tempRC; //충돌영
		if (!IntersectRect(&tempRC, &(ir1->_rc), &(ir2->_rc))) //두 이미지를 담은 렉트 사이 충돌이 없을 경우
		{
			return false;
		}
		
		//for 구문 안에서 사용될 변수들
		COLORREF color1;
		COLORREF color2;
		int r1;
		int g1;
		int b1;
		int r2;
		int g2;
		int b2;

		//기본 아이디어
		//렉트충돌로 충돌한 영역을 얻어와서
		//얻어온 영역에서 두 그림에 대하여 모든 픽셀에서 충돌체크를 한다
		//두 그림 모두 마젠타가 아닌 색깔이 잇는 픽셀이 하나라도 존재한다면 true
		for (int i = tempRC.left; i < tempRC.right; i++)
		{
			for (int j = tempRC.top; j < tempRC.bottom; j++)
			{
				color1 = GetPixel(ir1->_image->getMemDC(), i - ir1->_rc.left, j - ir1->_rc.top);
				//_image안의 DC에서 그림은 0,0부터 그려져있으므로 (i - ir1->_rc.left, j - ir1->_rc.top) 가 맞는 좌표 
				color2 = GetPixel(ir2->_image->getMemDC(), i - ir2->_rc.left, j - ir2->_rc.top);

				r1 = GetRValue(color1);
				g1 = GetGValue(color1);
				b1 = GetBValue(color1);

				r2 = GetRValue(color2);
				g2 = GetGValue(color2);
				b2 = GetBValue(color2);

				if (!(r1 == 255 && g1 == 0 && b1 == 255) && !(r2 == 255 && g2 == 0 && b2 == 255))
				{	//ir1, ir2 의 이미지에서 (i,j) 지점이 둘 다 마젠타 색이 아닌 경우  
					return true;
				}
			}
		}

		return false;
	}
}