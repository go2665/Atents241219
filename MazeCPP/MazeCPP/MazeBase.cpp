#include "MazeBase.h"
#include "Random.h"

void MazeBase::MakeMaze(int Width, int Height, int Seed)
{
	this->Width = Width;
	this->Height = Height;

	if (Seed != -1)	// 설정된 Seed가 있으면 적용
	{
		Random::Get().SetSeed(Seed);
	}

	ClearMaze();	// 기존 미로가 있으면 제거

	Cells = new CellBase*[Width * Height];	// CellBase*의 배열 생성

	OnSpecificAlgorithmExcute();	// 실제 알고리즘 수행		
}

void MazeBase::ConnectPath(CellBase* From, CellBase* To)
{
	// 대각선 연결은 없다.
	// From셀과 To셀은 인접해 있다.
	// 동쪽은 X+, 서쪽은 X-, 남쪽은 Y+, 북쪽은 Y-

	if (From->GetY() > To->GetY())			// To가 From의 북쪽에 있다.
	{
		From->MakePath(Direction::North);
		To->MakePath(Direction::South);
	}
	else if (From->GetX() < To->GetX())		// To가 From의 동쪽에 있다.
	{
		From->MakePath(Direction::East);
		To->MakePath(Direction::West);
	}
	else if (From->GetY() < To->GetY())		// To가 From의 남쪽에 있다.
	{
		From->MakePath(Direction::South);
		To->MakePath(Direction::North);
	}
	else if (From->GetX() > To->GetX())		// To가 From의 서쪽에 있다.
	{
		From->MakePath(Direction::West);
		To->MakePath(Direction::East);
	}

}
