#pragma once
#include "MazeEnum.h"

// 미로의 기본 셀을 나타내는 클래스
class CellBase
{
public:
	virtual ~CellBase() = default;	// 가상 소멸자(메	모리 누수 방지 + 다형성을 위해)

	// 지정된 방향으로 길을 여는 함수
	inline void MakePath(Direction PathDirection) { Path |= PathDirection; }

	// 지정된 방향이 길인지 확인하는 함수(길이면 true, 아니면 false)
	inline bool IsPath(Direction CheckDirection) const { return (Path & CheckDirection) != Direction::None; }

	// 지정된 방향이 벽인지 확인하는 함수(벽이면 true, 아니면 false)
	inline bool IsWall(Direction CheckDirection) const { return !IsPath(CheckDirection); }

	// Getter
	// X좌표 반환
	int GetX() const { return X; }

	// Y좌표 반환
	int GetY() const { return Y; }

	// 이 셀에 열려있는 방향 반환
	Direction GetPath() const { return Path; }

	// Setter
	// 이 셀의 위치를 설정
	inline void SetLocation(int X, int Y) { this->X = X; this->Y = Y; }

	// 데이터
private:
	int X = 0;
	int Y = 0;

	// 이 셀에 열려있는 방향
	Direction Path = Direction::None;
};