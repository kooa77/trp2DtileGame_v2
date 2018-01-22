#include "GameSystem.h"
#include "Texture.h"
#include "Frame.h"

Frame::Frame()
{
	_frameTime = 0.0f;
}

Frame::~Frame()
{
}

void Frame::Init(Texture* texture, int x, int y, int width, int height, float rotate, float frameTime)
{
	_texture = texture;

	// 이미지에서 출력에 사용될 영역을 지정한다.
	_textureRect.left = x;
	_textureRect.right = _textureRect.left + width;
	_textureRect.top = y;
	_textureRect.bottom = _textureRect.top + height;

	// 텍스쳐에 적용될 컬러
	_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	_width = width;
	_height = height;

	_rotate = rotate;

	_frameTime = frameTime;
}

void Frame::Render()
{
	// 크기조정
	D3DXVECTOR2 scale = D3DXVECTOR2(1.0f, 1.0f);

	// 회전
	D3DXVECTOR2 center = D3DXVECTOR2(_width / 2.0f, _height / 2.0f);
	float rotate = _rotate;

	// 위치 결정
	float x = _x - _width / 2.0f;
	float y = _y - _height / 2.0f;
	D3DXVECTOR2 translate = D3DXVECTOR2(x, y);

	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,		// 연산결과
		NULL,			// 스케일의 중심점
		0.0f,			// 확대 회전 요소.
		&scale,			// 확대. 크기 결정
		&center,		// 회전 중심
		rotate,			// 회전 값
		&translate		// 평행 이동 -> 위치가 결정
	);

	GameSystem::GetInstance()->GetSpriteDX()->SetTransform(&matrix);
	GameSystem::GetInstance()->GetSpriteDX()->Draw(_texture->GetTextureDX(), &_textureRect, NULL, NULL, _textureColor);
}

void Frame::Relase()
{
}

void Frame::Reset()
{
	//_spriteDX = spriteDX;
}

void Frame::SetPosition(float x, float y)
{
	_x = x;
	_y = y;
}