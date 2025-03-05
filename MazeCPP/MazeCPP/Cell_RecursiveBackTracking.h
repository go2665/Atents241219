#pragma once
#include "CellBase.h"

class Cell_RecursiveBackTracking : public CellBase
{
public:
	// getter
	inline bool IsVisited() const { return bIsVisited; }
	// setter
	inline void SetVisited(bool bIsVisited) { this->bIsVisited = bIsVisited; }

private:
	bool bIsVisited = false;
};