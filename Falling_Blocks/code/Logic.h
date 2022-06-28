#pragma once
class Logic
{

	struct tetromino
	{
		int shape[4][4];
		int next_shape[4][4];
		int topleft_x;
		int topleft_y;
		int new_topleft_x;
		int new_topleft_y;
		int size_x;
		int size_y;
		int rotation;
	};

//	void Init_Shape(int A[4][4]);
	void Init_Tet(int &topleft_x, int &topleft_y, int &size_x, int &size_y, int &new_topleft_x, int &new_topleft_y, tetromino &type);

public:

	tetromino I, J, L, O, S, T, Z;
	tetromino temp;
	int bucket_height;
	int bucket_width;

	float StartPosX;
	float StartPosY;

	float PiecePosX = 275;
	float PiecePosY = 25;

	Logic();
	~Logic();


	int Bucket[10][18];
	void Init_Bucket();

	void O_tetromino();
	void I_tetromino();
	void J_tetromino();
	void L_tetromino();
	void S_tetromino();
	void Z_tetromino();
	void T_tetromino();
	void Init_Shape(int A[4][4]);
};