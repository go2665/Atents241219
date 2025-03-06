// MazeCPP.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "Maze_Wilson.h"
#include "Maze_RecursiveBackTracking.h"
#include "Maze_Eller.h"

void PrintMaze(int Width, int Height, CellBase** cells);	// 함수 선언
int GetSubIndex(int StartIndex, int X, int Y, int Width);	

int main()
{
    int width = 30;
    int height = 10;	

	/*Maze_Wilson* maze = new Maze_Wilson();
	maze->MakeMaze(width, height, -1);
	PrintMaze(width, height, maze->GetCells());
	maze->ClearMaze();
	delete maze;
	maze = nullptr;*/

	//Maze_RecursiveBackTracking* BackTrackingMaze = new Maze_RecursiveBackTracking();
	//BackTrackingMaze->MakeMaze(width, height, -1);
	//PrintMaze(width, height, BackTrackingMaze->GetCells());
	//BackTrackingMaze->ClearMaze();
	//delete BackTrackingMaze;
	//BackTrackingMaze = nullptr;

	//Maze_Eller* EllerMaze = new Maze_Eller();
	//EllerMaze->MakeMaze(width, height, -1);
	//PrintMaze(width, height, EllerMaze->GetCells());
	//EllerMaze->ClearMaze();
	//delete EllerMaze;
	//EllerMaze = nullptr;
}

void PrintMaze(int Width, int Height, CellBase** cells)	// 함수의 실제 정의
{
	// * : 벽, ' ' : 길과 셀

	// 셀하나의 기본 모습
	// ***
	// * *
	// ***

	// 2*2 미로 예시
	// ******
	// *    *
	// * ****
	// * ****
	// *    *
	// ******

	int Length = Width * Height * 9 + Height * 3 + 1;	// 셀표현용(가로 * 세로 * 셀한칸9) + 엔터키(세로*3) + 널문자(1)
	char* Canvas = new char[Length]();
	std::fill(Canvas, Canvas + Length - 1, '\n');		// 전체를 엔터로 초기화

	for (int y = 0; y < Height; ++y)
	{
		for (int x = 0; x < Width; ++x)
		{
			int CellStartIndex = x * 3 + y * Width * 9 + y * 3;	// 셀의 시작 인덱스
			
			// 셀의 모서리부분
			Canvas[GetSubIndex(CellStartIndex, 0, 0, Width)] = '*';	
			Canvas[GetSubIndex(CellStartIndex, 2, 0, Width)] = '*';	
			Canvas[GetSubIndex(CellStartIndex, 0, 2, Width)] = '*';	
			Canvas[GetSubIndex(CellStartIndex, 2, 2, Width)] = '*';	

			// 셀의 가운데
			Canvas[GetSubIndex(CellStartIndex, 1, 1, Width)] = ' ';	

			// 셀의 벽부분
			CellBase* Cell = cells[y * Width + x];
			Canvas[GetSubIndex(CellStartIndex, 1, 0, Width)] = Cell->IsWall(Direction::North) ? '*' : ' ';
			Canvas[GetSubIndex(CellStartIndex, 0, 1, Width)] = Cell->IsWall(Direction::West) ? '*' : ' ';
			Canvas[GetSubIndex(CellStartIndex, 2, 1, Width)] = Cell->IsWall(Direction::East) ? '*' : ' ';
			Canvas[GetSubIndex(CellStartIndex, 1, 2, Width)] = Cell->IsWall(Direction::South) ? '*' : ' ';
		}
	}

	printf("%s\n", Canvas);

	delete[] Canvas;
	Canvas = nullptr;
}

int GetSubIndex(int StartIndex, int SubX, int SubY, int Width)
{
	return StartIndex		// 시작 인덱스
		+ SubX	 			// SubX만큼 이동하는 것은 그냥 옆으로 한칸 이동한 것
		+ SubY * Width * 3	// SubY만큼 이동하는 것은 가로길이(셀기준길이) * 3(한셀의 가로길이는 3)만큼 이동한 것
		+ SubY;				// SubY만큼 이동해서 엔터키(세로길이)만큼 추가
}

// 필요한 클래스
// 1. Cell : 미로의 한 셀을 나타내는 클래스
// 2. Maze : 미로를 나타내는 클래스
