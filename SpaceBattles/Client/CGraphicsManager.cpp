#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <string>

#include "CGraphicsManager.h"




//Struct of D3DVertex
struct D3DVERTEX
{
	D3DXVECTOR3 position;	// The position
	DWORD color;			// The color
	//FLOAT tu, tv;   // The texture coordinates
};
#define D3DFVF_VERTEXPARAMS (D3DFVF_XYZ | D3DFVF_DIFFUSE)
//#define D3DFVF_VERTEXPARAMS (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE)




//Struct camera
struct Camera
{
	float px, py, pz;
	float pspeed;
};




IDirect3D9 *g_d9 = NULL;
IDirect3DDevice9 *g_d9device = NULL;
IDirect3DVertexBuffer9 *g_d9verbuf = NULL;
IDirect3DTexture9 *g_Texture = NULL;
LPD3DXFONT g_font;
Camera g_camera;


std::vector<D3DVERTEX> g_VertexBuffer;
int g_TriangleCount = 0;

int g_FPSCount = 0;








//Calculate FPS
void CalculateFPS()
{
	static float framesPerSecond = 0.0f;
	static float lastTime = 0.0f;

	float currentTime = GetTickCount() * 0.001f;

	++framesPerSecond;
	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		g_FPSCount = framesPerSecond;
		framesPerSecond = 0;
	}
}




//Paint text
void PaintText(std::string _text, int _x1, int _y1, int _x2, int _y2, D3DCOLOR _color)
{
	RECT rec;

	rec.left = _x1;
	rec.top = _y1;
	rec.right = _x2;
	rec.bottom = _y2;

	g_font->DrawTextA(0, _text.c_str(), -1, &rec, DT_WORDBREAK, _color);
}








// Graphics manager instance
CGraphicsManager* CGraphicsManager::mGraphicsManager = nullptr;




// Get instance
CGraphicsManager* CGraphicsManager::getInstance()
{
	if (!mGraphicsManager)
		mGraphicsManager = new CGraphicsManager();
	return mGraphicsManager;
}




// Constructor
CGraphicsManager::CGraphicsManager()
{
}




// Graphics init
bool CGraphicsManager::Init(HWND _hWnd)
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

	HRESULT d3er = g_d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpparams, &g_d9device);
	if (g_d9device == NULL)
	{
		if (d3er == D3DERR_DEVICELOST) MessageBox(NULL, L"Direct3D device lost", L"Error!", MB_ICONERROR);
		if (d3er == D3DERR_INVALIDCALL) MessageBox(NULL, L"Direct3D invalid call", L"Error!", MB_ICONERROR);
		if (d3er == D3DERR_NOTAVAILABLE) MessageBox(NULL, L"Direct3D not available", L"Error!", MB_ICONERROR);
		if (d3er == D3DERR_OUTOFVIDEOMEMORY) MessageBox(NULL, L"Direct3D out of memory", L"Error!", MB_ICONERROR);
		return 0;
	}

	//Initially no data to update
	mUpdated = false;

	return 1;
}




// Reset graphics settings
bool CGraphicsManager::Reset()
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
	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0, 0,- 5), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
	g_d9device->SetTransform(D3DTS_VIEW, &view);


	//Set render properties
	g_d9device->SetRenderState(D3DRS_LIGHTING, false);
	//g_d9device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(100, 100, 100));
	//g_d9device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DCOLOR_XRGB(50, 50, 50));
	//g_d9device->SetRenderState(D3DRS_SPECULARENABLE, true);
	//g_d9device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	//g_d9device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//g_d9device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);


	//Init font
	//g_font = CreateFont(30, 10, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 1, 0, 0, 0, DEFAULT_PITCH | FF_MODERN, L"Arial");
	D3DXCreateFont(g_d9device, 30, 10, 1, 0, FALSE, 0, 30, 0, 0, L"Arial", &g_font);


	//Set lighting
	/*D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set the light's color
	light.Direction = D3DXVECTOR3(-1.0f, -0.3f, -1.0f);
	g_d9device->SetLight(0, &light);
	g_d9device->LightEnable(0, TRUE);*/


	//Set material
	/*D3DMATERIAL9 material;
	ZeroMemory(&material, sizeof(D3DMATERIAL9));    // clear out the struct for use
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set diffuse color to white
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set ambient color to white
	g_d9device->SetMaterial(&material);    // set the globably-used material to &material*/

	//Vertex buffer setup
	g_d9device->CreateVertexBuffer(1024 * sizeof(D3DVERTEX), 0, D3DFVF_VERTEXPARAMS, D3DPOOL_DEFAULT, &g_d9verbuf, NULL);
	if (g_d9verbuf == NULL)
	{
		MessageBox(NULL, L"Direct3D vertex buffer init error", L"Error!", MB_ICONERROR);
		return 0;
	}

    //Camera setup
	g_camera.px = 0;
	g_camera.py = 0;
	g_camera.pz = 0;
	g_camera.pspeed = 2;

	return 1;
}




// Add data
void CGraphicsManager::AddData(float x, float y, int size)
{
	if (mUpdated == true) return;
	D3DVERTEX vertex1, vertex2, vertex3, vertex4;
	vertex1.position[0] = x + (float)size / 2;
	vertex1.position[1] = y - (float)size / 2;
	vertex1.position[2] = 0;
	vertex1.color = 0xFFFFFF;

	vertex2.position[0] = x - (float)size / 2;
	vertex2.position[1] = y - (float)size / 2;
	vertex2.position[2] = 0;
	vertex2.color = 0xFFFFFF;

	vertex3.position[0] = x - (float)size / 2;
	vertex3.position[1] = y + (float)size / 2;
	vertex3.position[2] = 0;
	vertex3.color = 0xFFFFFF;

	vertex4.position[0] = x + (float)size / 2;
	vertex4.position[1] = y + (float)size / 2;
	vertex4.position[2] = 0;
	vertex4.color = 0xFFFFFF;

	g_VertexBuffer.push_back(vertex1);
	g_VertexBuffer.push_back(vertex2);
	g_VertexBuffer.push_back(vertex3);
	g_VertexBuffer.push_back(vertex1);
	g_VertexBuffer.push_back(vertex3);
	g_VertexBuffer.push_back(vertex4);
}




// Update buffer
void CGraphicsManager::UpdateBuffer()
{
	//Wait until updating is done
	mUpdated = true;
}




// Update data
void CGraphicsManager::UpdateData()
{
	//If there is no data yet - draw old data
	if (mUpdated == false) return;

	D3DVERTEX* d9VData;
	
	g_d9verbuf->Lock(NULL, NULL, (void **)&d9VData, NULL);

	g_TriangleCount = g_VertexBuffer.size() / 2;
	for (int i = 0; i < g_VertexBuffer.size(); i++)
	{
		d9VData[i].position = D3DXVECTOR3(g_VertexBuffer[i].position[0], g_VertexBuffer[i].position[1], g_VertexBuffer[i].position[2]);
		//d9VData[i].tu = g_VertexBuffer[i].tu;
		//d9VData[i].tv = -g_VertexBuffer[i].tv;
		d9VData[i].color = g_VertexBuffer[i].color;
	}
	g_VertexBuffer.clear();
	mUpdated = false;

	g_d9verbuf->Unlock();
}




// Render graphics
void CGraphicsManager::Render()
{
	//Calculate FPS
	CalculateFPS();

	//Update data for drawing
	this->UpdateData();

	//Setup matrix
	//D3DXMATRIX matTranslate;
	//D3DXMatrixTranslation(&matTranslate, g_camera.px, g_camera.py, g_camera.pz);
	//D3DXMATRIX view = matTranslate;
	//g_d9device->SetTransform(D3DTS_VIEW, &view);


	//Clear buffers
	g_d9device->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	g_d9device->BeginScene();

	g_d9device->SetStreamSource(0, g_d9verbuf, 0, sizeof(D3DVERTEX));
	g_d9device->SetFVF(D3DFVF_VERTEXPARAMS);
	g_d9device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, g_TriangleCount);

	PaintText("FPS: " + std::to_string(g_FPSCount), 10, 10, 300, 50, D3DCOLOR_ARGB(250, 250, 250, 350));

	g_d9device->EndScene();


	g_d9device->Present(NULL, NULL, NULL, NULL);
}




// Graphics release
void CGraphicsManager::Release()
{
	g_d9device->Release();
	g_d9->Release();
}
