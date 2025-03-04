#pragma once

struct FVector2I
{
public:
	int X = 0;
	int Y = 0;
	FVector2I() = default;
	FVector2I(int X, int Y) : X(X), Y(Y) {}

	// 두 벡터가 같다.
	// 두 벡터가 다르다.
	// 두 벡터를 더한다.
	// 두 벡터를 뺀다.

};