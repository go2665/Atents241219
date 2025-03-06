#pragma once
#include "CoreMinimal.h"
#include "EDirectionType.h"

class CellData
{
public:
	inline void MakePath(EDirectionType InPath) { Path |= InPath; }

	inline bool IsPath(EDirectionType InCheck) const { return EnumHasAnyFlags(Path, InCheck); }
	//inline bool IsPath(EDirectionType InCheck) const { return (Path & InCheck) != EDirectionType::None; }
	inline bool IsWall(EDirectionType InCheck) const { return !IsPath(InCheck); }

	// getter
	inline int8 GetX() const { return X; }
	inline int8 GetY() const { return Y; }
	inline EDirectionType GetPath() const { return Path; }

	// setter
	inline void SetLocation(int InX, int InY) { X = InX; Y = InY; }

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