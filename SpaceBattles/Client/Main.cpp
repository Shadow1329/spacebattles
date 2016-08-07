#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>


#define MAX_FPS 30


//Struct of D3DVertex
struct D3DVERTEX
{
	D3DXVECTOR3 position;	// The position
	D3DXVECTOR3 normal;		//Normal
	DWORD color;			// The color
	FLOAT tu, tv;   // The texture coordinates
};
#define D3DFVF_VERTEXPARAMS (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_DIFFUSE)


//Struct camera
struct Camera
{
	float px, py, pz;
	float rx, ry, rz;
	float pspeed;
	float rspeed;
};




//переменные
HWND g_hWnd = NULL;
IDirect3D9 *g_d9 = NULL;
IDirect3DDevice9 *g_d9device = NULL;
IDirect3DVertexBuffer9 *g_d9verbuf = NULL;

IDirect3DTexture9 *g_Texture = NULL;

Camera g_camera;

bool g_keyboard[128];

int g_tick = -1;


//объявление функций
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);  // Создание окна
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);       // Функция окна










															//Direct3D init
bool InitDirect3D()
{
	//Init direct3d
	g_d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_d9 == NULL)
	{
		MessageBox(NULL, L"Direct3D init error", L"Error!", MB_ICONERROR);
		return 0;
	}


	//Init direct3d device
	D3DPRESENT_PARAMETERS d3dpparams;
	ZeroMemory(&d3dpparams, sizeof(d3dpparams));
	d3dpparams.Windowed = true;
	d3dpparams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpparams.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpparams.EnableAutoDepthStencil = TRUE;
	d3dpparams.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT d3er = g_d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpparams, &g_d9device);
	if (g_d9device == NULL)
	{
		if (d3er == D3DERR_DEVICELOST) MessageBox(NULL, L"Direct3D device lost", L"Error!", MB_ICONERROR);
		if (d3er == D3DERR_INVALIDCALL) MessageBox(NULL, L"Direct3D invalid call", L"Error!", MB_ICONERROR);
		if (d3er == D3DERR_NOTAVAILABLE) MessageBox(NULL, L"Direct3D not available", L"Error!", MB_ICONERROR);
		if (d3er == D3DERR_OUTOFVIDEOMEMORY) MessageBox(NULL, L"Direct3D out of memory", L"Error!", MB_ICONERROR);
		return 0;
	}

	return 1;
}




//Setup Direct3D settings
bool SetupDirect3D()
{
	//World setup
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	g_d9device->SetTransform(D3DTS_WORLD, &world);


	//Projection setup
	D3DXMATRIX projection;
	D3DXMatrixPerspectiveFovLH(&projection, D3DXToRadian(60), (float)4 / 3, 1, 1000);
	g_d9device->SetTransform(D3DTS_PROJECTION, &projection);


	//View setup
	D3DXMATRIX view;
	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0, 0, -5), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
	g_d9device->SetTransform(D3DTS_VIEW, &view);


	//Set render properties
	g_d9device->SetRenderState(D3DRS_LIGHTING, true);
	g_d9device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(100, 100, 100));
	g_d9device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DCOLOR_XRGB(50, 50, 50));
	g_d9device->SetRenderState(D3DRS_SPECULARENABLE, true);
	g_d9device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_d9device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_d9device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);


	//Set lighting
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set the light's color
	light.Direction = D3DXVECTOR3(-1.0f, -0.3f, -1.0f);
	g_d9device->SetLight(0, &light);
	g_d9device->LightEnable(0, TRUE);


	//Set material
	D3DMATERIAL9 material;
	ZeroMemory(&material, sizeof(D3DMATERIAL9));    // clear out the struct for use
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set diffuse color to white
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set ambient color to white
	g_d9device->SetMaterial(&material);    // set the globably-used material to &material


										   //Load textures
										   /*if (fbxmodel.getTextureName() != "")
										   {
										   string InFileName = fbxmodel.getTextureName();
										   wstring OutFileName(InFileName.length(), L' ');
										   std::copy(InFileName.begin(), InFileName.end(), OutFileName.begin());

										   D3DXCreateTextureFromFile(g_d9device, OutFileName.c_str(), &g_Texture);
										   if (g_Texture == NULL)
										   {
										   wstring msg = L"Unable to load texture - " + OutFileName;
										   MessageBox(NULL, msg.c_str(), L"Error!", MB_ICONERROR);
										   return 0;
										   }
										   g_d9device->SetTexture(0, g_Texture);
										   g_d9device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
										   g_d9device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
										   g_d9device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
										   g_d9device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
										   g_d9device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
										   g_d9device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
										   g_d9device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
										   }


										   //Vertex buffer setup
										   g_d9device->CreateVertexBuffer(fbxmodel.getVertexArrayAtTime(50).size() * sizeof(D3DVERTEX), 0, D3DFVF_VERTEXPARAMS, D3DPOOL_DEFAULT, &g_d9verbuf, NULL);
										   if (g_d9verbuf == NULL)
										   {
										   MessageBox(NULL, L"Direct3D vertex buffer init error", L"Error!", MB_ICONERROR);
										   return 0;
										   }*/


										   //Camera setup
	g_camera.px = 0;
	g_camera.py = 120;
	g_camera.pz = -40;
	g_camera.rx = 0;
	g_camera.ry = D3DXToRadian(180);
	g_camera.rz = 0;
	g_camera.pspeed = 2;
	g_camera.rspeed = 1;

	return 1;
}




//Prepare buffer
/*bool PrepareData()
{
D3DVERTEX* d9VData;

g_d9verbuf->Lock(NULL, NULL, (void **)&d9VData, NULL);

vector<FBXVertex> *verarray = &fbxmodel.getVertexArrayAtTime(g_tick);
int size = verarray->size();

for (int i = 0; i < size; i++)
{
d9VData[i].position = D3DXVECTOR3((*verarray)[i].pos[0], (*verarray)[i].pos[1], (*verarray)[i].pos[2]);
d9VData[i].normal = D3DXVECTOR3((*verarray)[i].norm[0], (*verarray)[i].norm[1], (*verarray)[i].norm[2]);
d9VData[i].tu = (*verarray)[i].tu;
d9VData[i].tv = -(*verarray)[i].tv;
d9VData[i].color = (*verarray)[i].color;
}

g_d9verbuf->Unlock();

return 1;
}*/




//Render func
void Render()
{
	//Setup matrix
	D3DXMATRIX matTranslate;
	D3DXMATRIX matRotateX;
	D3DXMATRIX matRotateY;
	D3DXMATRIX matRotateZ;
	D3DXMatrixTranslation(&matTranslate, g_camera.px, g_camera.py, g_camera.pz);
	D3DXMatrixRotationX(&matRotateX, g_camera.rx - D3DXToRadian(90));
	D3DXMatrixRotationY(&matRotateY, g_camera.ry);
	D3DXMatrixRotationZ(&matRotateZ, g_camera.rz);
	D3DXMATRIX view = matTranslate * matRotateX * matRotateY * matRotateZ;
	g_d9device->SetTransform(D3DTS_VIEW, &view);




	//Clear buffers
	g_d9device->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 0, 255), 1.0f, 0);

	g_d9device->BeginScene();

	//PrepareData();
	g_d9device->SetStreamSource(0, g_d9verbuf, 0, sizeof(D3DVERTEX));
	g_d9device->SetFVF(D3DFVF_VERTEXPARAMS);
	//g_d9device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, fbxmodel.getVerticiesCount());

	g_d9device->EndScene();


	g_d9device->Present(NULL, NULL, NULL, NULL);
}




//Main loop of application
void MainLoop()
{
	//Get tick count
	DWORD begin_tick = GetTickCount();




	//Keyboard process
	if (g_keyboard['W'])
	{
		g_camera.px += g_camera.pspeed * sin(g_camera.ry);
		g_camera.py += g_camera.pspeed * cos(g_camera.ry);
	}
	if (g_keyboard['S'])
	{
		g_camera.px -= g_camera.pspeed * sin(g_camera.ry);
		g_camera.py -= g_camera.pspeed * cos(g_camera.ry);
	}
	if (g_keyboard['A'])
	{
		g_camera.px += g_camera.pspeed * sin(g_camera.ry + D3DXToRadian(90));
		g_camera.py += g_camera.pspeed * cos(g_camera.ry + D3DXToRadian(90));
	}
	if (g_keyboard['D'])
	{
		g_camera.px += g_camera.pspeed * sin(g_camera.ry - D3DXToRadian(90));
		g_camera.py += g_camera.pspeed * cos(g_camera.ry - D3DXToRadian(90));
	}
	if (g_keyboard[32])
	{
		g_camera.pz -= g_camera.pspeed;
	}
	if (g_keyboard[16])
	{
		g_camera.pz += g_camera.pspeed;
	}
	if (g_keyboard[27])
	{
		exit(0);
	}




	//Mouse process
	POINT p;
	GetCursorPos(&p);
	g_camera.ry += D3DXToRadian(400 - p.x);
	//g_camera.rx += D3DXToRadian(300 - p.y);
	//g_camera.rz += D3DXToRadian(300 - p.y);// * cos(g_camera.ry);
	//SetCursorPos(400, 300);




	//Redrer
	Render();




	//Get tick count
	DWORD end_tick = GetTickCount();
	int sleep_time = (1000 / MAX_FPS) - (end_tick - begin_tick);
	if (sleep_time > 0) Sleep(sleep_time);
}




//Release
void Release()
{
	//g_Texture->Release();
	//g_d9verbuf->Release();
	g_d9device->Release();
	g_d9->Release();
}




//Main func
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//Init window
	if (InitWindow(hInstance, nCmdShow) == 0)
	{
		MessageBox(NULL, L"Window init error", L"Error!", MB_ICONERROR);
		return 0;
	}


	//Init Direct3D
	if (InitDirect3D() == 0) return 0;


	//Load model


	//Setup direct 3D
	if (SetupDirect3D() == 0) return 0;


	//Main message loop
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//If no messages go to loop
			MainLoop();
		}
	}


	//Release Direct3D interfaces
	Release();


	return (int)msg.wParam;
}




//регистрация класса и создание окна
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Регистрация класса
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = (HBRUSH)(WHITE_PEN);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"FBXPlayerWindowClass";
	wc.lpszMenuName = NULL;

	if (!RegisterClassEx(&wc)) return 0;

	// Создание окна
	g_hWnd = CreateWindow(wc.lpszClassName,
		L"Direct3D9",
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		800,
		600,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (!g_hWnd) return 0;

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	//ShowCursor(false);
	//SetCursorPos(400, 300);

	return 1;
}




//Обработка сообщений окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		g_keyboard[wParam] = 1;
		break;

	case WM_KEYUP:
		g_keyboard[wParam] = 0;
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}




