#include "GameManager.h"
#include "Logger.h"

void GameManager::Initialize()
{
	SetConsoleFont(L"MS Gothic", 16, 16);	

	systems.reserve(4);
	pRenderer = new ConsoleDoubleBuffer();
	systems.push_back(pRenderer);
	pInput = new Input();
	systems.push_back(pInput);
	pStage = new Stage();
	systems.push_back(pStage);

	for (System* sys : systems)
	{
		if (sys != nullptr)
		{
			sys->Initialize();
		}
	}
	lastTime = clock();
}

bool GameManager::Loop()
{
	clock_t current = clock();
	deltaTime = static_cast<float>(current - lastTime) / CLOCKS_PER_SEC;

	for (auto sys : systems)
	{
		if (sys != nullptr)
		{
			sys->Update(deltaTime);	// 게임 내 데이터 업데이트
		}
	}
	
	pRenderer->Render(pStage->GetRenderText());	// 업데이트 된 결과에 따라 그리기
	Sleep(10);	// 10 마이크로 세컨드 동안 정지

	return true;
}

void GameManager::Destroy()
{
	for (System* sys : systems)
	{
		if (sys != nullptr)
		{
			sys->Destroy();		// 시스템 전부 제거
		}
	}

	int size = systems.size();
	for (int i = 0; i < size; i++)
	{
		delete systems[i];		// 시스템 메모리 해제
		systems[i] = nullptr;
	}
	systems.clear();

}
