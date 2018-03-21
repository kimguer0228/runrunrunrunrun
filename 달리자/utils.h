#pragma once
//상호참조를 피하기 위해 cpp파일에 stdafx.h 참조

#define PI 3.141592654f
#define PI2 PI * 2


class gameNode;

//일주일의 고생은 이것을 위함이여찌
namespace THETA_UTIL
{
	struct ST_ImageAndRect //픽셀 충돌에 사용할 스트럭쳐
	{
		image* _image;
		RECT _rc;
	};

	float getDistance(float startX, float startY, float endX, float endY);

	float getAngle(float x1, float y1, float x2, float y2);

	bool pixelCol(ST_ImageAndRect* ir1, ST_ImageAndRect* ir2);
}