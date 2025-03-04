#pragma once
#include "MazeEnum.h"

// 미로의 기본 셀을 나타내는 클래스
class CellBase
{
public:
	virtual ~CellBase() = default;	// 가상 소멸자(메	모리 누수 방지 + 다형성을 위해)

	// 지정된 방향으로 길을 여는 함수
	// void MakePath(Direction direction);

	// 지정된 방향이 길인지 확인하는 함수
	// 지정된 방향이 벽인지 확인하는 함수

	// Getter
	// X좌표 반환
	int GetX() const { return X; }

	// Y좌표 반환
	int GetY() const { return Y; }

	// 이 셀에 열려있는 방향 반환
	Direction GetPath() const { return Path; }

	// Setter
	// 이 셀의 위치를 설정
	inline void SetLocation(int x, int y) { this->X = x; this->Y = y; }

	// 데이터
private:
	int X = 0;
	int Y = 0;

	Direction Path = Direction::None;
};