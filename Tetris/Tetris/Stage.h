#pragma once
#include "System.h"
#include "common.h"

class Stage : public System
{
public:
	void Initialize() override;
	void Destroy() override;
	void Update(float deltaTime) override;

	inline const char* GetRenderText() const { return renderText; }

private:
	// 매 프레임마다 데이터를 문자열로 변경해서 renderText에 저장하는 함수
	void DataToText();

	static const unsigned int StageWidth = 10;		// 게임 판의 가로 길이
	static const unsigned int StageHeight = 20;		// 게임 판의 세로 길이

	static const unsigned int SpawnHeight = 4;		// 처음 블록이 생성되는 곳의 높이
	
	static const unsigned int LinesHeight = StageHeight + SpawnHeight;		// 테트로미노가 존재할 수 있는 높이 = 게임 판의 세로 길이 + 처음 블록이 생성되는 곳의 높이

	static const unsigned int FullWidth = StageWidth + 2;	// 게임에서 사용되는 최종 너피(테두리 2줄)
	static const unsigned int FullHeight = LinesHeight + 1;	// 게임에서 사용되는 최종 높이(테두리 1줄)
	
	/* 배경의 형태
	000000000000
	000000000000
	000000000000
	000000000000
	100000000001
	100000000001
	100000000001
	100000000001
	100000000001
	100000000001
	100000000001
	100000000001
	100000000001
	100000000001
	100000000001
	100000000001
	100000000001
	100000000001
	100000000001
	100000000001
	100000000001
	100000000001
	100000000001
	100000000001
	111111111111	
	*/

	// 배경 정보
	const CellType StageBackground[FullHeight][FullWidth] =
	{
		{CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,},
		{CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,},
		{CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,},
		{CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Blank,CellType::Border,},
		{CellType::Border,CellType::Border,CellType::Border,CellType::Border,CellType::Border,CellType::Border,CellType::Border,CellType::Border,CellType::Border,CellType::Border,CellType::Border,CellType::Border,},
	};

	// enum으로 데이터를 세팅해서 그림을 만들어 놓은 캔버스
	CellType canvas[FullHeight][FullWidth];

	// canvas를 기반으로 문자열을 만들어 놓은 데이터
	char* renderText = nullptr;

	// canvasText의 길이
	int canvasTextArraySize = FullHeight * FullWidth * 4;	// Border같은 경우 char 4개가 필요해서 최대 크기로 설정
	// Blank		"  "
	// Border		"▒▒"
	// Block		"□"
	// Line			"■"

	const char* enumToChar[4] = { "  ", "▒▒", "□", "■" };
	
};
