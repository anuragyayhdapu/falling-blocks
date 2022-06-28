#include "Logic.h"

Logic::Logic()
{
	StartPosX = 275;
	StartPosY = 50;
	Init_Bucket();
	O_tetromino();
	I_tetromino();
	J_tetromino();
	L_tetromino();
	S_tetromino();
	Z_tetromino();
	T_tetromino();
}


Logic::~Logic()
{
}

void Logic::Init_Bucket()
{
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 18; y++)
		{
			/*if (y > 13 && x > 0 && x < 9)
			{
				Bucket[x][y] = 1;
			}
			else
			{*/
				Bucket[x][y] = 0;
			/*}*/
		}
	}

	/*Bucket[0][17] = 1;
	Bucket[1][16] = Bucket[1][14] = Bucket[1][15] = 0;
	Bucket[2][14] = 0;
	Bucket[5][14] = Bucket[5][15] = 0;
	Bucket[6][14] = Bucket[6][15] = Bucket[6][16] = 0;
	Bucket[8][14] = Bucket[8][15] = Bucket[8][16] = Bucket[8][17] = 0;
	Bucket[9][17] = 1;
*/
}

void Logic::O_tetromino()
{
	Init_Shape(O.shape);
	Init_Shape(O.next_shape);
	O.shape[1][0] = O.shape[2][0] = O.shape[1][1] = O.shape[2][1] = 1;

	Init_Tet(O.topleft_x, O.topleft_y, O.size_x, O.size_y, O.new_topleft_x, O.new_topleft_y, O);
}

void Logic::I_tetromino()
{
	Init_Shape(I.shape);
	Init_Shape(I.next_shape);
	I.shape[1][0] = I.shape[1][1] = I.shape[1][2] = I.shape[1][3] = 1;

	Init_Tet(I.topleft_x, I.topleft_y, I.size_x, I.size_y, I.new_topleft_x, I.new_topleft_y, I);
}

void Logic::J_tetromino()
{
	Init_Shape(J.shape);
	Init_Shape(J.next_shape);
	J.shape[2][0] = J.shape[2][1] = J.shape[2][2] = J.shape[1][2] = 1;

	Init_Tet(J.topleft_x, J.topleft_y, J.size_x, J.size_y, J.new_topleft_x, J.new_topleft_y, J);
}

void Logic::L_tetromino()
{
	Init_Shape(L.shape);
	Init_Shape(L.next_shape);
	L.shape[1][0] = L.shape[1][1] = L.shape[1][2] = L.shape[2][2] = 1;

	Init_Tet(L.topleft_x, L.topleft_y, L.size_x, L.size_y, L.new_topleft_x, L.new_topleft_y, L);
}

void Logic::S_tetromino()
{
	Init_Shape(S.shape);
	Init_Shape(S.next_shape);
	S.shape[1][2] = S.shape[2][1] = S.shape[2][2] = S.shape[3][1] = 1;

	Init_Tet(S.topleft_x, S.topleft_y, S.size_x, S.size_y, S.new_topleft_x, S.new_topleft_y, S);
	S.topleft_y = 25 * 1;
	S.new_topleft_y = 0;
}

void Logic::Z_tetromino()
{
	Init_Shape(Z.shape);
	Init_Shape(Z.next_shape);
	Z.shape[1][1] = Z.shape[2][1] = Z.shape[2][2] = Z.shape[3][2] = 1;

	Init_Tet(Z.topleft_x, Z.topleft_y, Z.size_x, Z.size_y, Z.new_topleft_x, Z.new_topleft_y, Z);
	Z.topleft_y = 25 * 1;
	Z.new_topleft_y = 0;
}

void Logic::T_tetromino()
{
	Init_Shape(T.shape);
	Init_Shape(T.next_shape);
	T.shape[1][1] = T.shape[2][0] = T.shape[2][1] = T.shape[3][1] = 1;

	Init_Tet(T.topleft_x, T.topleft_y, T.size_x, T.size_y, T.new_topleft_x, T.new_topleft_y, T);
}

void Logic::Init_Shape(int A[4][4])
{
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			A[x][y] = 0;
		}
	}
}

void Logic::Init_Tet(int &topleft_x, int &topleft_y, int &size_x, int &size_y, int &new_topleft_x, int &new_topleft_y, tetromino &type)
{
	type.topleft_x = 25 * 10;
	type.topleft_y = 25 * 2;
	type.size_x = 2;
	type.size_y = 2;
	type.new_topleft_x = 3;
	type.new_topleft_y = 1;
}