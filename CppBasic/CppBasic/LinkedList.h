#pragma once

struct ListNode
{
public:
	int data = 0;
	ListNode* next = nullptr;
};

class LinkedList
{
public:
	// 동작
	void Add(int data);						// 마지막에 데이터를 추가하는 함수
	void Insert(int data, int position);	// Data를 position번째에 추가(position이 전체길이를 벗어나면 마지막에 추가)
	void Remove(int data);					// 리스트에서 data를 삭제
	void RemoveAt(int position);			// 리스트에서 position번째의 노드를 제거(position이 전체길이를 벗어나면 처리안함)
	void Clear();							// 리스트의 모든 노드를 삭제
	void Print();							// 리스트를 처음부터 끝까지 출력하는 함수

private:
	// 데이터
	ListNode* head = nullptr;


};

