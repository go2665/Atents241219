#pragma once
template <typename T>
class Singleton
{
protected:
	Singleton() = default;	// 기본생성자와 같지만 protected로 지정해서 외부에서 인스턴스 생성을 못하게 만든다.
	virtual ~Singleton() {};	// 자신의 소멸자가 실행되게 하기 위해 가상함수로 설정

	// 복사생성자와 대입연산자를 삭제해서 복사 및 할당을 방지
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

public:	
	// 싱글톤 인스턴스를 반환하는 static 메서드
	static T& Get()
	{
		static T instance;	// static 지역변수로 싱글톤 인스턴스 생성(첫번째로 Get함수를 호출했을 때 생성됨)
		return instance;
	}
};

