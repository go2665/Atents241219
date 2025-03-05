#pragma once

struct FVector2I
{
public:
	int X = 0;
	int Y = 0;
	FVector2I() = default;
	FVector2I(int X, int Y) : X(X), Y(Y) {}

	// 두 벡터가 같다.
	bool operator==(const FVector2I& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	// 두 벡터가 다르다.
	bool operator!=(const FVector2I& Other) const
	{
		//return !(*this == Other);
		//return !(X == Other.X && Y == Other.Y);
		return X != Other.X || Y != Other.Y;
	}

	// 두 벡터를 더한다.
	FVector2I operator+(const FVector2I& Other) const
	{
		return FVector2I(X + Other.X, Y + Other.Y);
	}

	// 두 벡터를 뺀다.
	FVector2I operator-(const FVector2I& Other) const
	{
		return FVector2I(X - Other.X, Y - Other.Y);
	}

};