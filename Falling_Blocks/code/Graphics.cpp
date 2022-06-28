#include "Graphics.h"
#include "Logic.h"

#include <string.h>

Graphics::Graphics()
{
	factory = NULL;
	renderTarget = NULL;
	brush = NULL;

	writeFactory = NULL;
	textFormat = NULL;
}

Graphics::~Graphics()
{
	if (factory)
	{
		factory->Release();
	}
	if (renderTarget)
	{
		renderTarget->Release();
	}
	if (brush)
	{
		brush->Release();
	}
	if (writeFactory)
	{
		writeFactory->Release();
	}
	if (textFormat)
	{
		textFormat->Release();
	}
}

bool Graphics::Init(HWND windowHandle)
{
	// Create Direct2D factory
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (res != S_OK)
	{
		return false;
	}

	RECT rect;
	GetClientRect(windowHandle, &rect);

	// Used that factory to Create the render target and set it to the client are on our window
	res = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(windowHandle, D2D1::SizeU(rect.right, rect.bottom)),
		&renderTarget);
	if (res != S_OK)
	{
		return false;
	}

	res = renderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 1.0f), &brush);
	if (res != S_OK)
	{
		return false;
	}

	// create IDwriteFactory interface, it's root to all Dwrite objects
	res = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&writeFactory));
	if (res != S_OK)
	{
		return false;
	}

	// create IDwriteTextFormat interface object
	res = writeFactory->CreateTextFormat(L"Angel", NULL,
		DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		20.0F, L"en-us", &textFormat);
	if (res != S_OK)
	{
		return false;
	}

	// Right align text
	res = textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	if (res != S_OK)
	{
		return false;
	}

	return true;
}

void Graphics::ClearScreen()
{
	renderTarget->Clear(D2D1::ColorF(0x151515));
}


void Graphics::DrawBucket(float x, float y,float a)
{
	int height = 475;
	int width = 25;
	UINT32 color = 0x696969;
	float strokeWidth = 3.0f;
	DrawBox(x, y, height, width, color, a, strokeWidth, false);	// left
	x = x + 275;
	DrawBox(x, y, height, width, color, a, strokeWidth, false);	// right
	height = 25;
	width = 250;
	x = x - 250;
	y = y + 450;
	DrawBox(x, y, height, width, color, a, strokeWidth, false);	// bottom

}

void Graphics::DrawHintIndicator(float x, float y, float a, int r0, Logic &l_object)
{
	int height = 125;
	int x_offset = 525;
	int y_offset = 100;
	UINT32 color = 0xFF6347;
	float strokeWidth = 3.0f;
	DrawBox(x, y, height, height, color, a, strokeWidth, false);

	switch (r0)
	{
	case 0:
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				if (l_object.I.shape[x][y] != 0)
				{
					Draw_Tetrominos((x * 25) + x_offset + 25, (y * 25) + y_offset - 12, I_color);
				}
			}
		}
		break;
	case 1:
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				if (l_object.O.shape[x][y] != 0)
				{
					Draw_Tetrominos((x * 25) + x_offset + 12, (y * 25) + y_offset + 12, O_color);
				}
			}
		}
		break;
	case 2:
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				if (l_object.J.shape[x][y] != 0)
				{
					Draw_Tetrominos((x * 25) + x_offset + 12, (y * 25) + y_offset - 2, J_color);
				}
			}
		}
		break;
	case 3:
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				if (l_object.L.shape[x][y] != 0)
				{
					Draw_Tetrominos((x * 25) + x_offset + 15, (y * 25) + y_offset - 2, L_color);
				}
			}
		}
		break;
	case 4:
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				if (l_object.S.shape[x][y] != 0)
				{
					Draw_Tetrominos((x * 25) + x_offset, (y * 25) + y_offset - 12, S_color);
				}
			}
		}
		break;
	case 5:
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				if (l_object.Z.shape[x][y] != 0)
				{
					Draw_Tetrominos((x * 25) + x_offset, (y * 25) + y_offset - 12, Z_color);
				}
			}
		}
		break;
	case 6:
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				if (l_object.T.shape[x][y] != 0)
				{
					Draw_Tetrominos((x * 25) + x_offset, (y * 25) + y_offset + 15 - 5, T_color);
				}
			}
		}
		break;
	default:
		break;
	}


}

void Graphics::DrawGrid()
{
	brush->SetColor(D2D1::ColorF(0xFFFFFF, 0.5f));

//	D2D1_SIZE_F rtSize = renderTarget->GetSize();

	int X = 175;
	int Y = 50;
	int height = 500;
	int width = 250;

	for (int x = X; x < X + width; x += 25)
	{
		renderTarget->DrawLine(D2D1::Point2F(x, Y), D2D1::Point2F(x, height), brush, 0.5f);			// vertical lines
	}

	for (int y = Y; y < Y + height; y += 25)
	{
		renderTarget->DrawLine(D2D1::Point2F(X, y), D2D1::Point2F(X + width, y), brush, 0.5f);		// horizontal lines
	}

}

void Graphics::DrawBox(float x, float y, float height, float width, UINT32 color, float a, float strkWdh, bool pd)
{
	brush->SetColor(D2D1::ColorF(color, a));
	renderTarget->DrawRectangle(D2D1::RectF(x, y, x + width, y + height), brush, strkWdh);
	if (pd)
	{
		brush->SetColor(D2D1::ColorF(0x696969));
		renderTarget->DrawLine(D2D1::Point2F(x + 10, y + 10), D2D1::Point2F(x + 9, y + 11), brush, strkWdh);
	}
}

void Graphics::Render()
{
//	BeginDraw();

//	ClearScreen();
	DrawGrid();
	DrawBucket(150, 50, 1.0f);
	renderTarget->SetTransform(D2D1::IdentityMatrix());
//	DrawHintIndicator(525, 75, 1.0f, r0);

//	EndDraw();
}


void Graphics::Draw_Tetrominos(float x, float y, UINT32 color)
{
	brush->SetColor(D2D1::ColorF(color, 1.0f));
	renderTarget->FillRectangle(D2D1::RectF(x, y, x + 25, y + 25), brush);
}

void Graphics::Draw_Text(HWND windowHandle)
{
	RECT rect;
	GetClientRect(windowHandle, &rect);

	// Initialize text string and store it's length
	text = L" High Score : ";
	length = (UINT32)wcslen(text);

	// create a Direct2D rectangle
	D2D1_RECT_F displayArea = D2D1::RectF(500, 300, 620, 300);

	brush->SetColor(D2D1::ColorF(0xFFFFFF, 0.5f));

	// Render Text to screen
	renderTarget->DrawTextW(text, length, textFormat, displayArea, brush);

	D2D1_RECT_F displayArea2 = D2D1::RectF(500, 350, 620, 350);
	text = L"Score : ";
	length = (UINT32)wcslen(text);
	renderTarget->DrawTextW(text, length, textFormat, displayArea2, brush);
}

void Graphics::Draw_Numbers(int n)
{
//	int n = 11;
	int pos = 350;
	int i = 0;
	int digit = 0;
	while (n > 0)		// reverse
	{
		digit = digit * 10;
		digit = digit + n % 10;
		n = n / 10;
	}

	n = digit;
	digit = 0;

	while (n > 0)		// seperate digits
	{
		digit = n % 10;
		n = n / 10;
		// call funtions from here
		Num(digit, i, pos);
		i++;
	}

	Num(0, i, pos);
//	Num(0, i + 1);
	
}

void Graphics::Num(int digit, int i, int pos)
{
	brush->SetColor(D2D1::ColorF(0xFFFFFF, 0.5f));
	int s = 12;	// space between numbers
	wchar_t *num = L"0";
	length = (UINT32)wcslen(num);

	int x = 640, y = 640;
	
	switch (digit)
	{
	case 0: num = L"0";
		break;
	case 1: num = L"1";
		break;
	case 2: num = L"2";
		break;
	case 3: num = L"3";
		break;
	case 4: num = L"4";
		break;
	case 5: num = L"5";
		break;
	case 6: num = L"6";
		break;
	case 7: num = L"7";
		break;
	case 8: num = L"8";
		break;
	case 9: num = L"9";
		break;
	default:
		break;
	}

	D2D1_RECT_F score = D2D1::RectF(x + s * i, pos, y + s * i, pos);
	renderTarget->DrawTextW(num, length, textFormat, score, brush);
}

void Graphics::Highscore(int highScore)
{
	int i = 0;
	int digit = 0;
	while (highScore > 0)		// reverse
	{
		digit = digit * 10;
		digit = digit + highScore % 10;
		highScore = highScore / 10;
	}

	highScore = digit;
	digit = 0;

	while (highScore > 0)		// seperate digits
	{
		digit = highScore % 10;
		highScore = highScore / 10;
		Num(digit, i, 300);
		i++;
	}

	Num(0, i, 300);
}

void Graphics::You_Lose()
{
	text = L"You Lose !! \n Press 'R' to Restart.";
	length = (UINT32)wcslen(text);
	brush->SetColor(D2D1::ColorF(0xff2517, 0.5f));
	D2D1_RECT_F LoseScreen = D2D1::RectF(450, 425, 660, 425);
	renderTarget->DrawTextW(text, length, textFormat, LoseScreen, brush);
}