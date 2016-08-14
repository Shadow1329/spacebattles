#pragma once
#include <windows.h>




//Graphics manager
class CGraphicsManager
{
private:
	static CGraphicsManager *mGraphicsManager;		// Instance
	bool mUpdated;									// Data updated


	// Update data for drawing												
	void UpdateData();




public:
	// Instance
	static CGraphicsManager* getInstance();

	// Constructor && Destructor
	CGraphicsManager();
	~CGraphicsManager() {};

	// Init graphics
	bool Init(HWND _hWnd);

	// Reset graphics
	bool Reset();

	// Add data
	void AddData(float x, float y, int size);

	// Update buffer
	void UpdateBuffer();

	// Render graphics
	void Render();

	// Release graphics
	void Release();
};

