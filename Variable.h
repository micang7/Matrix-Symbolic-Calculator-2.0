#pragma once
#include "TreeNode.h"

class Variable : public TreeNode
{
	char m_name;
public:
	Variable(char name);

	TreeNode* clone() const override;
	TreeNode* shift() override;

	AbstractSyntaxTree evaluate1(const Napis& opr, TreeNode* right) const override;

	Napis toNapis() const override;
};