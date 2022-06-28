#include <Windows.h>
#include "Graphics.h"
#include "Logic.h"
#include <time.h>
#include <iostream>
#include <fstream>

Graphics* graphics;
Logic* logic;
DWORD start_time;

int r0;
int r;
bool landed = false;
bool left = false;
bool right = false;
bool paused = false;
bool restart = false;
int rota = 0;	// rotation manager
int score = 0;
int highScore = 0;
int speed = 700;
int level = 1;
int lose = 0;
bool drop = false;

///
///		IMP TASK : (1) ORGANIZE YOUR TETROMINOS							(poorely done)
///				   (2) GENERATE TETEROMINOS RANDOMELY					(done)
///				   (3) optional : get rid of tetromino when it lands	(done, barbarian style)
///				   (4) Clearing Lines 

void GameLoop();
void Fill_Grid();
void Fill_Block();
void Falling();
void Moving();
void Update_Bucket();
void Draw_Another();
void Set_Values();
void Select_rand();
void Rotation(); 
void Shapes(int rota, int r);
void Init_temp_shape();
void Clear_Lines();
void SpeedIncrease();
void Update_Highscore();

void GameLoop()
{
	if (!paused)
	{
		if (GetTickCount() - start_time > speed)		// think about how to stack and use all the tetrominos.... IMP!!!!!
		{
			Falling();
			start_time = GetTickCount();
		}
	}
}

void Fill_Grid()
{
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 18; y++)
		{
			if (logic->Bucket[x][y] != 0)
			{
				graphics->DrawBox(x*(25) + 175, y*(25) + 50, 25, 25, 0x229cdc, 1.0, 3.0f, true);		// 0x229cdc
			}
		}
	}
}

void Fill_Block()		// new
{
	UINT32 color;

	switch (r)
	{
	case 0: color = I_color;
		break;
	case 1: color = O_color;
		break;
	case 2: color = J_color;
		break;
	case 3: color = L_color;
		break;
	case 4: color = S_color;
		break;
	case 5: color = Z_color;
		break;
	case 6: color = T_color;
		break;
	default:
		break;
	}

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			if (logic->temp.shape[x][y] != 0)
			{
				graphics->Draw_Tetrominos(x*(25) + logic->temp.topleft_x, y*(25) + logic->temp.topleft_y, color);
			}
		}
	}
}

void Moving()
{
	// moving right 
	if (right)
	{
		logic->temp.new_topleft_x++;
	}
	// and left
	if (left)
	{
		logic->temp.new_topleft_x--;
	}
	bool check = true;
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			if (logic->temp.shape[x][y] != 0)
			{
				if (logic->Bucket[x + logic->temp.new_topleft_x][y + logic->temp.new_topleft_y] != 0)
				{
					check = false;	// space taken
				}
				else if (logic->temp.new_topleft_x > 9)
				{
					check = false;	// too right
				}
				else if (logic->temp.new_topleft_x < -1)	// lookout for this -1
				{
					check = false;	// too left
				}
			}
		}
	}

	if (check && right)
	{
		logic->temp.topleft_x += 25;
	}
	if (check  && left)
	{
		logic->temp.topleft_x -= 25;
	}
	if (!check  && right)
	{
		logic->temp.new_topleft_x--;
	}
	if (!check && left)
	{
		logic->temp.new_topleft_x++;
	}
	right = false;
	left = false;

	Set_Values();
}

void Falling()		// new
{
	Set_Values();

	int i = 0;

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			if (logic->temp.shape[x][y] != 0)
			{
				if (y + logic->temp.new_topleft_y > 17)		// find a way to get height of bucket
				{
					landed = true;
				}
				else if (logic->Bucket[x + logic->temp.new_topleft_x][y + logic->temp.new_topleft_y] == 0)
				{
					i++;
				}
				else if (logic->Bucket[x + logic->temp.new_topleft_x][y + logic->temp.new_topleft_y] != 0)
				{
					landed = true;
				}
			}
		}
	}

	if ( i == 4)
	{
		logic->temp.topleft_y += 25;	
		logic->temp.new_topleft_y++;
	//	logic->I = logic->temp;		// and randomize his too	// this should be at the end of every call, in a seperate function
	}

	Set_Values();

	if (landed)
	{
		rota = 0;
		Update_Bucket();
		logic->O_tetromino();		// Initialization and selecting next tetromino goes here, unless you find another way
		logic->I_tetromino();
		logic->J_tetromino();
		logic->L_tetromino();
		logic->S_tetromino();
		logic->Z_tetromino();
		logic->T_tetromino();
	}
}

void Rotation()	// Need to fixthis function, the rotation will work
{
	bool checK = true;

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			if (logic->temp.next_shape[x][y] != 0)
			{
				if (logic->temp.topleft_x < 25 * 7)
				{
					checK = false;		// too left
				}
				if (logic->temp.topleft_x > 25 * 13)
				{
					checK = false;		// too right
				}
				if (logic->temp.topleft_y > 25 * 16)		// too low
				{
					checK = false;
				}
				if (logic->Bucket[x + (logic->temp.topleft_x - 175) / 25][y + (logic->temp.topleft_y - 50) / 25] != 0)		// space taken
				{
					checK = false;
				}
			}
		}
	}

	if (checK)
	{
		Shapes(rota, r);
	}
	
}

void Shapes(int rota, int r)
{
	Init_temp_shape();
	// one spin clock-wise
	if (rota == 1)
	{
		// I
		if (r == 0)
		{
			logic->temp.shape[0][1] = logic->temp.shape[1][1] = logic->temp.shape[2][1] = logic->temp.shape[3][1] = 1;
			logic->temp.next_shape[1][0] = logic->temp.next_shape[1][1] = logic->temp.next_shape[1][2] = logic->temp.next_shape[1][3] = 1;
			Set_Values();
		}
		// O
		if (r == 1)
		{
			logic->temp.shape[1][0] = logic->temp.shape[2][0] = logic->temp.shape[1][1] = logic->temp.shape[2][1] = 1;
			logic->temp.next_shape[1][0] = logic->temp.next_shape[2][0] = logic->temp.next_shape[1][1] = logic->temp.next_shape[2][1] = 1;
			Set_Values();
		}
		// J
		else if (r == 2)
		{
			logic->temp.shape[1][0] = logic->temp.shape[1][1] = logic->temp.shape[2][1] = logic->temp.shape[3][1] = 1;
			logic->temp.next_shape[1][0] = logic->temp.next_shape[1][1] = logic->temp.next_shape[1][2] = logic->temp.next_shape[2][0] = 1;
			Set_Values();
		}
		// L
		else if (r == 3)
		{
			logic->temp.shape[1][1] = logic->temp.shape[1][2] = logic->temp.shape[2][1] = logic->temp.shape[3][1] = 1;
			logic->temp.next_shape[1][0] = logic->temp.next_shape[2][0] = logic->temp.next_shape[2][1] = logic->temp.next_shape[2][2] = 1;
			Set_Values();
		}
		// S
		else if (r == 4)
		{
			logic->temp.shape[1][0] = logic->temp.shape[1][1] = logic->temp.shape[2][1] = logic->temp.shape[2][2] = 1;
			logic->temp.next_shape[1][2] = logic->temp.next_shape[2][1] = logic->temp.next_shape[2][2] = logic->temp.next_shape[3][1] = 1;
			Set_Values();
		}
		// Z
		else if (r == 5)
		{
			logic->temp.shape[1][1] = logic->temp.shape[1][2] = logic->temp.shape[2][0] = logic->temp.shape[2][1] = 1;
			logic->temp.next_shape[1][1] = logic->temp.next_shape[2][1] = logic->temp.next_shape[2][2] = logic->temp.next_shape[3][2] = 1;
			Set_Values();
		}
		// T
		else if (r == 6)
		{
			logic->temp.shape[1][0] = logic->temp.shape[1][1] = logic->temp.shape[1][2] = logic->temp.shape[2][1] = 1;
			logic->temp.next_shape[1][1] = logic->temp.next_shape[2][1] = logic->temp.next_shape[3][1] = logic->temp.next_shape[2][2] = 1;
			Set_Values();
		}
	}

	// original
	else if (rota == 0)
	{
		// I
		if (r == 0)
		{
			logic->temp.shape[1][0] = logic->temp.shape[1][1] = logic->temp.shape[1][2] = logic->temp.shape[1][3] = 1;
			logic->temp.next_shape[0][1] = logic->temp.next_shape[1][1] = logic->temp.next_shape[2][1] = logic->temp.next_shape[3][1] = 1;
			Set_Values();
		}
		// O
		if (r == 1)
		{
			logic->temp.shape[1][0] = logic->temp.shape[2][0] = logic->temp.shape[1][1] = logic->temp.shape[2][1] = 1;
			logic->temp.next_shape[1][0] = logic->temp.next_shape[2][0] = logic->temp.next_shape[1][1] = logic->temp.next_shape[2][1] = 1;
			Set_Values();
		}
		// J
		else if (r == 2)
		{
			logic->temp.shape[2][0] = logic->temp.shape[2][1] = logic->temp.shape[2][2] = logic->temp.shape[1][2] = 1;
			logic->temp.next_shape[1][0] = logic->temp.next_shape[1][1] = logic->temp.next_shape[2][1] = logic->temp.next_shape[3][1] = 1;
			Set_Values();
		}
		// L
		else if (r == 3)
		{
			logic->temp.shape[1][0] = logic->temp.shape[1][1] = logic->temp.shape[1][2] = logic->temp.shape[2][2] = 1;
			logic->temp.next_shape[1][1] = logic->temp.next_shape[1][2] = logic->temp.next_shape[2][1] = logic->temp.next_shape[3][1] = 1;
			Set_Values();
		}
		// S
		else if (r == 4)
		{
			logic->temp.shape[1][2] = logic->temp.shape[2][1] = logic->temp.shape[2][2] = logic->temp.shape[3][1] = 1;
			logic->temp.next_shape[1][0] = logic->temp.next_shape[1][1] = logic->temp.next_shape[2][1] = logic->temp.next_shape[2][2] = 1;

			Set_Values();
		}
		// Z
		else if (r == 5)
		{
			logic->temp.shape[1][1] = logic->temp.shape[2][1] = logic->temp.shape[2][2] = logic->temp.shape[3][2] = 1;
			logic->temp.next_shape[1][1] = logic->temp.next_shape[1][2] = logic->temp.next_shape[2][0] = logic->temp.next_shape[2][1] = 1;
			Set_Values();
		}
		// T
		else if (r == 6)
		{
			logic->temp.shape[1][1] = logic->temp.shape[2][0] = logic->temp.shape[2][1] = logic->temp.shape[3][1] = 1;
			logic->temp.next_shape[1][0] = logic->temp.next_shape[1][1] = logic->temp.next_shape[1][2] = logic->temp.next_shape[2][1] = 1;
			Set_Values();
		}
	}

	// inverted
	else if (rota == 2)
	{
		// I
		if (r == 0)
		{
			logic->temp.shape[1][0] = logic->temp.shape[1][1] = logic->temp.shape[1][2] = logic->temp.shape[1][3] = 1;
			logic->temp.next_shape[0][1] = logic->temp.next_shape[1][1] = logic->temp.next_shape[2][1] = logic->temp.next_shape[3][1] = 1;
			Set_Values();
		}
		// O
		if (r == 1)
		{
			logic->temp.shape[1][0] = logic->temp.shape[2][0] = logic->temp.shape[1][1] = logic->temp.shape[2][1] = 1;
			logic->temp.next_shape[1][0] = logic->temp.next_shape[2][0] = logic->temp.next_shape[1][1] = logic->temp.next_shape[2][1] = 1;
			Set_Values();
		}
		// J
		else if (r == 2)
		{
			logic->temp.shape[1][0] = logic->temp.shape[1][1] = logic->temp.shape[1][2] = logic->temp.shape[2][0] = 1;
			logic->temp.next_shape[1][1] = logic->temp.next_shape[2][1] = logic->temp.next_shape[3][1] = logic->temp.next_shape[3][2] = 1;
			Set_Values();
		}
		// L
		else if (r == 3)
		{
			logic->temp.shape[1][0] = logic->temp.shape[2][0] = logic->temp.shape[2][1] = logic->temp.shape[2][2] = 1;
			logic->temp.next_shape[1][1] = logic->temp.next_shape[2][1] = logic->temp.next_shape[3][1] = logic->temp.next_shape[3][0] = 1;
			Set_Values();
		}
		// S
		else if (r == 4)
		{
			logic->temp.shape[1][2] = logic->temp.shape[2][1] = logic->temp.shape[2][2] = logic->temp.shape[3][1] = 1;
			logic->temp.next_shape[1][0] = logic->temp.next_shape[1][1] = logic->temp.next_shape[2][1] = logic->temp.next_shape[2][2] = 1;
			Set_Values();
		}
		// Z
		else if (r == 5)
		{
			logic->temp.shape[1][1] = logic->temp.shape[2][1] = logic->temp.shape[2][2] = logic->temp.shape[3][2] = 1;
			logic->temp.next_shape[1][1] = logic->temp.next_shape[1][2] = logic->temp.next_shape[2][0] = logic->temp.next_shape[2][1] = 1;
			Set_Values();
		}
		// T
		else if (r == 6)
		{
			logic->temp.shape[1][1] = logic->temp.shape[2][1] = logic->temp.shape[3][1] = logic->temp.shape[2][2] = 1;
			logic->temp.next_shape[1][1] = logic->temp.next_shape[2][0] = logic->temp.next_shape[2][1] = logic->temp.next_shape[2][2] = 1;
			Set_Values();
		}
	}

	// one spin anti clock-wise
	else if (rota == 3)
	{
		// I
		if (r == 0)
		{
			logic->temp.shape[0][1] = logic->temp.shape[1][1] = logic->temp.shape[2][1] = logic->temp.shape[3][1] = 1;
			logic->temp.next_shape[1][0] = logic->temp.next_shape[1][1] = logic->temp.next_shape[1][2] = logic->temp.next_shape[1][3] = 1;
			Set_Values();
		}
		// O
		if (r == 1)
		{
			logic->temp.shape[1][0] = logic->temp.shape[2][0] = logic->temp.shape[1][1] = logic->temp.shape[2][1] = 1;
			logic->temp.next_shape[1][0] = logic->temp.next_shape[2][0] = logic->temp.next_shape[1][1] = logic->temp.next_shape[2][1] = 1;
			Set_Values();
		}
		// J
		else if (r == 2)
		{
			logic->temp.shape[1][1] = logic->temp.shape[2][1] = logic->temp.shape[3][1] = logic->temp.shape[3][2] = 1;
			logic->temp.next_shape[2][0] = logic->temp.next_shape[2][1] = logic->temp.next_shape[2][2] = logic->temp.next_shape[1][2] = 1;
			Set_Values();
		}
		// L
		else if (r == 3)
		{
			logic->temp.shape[1][1] = logic->temp.shape[2][1] = logic->temp.shape[3][1] = logic->temp.shape[3][0] = 1;
			logic->temp.next_shape[1][0] = logic->temp.next_shape[1][1] = logic->temp.next_shape[1][2] = logic->temp.next_shape[2][2] = 1;
			Set_Values();
		}
		// S
		else if (r == 4)
		{
			logic->temp.shape[1][0] = logic->temp.shape[1][1] = logic->temp.shape[2][1] = logic->temp.shape[2][2] = 1;
			logic->temp.next_shape[1][2] = logic->temp.next_shape[2][1] = logic->temp.next_shape[2][2] = logic->temp.next_shape[3][1] = 1;
			Set_Values();
		}
		// Z
		else if (r == 5)
		{
			logic->temp.shape[1][1] = logic->temp.shape[1][2] = logic->temp.shape[2][0] = logic->temp.shape[2][1] = 1;
			logic->temp.next_shape[1][1] = logic->temp.next_shape[2][1] = logic->temp.next_shape[2][2] = logic->temp.next_shape[3][2] = 1;
			Set_Values();
		}
		// T
		else if (r == 6)
		{
			logic->temp.shape[1][1] = logic->temp.shape[2][0] = logic->temp.shape[2][1] = logic->temp.shape[2][2] = 1;
			logic->temp.next_shape[1][1] = logic->temp.next_shape[2][0] = logic->temp.next_shape[2][1] = logic->temp.next_shape[3][1] = 1;
			Set_Values();
		}
	}
}

void Update_Bucket()
{
	bool bucket_filled = false;
	for (int x = 0; x < 10; x++)
	{
		if (logic->Bucket[x][0] != 0) { bucket_filled = true;   lose = 1; }			// resets the bucket, new game initialization from here
	}

	if (bucket_filled)
	{
		paused = true;
		if (restart)
		{
			logic->Init_Bucket();
		}
	}
	else
	{
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				if (logic->temp.shape[x][y] != 0)
				{
					logic->Bucket[x + (logic->temp.topleft_x - 175) / 25][y + (logic->temp.topleft_y - 50) / 25] = logic->temp.shape[x][y];		// 175 = x-offset, 50 = y-offfset
				}
			}
		}
	}
	restart = false;
	
}

void Select_rand()
{
	srand(time(NULL));
	//r = r0;
	if (landed)
	{
		r = r0;
		r0 = rand() % 7;
	}
	switch (r)
	{
	case 0: logic->temp = logic->I;
		break;
	case 1: logic->temp = logic->O;
		break;
	case 2: logic->temp = logic->J;
		break;
	case 3: logic->temp = logic->L;
		break;
	case 4: logic->temp = logic->S;
		break;
	case 5: logic->temp = logic->Z;
		break;
	case 6: logic->temp = logic->T;
		break;
	default:
		break;
	}
	landed = false;
}

void Set_Values()
{
	switch (r)
	{
	case 0: logic->I = logic->temp;
		break;
	case 1: logic->O = logic->temp;
		break;
	case 2: logic->J = logic->temp;
		break;
	case 3: logic->L = logic->temp;
		break;
	case 4: logic->S = logic->temp;
		break;
	case 5: logic->Z = logic->temp;
		break;
	case 6: logic->T = logic->temp;
		break;
	default:
		break;
	}
}

void Init_temp_shape()
{
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			logic->temp.shape[x][y] = 0;
		}
	}
}

void Clear_Lines()
{
	bool can_clear = true;
	int X = 0;
	for (int y = 0; y < 18; y++)
	{
		can_clear = true;
		for (int x = 0; x < 10; x++)
		{
			if (logic->Bucket[x][y] == 0) { can_clear = false; }
		}
		if (can_clear == true)
		{
			score = score + level;
			for (int Y = y; Y > 1; Y--)
			{
				for (int X = 0; X < 10; X++)
				{
					logic->Bucket[X][Y] = logic->Bucket[X][Y - 1];
				}
			}
			if (score % 10 == 0)
			{
				score++;
			}
		}
	}
}

void SpeedIncrease()
{
	if (score <= 5)
	{
		speed = 700; level = 1;
	}
	else if (score > 5 && score <= 20)
	{
		speed = 600; level = 2;
	}
	else if (score > 20 && score <= 50)
	{
		speed = 400; level = 3;
	}
	else if (score >50  && score <= 110)
	{
		speed = 300; level = 4;
	}
	else if (score > 110 && score <= 210)
	{
		speed = 200; level = 5;
	}
	else if (score > 210)
	{
		speed = 100; level = 6;
	}
}

void Update_Highscore()
{
	std::ifstream file;
	file.open("highscore.txt", std::ios::in);
	file >> highScore;

	if (highScore < score)
	{
		highScore = score;
	}

	std::ofstream file2;
	file2.open("highscore.txt", std::ios::out | std::ios::trunc);
	file2 << highScore;

	void close();
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE) 
	{
		srand(time(NULL));
		r0 =  rand() % 7;
		r =   rand() % 7;
		return 0;
	}

	if (uMsg == WM_DESTROY /*|| wParam == VK_ESCAPE*/) { PostQuitMessage(0); return 0; }

	if (uMsg == WM_KEYDOWN)
	{
		if (!paused)
		{
			if (wParam == VK_LEFT) { left = true; }
			if (wParam == VK_RIGHT) { right = true; }
			if (wParam == VK_DOWN) { drop = true; }
			if (wParam == VK_UP)
			{
				switch (rota)
				{
				case 0: rota = 1;
					break;
				case 1: rota = 2;
					break;
				case 2: rota = 3;
					break;
				case 3: rota = 0;
					break;
				default:
					break;
				}
			}
		}
	}

	if (uMsg == WM_CHAR)
	{
		if (wParam == 'p') { paused = !paused; }
		if (wParam == 'r') { restart = true;  paused = false; }
	}

	
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = L"MainWindow";
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	
	RegisterClassEx(&wc);

	RECT rect = { 0, 0, 800, 600 };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

	HWND windowhandle = CreateWindow( 
		L"MainWindow", 
		L"Falling Blocks",
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, //  can't resize 
		100, 
		100,
		rect.right - rect.left, 
		rect.bottom - rect.top,
		NULL, NULL, hInstance, 0);

	if (!windowhandle) return -1;

	graphics = new Graphics(); // call the default constructor 
	logic = new Logic();
	
	if (!graphics->Init(windowhandle)) // to initialize Direct2D
	{
		delete graphics;
		return -1;
	}

	ShowWindow(windowhandle, nCmdShow);

	MSG message;
	do
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			graphics->BeginDraw();
			graphics->ClearScreen();
		
			graphics->Render();
			graphics->DrawHintIndicator(525, 75, 1.0f, r0, *logic);

			Select_rand();		// selects tetrominos randomly 
			Fill_Grid();		// fills grid whenever a tetromino lamds

			graphics->Draw_Text(windowhandle);
			if (lose == 1)
			{
				graphics->You_Lose();
				if (!paused)
				{
					score = 0;
					lose = 0;
				}
			}

			if (!paused)
			{
				Moving();			// moving left and right
				Rotation();			// hoping this is right place for rotation
				GameLoop();			// everything in it is delayed 't' seconds
				if (drop) { Falling(); drop = false; }
			}
			SpeedIncrease();
			graphics->Draw_Numbers(score);
			graphics->Highscore(highScore);
			Update_Highscore();

			Clear_Lines();
			Fill_Block();		// shows the corresponding falling block

			graphics->EndDraw();
		}

	} while (WM_QUIT != message.message);
	
	delete graphics;
	delete logic;
	return 0;
} ////  SHAPE ALTER  PROBLEM     :( :< :X :X :X			(solved, again barbarian style :{) )