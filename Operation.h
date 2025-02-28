#pragma once
#include "TreeNode.h"
#include "Napis.h"

class Operation : public TreeNode
{
protected:
	Napis m_opr;
public:
	Operation(const Napis& opr);

	Napis toNapisJustNode() const override;
};
