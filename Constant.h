#pragma once
#include "TreeNode.h"

class Constant : public TreeNode
{
	int m_val;
public:
	Constant(int val);

	TreeNode* clone() const override;

	Napis toNapis() const override;
};