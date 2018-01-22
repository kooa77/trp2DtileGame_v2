#include <Windows.h>
#include <d3dx9.h>
#include <list>
#include "InputSystem.h"
#include "ResourceManager.h"
#include "ComponentSystem.h"
#include "GameSystem.h"
#include "GameTimer.h"
#include "Map.h"
#include "NPC.h"
#include "Monster.h"
#include "Player.h"
#include "RecoveryItem.h"
#include "Sprite.h"
#include "Font.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	int clientWidth = 1280;
	int clientHeight = 800;
	bool isWindow = true;

	// 윈도우 응용프로그램(창) 작동 스타일 만들고 등록
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;	// 창의 스타일 지정
	wc.lpfnWndProc = WndProc;	// 윈도우 프로시저
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;	// 응용프로그램 인스턴스 핸들 연결
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);	// 아이콘
	wc.hCursor = LoadCursor(0, IDC_ARROW);	// 커서
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// 배경 색깔
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"GameAppStyle";	// 응용프로그램의 스타일 대표이름
	if (!RegisterClass(&wc))
	{
		return 0;
	}

	// 윈도우 창을 생성하고 핸들을얻음
	DWORD style;
	if (isWindow)
	{
		style = WS_OVERLAPPEDWINDOW;
	}
	else
	{
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	}
	
	HWND hWnd = CreateWindow(
		L"GameAppStyle",	// 응용프로그램의 스타일
		L"2D Tile Game",	// 창의 제목
		style,
		CW_USEDEFAULT, CW_USEDEFAULT,	//x, y
		clientWidth, clientHeight ,	//너비, 높이
		0,	// 부모 창이 있을 시, 부모창의 핸들
		0,	// 메뉴 핸들.
		hInstance,	// 만들어지는윈도우 창을 사용할 응용 프로그램 핸들 세팅
		0
	);
	if (NULL == hWnd)
	{
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 창모드 이면 클라이언트 크기를 조정
	if (true == isWindow)
	{
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		MoveWindow(hWnd,
			0, 0,
			clientWidth + (clientWidth - clientRect.right),
			clientHeight + (clientHeight - clientRect.bottom),
			TRUE
		);
	}


	// DirectX

	HRESULT hr;

	// Direct3D 객체를 생성
	LPDIRECT3D9 direct3d;
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (NULL == direct3d)
	{
		return 0;
	}

	// 어떤방식으로 보여줄 것인지 설정
	D3DFORMAT format;
	if(isWindow)
	{
		format = D3DFMT_UNKNOWN;
	}
	else
	{
		format = D3DFMT_X8R8G8B8;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = clientWidth;
	d3dpp.BackBufferHeight = clientHeight;
	d3dpp.BackBufferFormat = format;
	d3dpp.BackBufferCount = 1;	// 백 버퍼의 갯수
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = isWindow;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// DirectX 디바이스를 생성. 디바이스를 통해서 하드웨어에 직접 접근이 가능
	LPDIRECT3DDEVICE9 device3d;
	hr = direct3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&device3d
	);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed CreateDevice", L"Error", MB_OK);
		return 0;
	}

	// Sprite COM 인터페이스 얻기
	ID3DXSprite* spriteDX;
	hr = D3DXCreateSprite(device3d, &spriteDX);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed D3DXCreateSprite", L"Error", MB_OK);
		return 0;
	}

	GameSystem::GetInstance()->SetClientWidth(clientWidth);
	GameSystem::GetInstance()->SetClientHeight(clientHeight);
	GameSystem::GetInstance()->SetDeviceDX(device3d);
	GameSystem::GetInstance()->SetSpriteDX(spriteDX);

	std::list<Component*> stageComponentList;

	Map* map = new Map(L"Map");
	map->Init(L"MapSprite.png", L"MapData_Layer");
	stageComponentList.push_back(map);

	// 회복 아이템 배치
	for (int i = 0; i < 10; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"recovery_item_%d", i);
		RecoveryItem* item = new RecoveryItem(name);
		item->Init(L"item_pack.png", L"recovery_item");
		stageComponentList.push_back(item);
	}

	Character* character = new Player(L"player");
	character->Init(L"character_sprite.png", L"player");
	stageComponentList.push_back(character);

	for (int i = 0; i < 20; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"npc_%d", i);
		Character* npc = new NPC(name);
		npc->Init(L"character_sprite_pack.png", L"npc");
		stageComponentList.push_back(npc);
	}

	for (int i = 0; i < 10; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"monster_%d", i);
		Monster* monster = new Monster(name);
		monster->Init(L"monster_sprite_pack.png", L"npc");
		stageComponentList.push_back(monster);
	}
	
	map->SetViewer(character);

	// FPS 결정 (60fps)
	float frameTime = 1.0f / 60.0f;
	float frameDuration = 0.0f;

	GameTimer gameTimer;
	gameTimer.Init();

	// 메시지 배분
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);	// 키코드를 문자 메시지 변화.
			DispatchMessage(&msg);	// 메시지를 창으로 배분.
		}
		else
		{
			gameTimer.Update();
			float deltaTime = gameTimer.GetDeltaTime();

			ComponentSystem::GetInstance()->Update(deltaTime);

			for (std::list<Component*>::iterator it = stageComponentList.begin();
				it != stageComponentList.end(); it++)
			{
				(*it)->Update(deltaTime);
			}

			frameDuration += deltaTime;
			if (frameTime <= frameDuration)
			{
				frameDuration = 0;

				device3d->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(124, 124, 124), 0.0f, 0);
				{
					device3d->BeginScene();
					{
						spriteDX->Begin(D3DXSPRITE_ALPHABLEND);
						{
							map->Render();
						}
						spriteDX->End();
					}
					device3d->EndScene();
				}

				// 디바이스가 유실되었으면, 다시 복구 해줘서 정상출력이 되도록 해줘야한다.
				{
					// 디바이스 상태 확인
					hr = device3d->TestCooperativeLevel();
					if (FAILED(hr))	// 디바이스 상태에 문제 발생
					{
						// 디바이스 유실 상태 확인
						if (D3DERR_DEVICELOST == hr)	// 복구 불가인 상태
						{
							Sleep(100);	// 복구 가능한 상태로 바뀔 때 까지 기다린다.
						}
						else if (D3DERR_DEVICENOTRESET == hr)	// 로스트 상태이지만, 복구 가능 상태
						{
							for (std::list<Component*>::iterator it = stageComponentList.begin();
								it != stageComponentList.end(); it++)
							{
								(*it)->Release();
							}

							direct3d = Direct3DCreate9(D3D_SDK_VERSION);
							if (NULL != direct3d)
							{
								hr = direct3d->CreateDevice(
									D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp,
									&device3d
								);
								if (SUCCEEDED(hr))
								{
									hr = D3DXCreateSprite(device3d, &spriteDX);
									if (SUCCEEDED(hr))
									{
										for (std::list<Component*>::iterator it = stageComponentList.begin();
											it != stageComponentList.end(); it++)
										{
											(*it)->Reset();
										}
									}
								}
							}
						}
					}							
				}

				device3d->Present(NULL, NULL, NULL, NULL);
			}
		}
	}

	// 프로그램이 끝나기 전에, 사용 했던자원을해제한다.

	ComponentSystem::GetInstance()->RemoveAllComponents();
	/*
	character->Deinit();
	delete character;
	character = NULL;

	map->Deinit();
	delete map;
	map = NULL;
	*/

	ResourceManager::GetInstance()->RemoveAllTexture();

	if (spriteDX)
	{
		spriteDX->Release();
		spriteDX = NULL;
	}
	if (device3d)
	{
		device3d->Release();
		device3d = NULL;
	}
	if (direct3d)
	{
		direct3d->Release();
		direct3d = NULL;
	}

	return 0;
}


// 윈도우 프로시저함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 메시지 처리
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		return 0;
	case WM_KEYDOWN:
		InputSystem::GetInstance()->KeyDown(wParam);
		return 0;
	case WM_KEYUP:
		if (VK_ESCAPE == wParam)
		{
			DestroyWindow(hWnd);
		}
		InputSystem::GetInstance()->KeyUp(wParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}