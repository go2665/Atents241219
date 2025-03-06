// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
struct ACTION_API FVector2I
{
public:
	int8 X = 0;
	int8 Y = 0;

	FVector2I() = default;
	FVector2I(int8 InX, int8 InY) : X(InX), Y(InY) {}

	FVector2I operator+(const FVector2I& Other) const
	{
		return FVector2I(X + Other.X, Y + Other.Y);
	}

	FVector2I operator-(const FVector2I& Other) const
	{
		return FVector2I(X - Other.X, Y - Other.Y);
	}

	bool operator==(const FVector2I& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	bool operator!=(const FVector2I& Other) const
	{
		return X != Other.X || Y != Other.Y;
	}
};
