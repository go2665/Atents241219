#pragma once

// 미로 셀의 방향을 나타내는 열거형 클래스. 비트플래그로 사용(북동남서 순으로 비트값을 가짐)
enum class Direction : unsigned char
{
	None = 0,
	North = 1,
	East = 2,
	South = 4,
	West = 8
};

// Direction용 명령어 오버로딩
inline Direction operator|(Direction left, Direction right)
{
	return static_cast<Direction>(static_cast<unsigned char>(left) | static_cast<unsigned char>(right));
}

inline Direction operator&(Direction left, Direction right)
{
	return static_cast<Direction>(static_cast<unsigned char>(left) & static_cast<unsigned char>(right));
}

inline Direction& operator|=(Direction& left, Direction right)
{
	left = static_cast<Direction>(static_cast<unsigned char>(left) | static_cast<unsigned char>(right));
	return left;
}

inline Direction& operator&=(Direction& left, Direction right)
{
	left = static_cast<Direction>(static_cast<unsigned char>(left) & static_cast<unsigned char>(right));
	return left;
}