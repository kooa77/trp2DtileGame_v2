#include "GameSystem.h"
#include "Font.h"


Font::Font(std::wstring fontName, int fontSize, D3DCOLOR fontColor)
{
	//ID3DXFont* testFontDX;
	HRESULT hr = D3DXCreateFont(GameSystem::GetInstance()->GetDeviceDX(),
		fontSize, 0, FW_BOLD, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		fontName.c_str(), &_fontDX);

	if (FAILED(hr))
	{
		MessageBox(0, L"Failed CreateFont", 0, 0);
	}

	_fontName = fontName;
	_fontSize = fontSize;

	_color = fontColor;
}

Font::~Font()
{
}

void Font::Render()
{
	// 행렬을 생성
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, NULL, 0.0f, NULL);
	GameSystem::GetInstance()->GetSpriteDX()->SetTransform(&matrix);
	_fontDX->DrawTextW(GameSystem::GetInstance()->GetSpriteDX(),
		_text.c_str(), -1, &_rect, DT_CENTER, _color);
}

void Font::Release()
{
	if (NULL != _fontDX)
	{
		_fontDX->Release();
		_fontDX = NULL;
	}
}

void Font::Reset()
{
	HRESULT hr = D3DXCreateFont(GameSystem::GetInstance()->GetDeviceDX(),
		_fontSize, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		_fontName.c_str(), &_fontDX);

	if (FAILED(hr))
	{
		MessageBox(0, L"Failed CreateFont", 0, 0);
	}
}

void Font::SetPosition(int x, int y)
{
	_rect.left = x;
	_rect.top = y;
	_rect.right = _rect.left + _width;
	_rect.bottom = _rect.top + _height;
}

void Font::SetRect(int x, int y, int width, int height)
{
	_width = width;
	_height = height;

	_rect.left = x;
	_rect.top = y;
	_rect.right = _rect.left + width;
	_rect.bottom = _rect.top + height;
}

void Font::SetText(std::wstring text)
{
	_text = text;
}
