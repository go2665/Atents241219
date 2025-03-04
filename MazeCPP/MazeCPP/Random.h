#pragma once
#include <random>
#include "Singleton.h"

class Random : public Singleton<Random>
{
public:
	// 생성자(random_device를 이용해 시드값 설정)
	Random() : generator(std::random_device()()) {}

	// 시드값을 입력 받는 생성자
	explicit Random(unsigned int seed) : generator(seed) {}	// explicit : 명시적 변환만 허용. 가독성을 높이고 불필요한 암시적변환을 방지
	
	/// <summary>
	/// 새 시드값 설정하기
	/// </summary>
	/// <param name="seed">새로운 시드값</param>
	void SetSeed(unsigned int seed)
	{
		generator.seed(seed);
	}

	/// <summary>
	/// 랜덤한 정수를 리턴하는 함수( 0 ~ (maxExclude-1) 범위)
	/// </summary>
	/// <param name="maxExclude">최대값-1</param>
	/// <returns>랜덤한 정수</returns>
	int GetRandom(int maxExclude)
	{
		// 균등분포로 처리
		std::uniform_int_distribution<int> dis(0, maxExclude - 1);	// 파라메터로 입력받은 갯수만큼 처리하기 위해
		return dis(generator);
	}

	/// <summary>
	/// 지정된 범위의 정수를 리턴하는 함수
	/// </summary>
	/// <param name="minInclude">최소값</param>
	/// <param name="maxInclude">최대값</param>
	/// <returns>min~max 범위의 정수</returns>
	int GetRandomInRange(int minInclude, int maxInclude)
	{
		std::uniform_int_distribution<int> dis(minInclude, maxInclude);
		return dis(generator);
	}

	/// <summary>
	/// 0~1사이의 랜덤한 실수를 리턴하는 함수
	/// </summary>
	/// <returns>0~1사이의 값</returns>
	float GetRandomFloat()
	{
		std::uniform_real_distribution<float> dis(0.0f, 1.0f);
		return dis(generator);
	}

	/// <summary>
	/// STL 컨테이너를 무작위로 셔플하는 함수
	/// </summary>
	/// <typeparam name="T">STL 컨테이너</typeparam>
	/// <param name="container">셔플할 데이터가 들어있는 컨테이너</param>
	template<typename T>
	void Shuffle(T& container)
	{
		std::shuffle(container.begin(), container.end(), generator);
	}

private:
	std::mt19937 generator;
};

