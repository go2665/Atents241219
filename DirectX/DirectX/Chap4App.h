#pragma once
#include "./Common/D3DApp.h"

class Chap4App : public D3DApp
{
public :
	// 사용할 생성자 및 소멸자
	Chap4App(HINSTANCE hInstance);
	virtual ~Chap4App();

	// 제거할 생성자 및 대입 연산자
	Chap4App(const Chap4App& rhs) = delete;				// 복사 생성자 삭제(싱글톤 처리를 위해)
	Chap4App& operator=(const Chap4App& rhs) = delete;	// 대입 연산자 삭제(싱글톤 처리를 위해)

protected:
	virtual void Update(const GameTimer& gt) override {};	// 프레임 처리(게임 로직 업데이트)
	virtual void Draw(const GameTimer& gt) override;		// 랜더링(화면 출력)
};

