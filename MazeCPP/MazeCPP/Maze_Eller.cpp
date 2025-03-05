#include "Maze_Eller.h"
#include "Random.h"
#include <unordered_map>
#include <vector>

void Maze_Eller::OnSpecificAlgorithmExcute()
{
	/// 1. 한 줄 만들기
	///     1.1. 위쪽 줄을 참조해서 한 줄 만들기
	///         1.1.1. 위쪽 셀에 벽이 없으면 위쪽 셀과 같은 집합에 포함시킨다.
	///         1.1.2. 위쪽 셀에 벽이 있으면 새 집합에 포함시킨다.
	///     1.2. 첫번째 줄은 가로 길이만큼 셀을 만들고 각각 고유한 집합에 포함시킨다.
	/// 2. 옆칸끼리 합치기
	///     2.1. 서로 집합이 다르면 랜덤하게 벽을 제거하고 같은 집합으로 만든다.(같은 줄에 있는 같은 종류의 셀이 한번에 바뀐다.)
	///     2.2. 서로 같은 집합이면 패스
	/// 3. 아래쪽 벽 제거하기
	///     3.1. 같은 집합당 최소 하나 이상의 벽이 제거되어야 한다.
	/// 4. 한 줄 완료 -> 1번으로 돌아가기
	/// 5. 마지막 줄 만들고 정리
	///     5.1. 생성까진 똑같이 처리
	///     5.2. 합칠 때 세트가 다르면 무조건 합친다.
	
	int h = Height - 1;
	Cell_Eller* UpperLine = nullptr;
	for (int y = 0; y < h; ++y)
	{
		// 1. 한 줄 만들기
		Cell_Eller* Line = MakeLine(UpperLine);

		// 2. 옆칸끼리 합치기
		MergeAdjacentCell(Line, AdjacentMergeRate);

		// 3. 아래쪽 벽 제거하기
		RemoveDownWall(Line);
		
		WriteLine(Line);	// 완료된 줄을 Cells에 기록		
		UpperLine = Line;	// 완료된 줄을 UpperLine으로 설정
	}

	// 5. 마지막 줄 만들고 정리
	Cell_Eller* LastLine = MakeLine(UpperLine);
	MergeAdjacentCell(LastLine, 1.1f);	// 마지막 줄은 무조건 합친다.
	WriteLine(LastLine);				// 마지막줄 Cells에 기록
}

Cell_Eller* Maze_Eller::MakeLine(Cell_Eller* UpperLine)
{
	/// 1. 한 줄 만들기
	///     1.1. 위쪽 줄을 참조해서 한 줄 만들기
	///         1.1.1. 위쪽 셀에 벽이 없으면 위쪽 셀과 같은 집합에 포함시킨다.
	///         1.1.2. 위쪽 셀에 벽이 있으면 새 집합에 포함시킨다.
	///     1.2. 첫번째 줄은 가로 길이만큼 셀을 만들고 각각 고유한 집합에 포함시킨다.
	
	int Row = (UpperLine == nullptr) ? 0 : (UpperLine->GetY() + 1);		// 몇번째 줄인지 결정
	Cell_Eller* Line = new Cell_Eller[Width]();		// 한줄 메모리 할당

	for (int x = 0; x < Width; x++)
	{
		Line[x].SetLocation(x, Row);	// 셀의 위치 설정
		if (UpperLine && UpperLine[x].IsPath(Direction::South))	// 위쪽 셀에 남쪽 벽이 없으면
		{
			Line[x].SetSetGroup(UpperLine[x].GetSetGroup());	// 위쪽 셀과 같은 집합에 포함
			Line[x].MakePath(Direction::North);					// 위쪽 셀과 연결	(위쪽셀은 남쪽으로 길이 나있으니까)
		}
		else
		{
			// 위쪽 셀이 없거나 위쪽쎌에 남쪽 벽이 있다.
			Line[x].SetSetGroup(SerialNumber);	// 유니크한 새 집합에 포함
			SerialNumber++;						// 집합 번호 증가	(다음 번에도 유니한 값이 된다.)
		}
	}

	return Line;
}

void Maze_Eller::MergeAdjacentCell(Cell_Eller* Line, float Rate)
{
	/// 2. 옆칸끼리 합치기
	///     2.1. 서로 집합이 다르면 랜덤하게 벽을 제거하고 같은 집합으로 만든다.(같은 줄에 있는 같은 종류의 셀이 한번에 바뀐다.)
	///     2.2. 서로 같은 집합이면 패스
	
	int Count = 1;		// 한줄이 모두 같은 집합에 속하는 것을 방지하기 위해 만들어 놓은 변수
	int W = Width - 1;	// 마지막 셀은 다음 셀이 없으므로 제외
	for (int x = 0; x < W; ++x)
	{
		if (Count < W	// 모든 셀이 같은 집합에 속하는 것을 방지
			&& Line[x].GetSetGroup() != Line[x + 1].GetSetGroup()	// 지금 셀과 옆의 셀이 서로 다른 집합이면
			&& Random::Get().GetRandomFloat() < Rate)				// 합칠 확률을 만족하면
		{
			Cell_Eller* Current = &Line[x];
			Cell_Eller* Next = &Line[x + 1];
			Current->MakePath(Direction::East);		// 현재셀과 다음셀을 연결
			Next->MakePath(Direction::West);		// 다음셀과 현재셀을 연결

			int TargetGroup = Next->GetSetGroup();	// 다음셀의 집합 번호를 가져와서 이 집합에 속한 모든 셀은 Current의 집합으로 변경
			Next->SetSetGroup(Current->GetSetGroup());	// 다음셀을 현재셀과 같은 집합으로 설정
			Count++;									// 한번 합쳤으니까 카운트 증가
			for (int i = x + 2; i < Width; i++)			// 다음셀부터 끝까지 확인
			{
				if (Line[i].GetSetGroup() == TargetGroup)	// 다음셀의 집합 번호와 같은 집합이면
				{
					Line[i].SetSetGroup(Current->GetSetGroup());	// 현재셀과 같은 집합으로 설정
					Count++;										// 설정했으니 카운트 증가
				}
			}
		}
	}
}

void Maze_Eller::RemoveDownWall(Cell_Eller* Line)
{
	/// 3. 아래쪽 벽 제거하기
	///     3.1. 같은 집합당 최소 하나 이상의 벽이 제거되어야 한다.

	std::unordered_map<int, std::vector<int>> GroupMap;	// 집합별로 셀의 목록을 저장하는 맵
	for (int x = 0; x < Width; x++)
	{
		int key = Line[x].GetSetGroup();
		if (GroupMap.find(key) == GroupMap.end())	// 집합이 없으면
		{
			GroupMap[key] = std::vector<int>();		// 새로운 벡터를 생성
			GroupMap[key].reserve(Width);			// 가로길이만큼 공간 확보
		}
		GroupMap[key].push_back(x);	// 집합에 셀을 추가
	}

	for (const auto& Pair : GroupMap)
	{
		std::vector<int> Array = Pair.second;	// 집합에 속한 셀 목록
		int Size = Array.size();
		for (int i = 0; i < Size; i++)
		{
			if (Random::Get().GetRandomFloat() < DownWallRemoveRate)	// 제거할 확률을 만족하면
			{
				Line[Array[i]].MakePath(Direction::South);	// 아래쪽 벽 제거				
			}
		}
		int MustIndex = Random::Get().GetRandomInRange(0, Size - 1);	// 반드시 하나는 아래쪽 제거
		Line[Array[MustIndex]].MakePath(Direction::South);	
	}
}

void Maze_Eller::WriteLine(Cell_Eller* Line)
{
	int Index = Line[0].GetY() * Width;
	for (int x = 0; x < Width; x++)
	{
		Cells[Index + x] = &Line[x];	// 완성된 줄을 Cells에 기록
	}
}
