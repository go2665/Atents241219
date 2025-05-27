#include <iostream>
#include "BinarySearchTree.h"

void BinarySearchTree::Initialize()
{
    // 데이터 초기화
	Insert(30, 'C');
	Insert(10, 'A');
	Insert(20, 'B');
	Insert(40, 'E');
	Insert(50, 'D');
}

void BinarySearchTree::Clear()
{
    // 트리의 모든 노드 삭제
	ClearRecursive(root);
	root = nullptr;
}

bool BinarySearchTree::Insert(int key, char value)
{
	// 트리에 key를 키값으로하고 value를 값으로 하는 노드 삽입
	// 삽입 성공 시 true, 실패 시 false 반환(예: 중복된 키값 삽입 시)	

	return InsertRecursive(&root, key, value);
}

bool BinarySearchTree::Remove(int key)
{
	// 트리에서 key를 키값으로 하는 노드 삭제
	// 삭제 성공 시 true, 실패 시 false 반환(예: 키값이 존재하지 않는 경우)

return RemoveRecursive(&root, key);
}

Node* BinarySearchTree::Search(int key) const
{
	// 트리에서 key를 키값으로 하는 노드 검색
	// 검색 성공 시 해당 노드의 포인터 반환, 실패 시 nullptr 반환(예: 키값이 존재하지 않는 경우)
	return SearchRecursive(root, key);
}

void BinarySearchTree::PrintPreOrder() const
{
	// 전위 순회로 트리 출력
	// 예: 루트 -> 왼쪽 서브트리 -> 오른쪽 서브트리
	// PreOrder : (30:C) (10:A) (20:B) (40:E) (50:D)
	std::cout << "PreOrder : ";
	PreOrderRecursive(root);
	std::cout << "(END)" << std::endl;
}

void BinarySearchTree::PrintInOrder() const
{
	// 중위 순회로 트리 출력
	// 예: 왼쪽 서브트리 -> 루트 -> 오른쪽 서브트리
	// InOrder  : (10:A) (20:B) (30:C) (40:E) (50:D)
	std::cout << "InOrder  : ";
	InOrderRecursive(root);
	std::cout << "(END)" << std::endl;
}

void BinarySearchTree::PrintPostOrder() const
{
	// 후위 순회로 트리 출력
	// 예: 왼쪽 서브트리 -> 오른쪽 서브트리 -> 루트
	// PostOrder: (20:B) (10:A) (50:D) (40:E) (30:C)
	std::cout << "PostOrder: ";
	PostOrderRecursive(root);
	std::cout << "(END)" << std::endl;
}

void BinarySearchTree::ClearRecursive(Node* node)
{
	if (node == nullptr)
		return;

	// node가 nullptr이 아닐 때, 자식 노드를 삭제 시도(왼쪽과 오른쪽 서브트리를 재귀적으로 삭제)
	ClearRecursive(node->left);
	ClearRecursive(node->right);

	// 왼쪽과 오른쪽 노드가 모두 삭제됨. 자기 자신을 삭제
	delete node;
}

bool BinarySearchTree::InsertRecursive(Node** node, int key, char value)
{
	if (*node == nullptr)
	{
		// 현재 노드가 nullptr이면 새로운 노드를 생성하여 삽입
		*node = new Node{ key, value, nullptr, nullptr };
		return true; // 삽입 성공
	}

	if (key < (*node)->key)
	{
		return InsertRecursive(&(*node)->left, key, value);	// key가 현재 노드의 키보다 작으면 왼쪽 서브트리로 이동
	}
	else if (key > (*node)->key)
	{
		return InsertRecursive(&(*node)->right, key, value);	// key가 현재 노드의 키보다 크면 오른쪽 서브트리로 이동
	}
	else
	{
		// 중복된 키값 삽입 시 false 반환
		return false;
	}
}

bool BinarySearchTree::RemoveRecursive(Node** node, int key)
{
	if (*node == nullptr)
	{
		return false; // 키가 존재하지 않음
	}

	if (key < (*node)->key)
	{
		return RemoveRecursive(&(*node)->left, key); // 왼쪽 서브트리로 이동
	}
	else if (key > (*node)->key)
	{
		return RemoveRecursive(&(*node)->right, key); // 오른쪽 서브트리로 이동
	}
	else
	{
		// 삭제할 노드를 찾음(자식이 없다, 자식이 하나다(왼쪽 자식이다, 오른쪽 자식이다), 자식이 둘이다)
		Node* temp = *node;
		
		if ((*node)->left == nullptr)
		{
			// 자식이 둘 다 없거나 왼쪽 자식이 없는 경우
			*node = (*node)->right; // 오른쪽 자식으로 대체(자식이 둘 다 없는 경우 어차피 오른쪽은 nullptr이 된다)
		}
		else if ((*node)->right == nullptr)
		{
			*node = (*node)->left; // 왼쪽 자식으로 대체
		}
		else
		{
			Node* successor = (*node)->right;
			Node* successorParent = *node;

			while (successor->left != nullptr)
			{
				successorParent = successor;
				successor = successor->left; // 오른쪽 서브트리에서 가장 작은 노드 찾기
			}

			(*node)->key = successor->key;		// 현재 노드의 키를 후계자의 키로 대체
			(*node)->value = successor->value;	// 현재 노드의 값을 후계자의 값으로 대체

			if (successorParent->left == successor)
			{
				// 후계자가 왼쪽 자식인 경우
				successorParent->left = successor->right; // 후계자의 오른쪽 자식을 후계자의 부모의 왼쪽 자식으로 대체
			}
			else
			{
				// 후계자가 오른쪽 자식인 경우
				successorParent->right = successor->right; // 후계자의 오른쪽 자식을 후계자의 부모의 오른쪽 자식으로 대체
			}
			// successor는 node의 오른쪽 서브트리에서 가장 작은 노드이므로, 왼쪽 자식이 없을 수 없다.
		}
			
		delete temp;
		return true;
	}

	return false;
}

Node* BinarySearchTree::SearchRecursive(Node* node, int key) const
{	
	if (node == nullptr)
	{
		return nullptr; 
	}

	if (key < node->key)
	{
		return SearchRecursive(node->left, key);
	}
	else if (key > node->key)
	{
		return SearchRecursive(node->right, key);
	}

	return node;
}

void BinarySearchTree::PreOrderRecursive(const Node* node) const
{
	if (node == nullptr)
		return;
	// 현재 노드 출력
	std::cout << "(" << node->key << ":" << node->value << ") ";

	// 왼쪽 서브트리 방문
	PreOrderRecursive(node->left);

	// 오른쪽 서브트리 방문
	PreOrderRecursive(node->right);
}

void BinarySearchTree::InOrderRecursive(const Node* node) const
{
	if (node == nullptr)
		return;

	// 왼쪽 서브트리 방문
	InOrderRecursive(node->left);
	// 현재 노드 출력
	std::cout << "(" << node->key << ":" << node->value << ") ";
	// 오른쪽 서브트리 방문
	InOrderRecursive(node->right);
}

void BinarySearchTree::PostOrderRecursive(const Node* node) const
{
	if (node == nullptr)
		return;
	// 왼쪽 서브트리 방문
	PostOrderRecursive(node->left);
	// 오른쪽 서브트리 방문
	PostOrderRecursive(node->right);
	// 현재 노드 출력
	std::cout << "(" << node->key << ":" << node->value << ") ";
}
