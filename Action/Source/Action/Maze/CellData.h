#pragma once
#include "CoreMinimal.h"
#include "EDirectionType.h"
#include "FVector2I.h"

class ACTION_API CellData
{
public:
	// 특정 방향으로 길을 뚫는 함수
	inline void MakePath(EDirectionType InPath) { Path |= InPath; }

	// 특정 방향이 길인지 확인하는 함수
	inline bool IsPath(EDirectionType InCheck) const { return EnumHasAnyFlags(Path, InCheck); }
	//inline bool IsPath(EDirectionType InCheck) const { return (Path & InCheck) != EDirectionType::None; }

	// 특정 방향이 벽인지 확인하는 함수
	inline bool IsWall(EDirectionType InCheck) const { return !IsPath(InCheck); }

	// getter
	inline int8 GetX() const { return X; }
	inline int8 GetY() const { return Y; }
	inline FVector2I GetLocation() const { return FVector2I(X, Y); }
	inline EDirectionType GetPath() const { return Path; }
	inline CellData* GetNextCell() const { return NextCell; }
	inline bool IsMazeMember() const { return bIsMazeMember; }

	// setter
	inline void SetLocation(int InX, int InY) { X = InX; Y = InY; }
	inline void SetNextCell(CellData* InNextCell) { NextCell = InNextCell; }
	inline void SetMazeMember(bool bInIsMazeMember) { bIsMazeMember = bInIsMazeMember; }

private:
	// 셀의 X좌표
	int8 X = 0;

	// 셀의 Y좌표
	int8 Y = 0;

	// 셀에 열려있는	문의 방향
	EDirectionType Path = EDirectionType::None;

	// 다음 셀(길을 뚫어야 하는 방향에 있는 셀)
	CellData* NextCell = nullptr;

	// 미로에 포함되었는지 여부(true면 포함, false면 미포함)
	bool bIsMazeMember = false;	
};