
#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include "Logic.h"

#pragma comment (lib, "d2d1.lib")
#pragma comment (lib,"dwrite.lib")

#define I_color  0x008080
#define O_color  0xccc472
#define J_color  0x468499
#define L_color  0x7e997b
#define S_color  0x579951
#define Z_color  0xc03919
#define T_color  0xb05ea8

class Graphics
{
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* renderTarget;
	ID2D1SolidColorBrush* brush;

	// for writing
	IDWriteFactory* writeFactory;
	IDWriteTextFormat* textFormat;

	wchar_t* text;	// text string
	UINT32 length;	// length of string


public:
	Graphics();
	~Graphics();

	bool Init(HWND windowHandle);
	void Draw_Text(HWND windowHandle);

	void BeginDraw()
	{
		renderTarget->BeginDraw();
	}

	void EndDraw()
	{
		renderTarget->EndDraw();
	}

	void Render();
	void DrawBox(float x, float y, float height, float width, UINT32 color, float a, float strkWdh, bool pd);
	void ClearScreen();
	void DrawBucket(float x, float y, float a);
	void DrawHintIndicator(float x, float y, float a, int r0, Logic &object);
	void DrawGrid();
	void Draw_Tetrominos(float x, float y, UINT32 color);
	void Draw_Numbers(int score);
	void Num(int digit, int i, int pos);
	void Highscore(int highScore);
	void You_Lose();


/*	Tetris	 Colors		r
	I		 0x40E0D0	0		
	O		 0xFFF68F	1
	J		 0x0047AB	2
	L		 0xE49B0F	3
	S		 0xADFF2F	4
	Z		 0xFF0000	5
	T		 0xF73C8B	6
*/
};