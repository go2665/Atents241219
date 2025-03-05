// MazeCPP.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "Maze_Wilson.h"

int main()
{
    int width = 10;
    int height = 10;

	Maze_Wilson* maze = new Maze_Wilson();
	maze->MakeMaze(width, height, 10);
	maze->ClearMaze();
	delete maze;
	maze = nullptr;
}

// 필요한 클래스
// 1. Cell : 미로의 한 셀을 나타내는 클래스
// 2. Maze : 미로를 나타내는 클래스
