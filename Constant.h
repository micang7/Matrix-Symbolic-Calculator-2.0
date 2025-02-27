#pragma once
#include "TreeNode.h"

class Constant : public TreeNode
{
	int m_val;
public:
	Constant(int val);

	TreeNode* clone() const override;
	TreeNode* shift() override;

	AbstractSyntaxTree evaluate1(const Napis& opr, TreeNode* right) const override;
	AbstractSyntaxTree evaluate2(const Constant& left, const Napis& opr) const override;

	AbstractSyntaxTree operator+(const Constant& constant2) const;
	AbstractSyntaxTree operator-(const Constant& constant2) const;
	AbstractSyntaxTree operator*(const Constant& constant2) const;
	AbstractSyntaxTree operator/(const Constant& constant2) const;
	AbstractSyntaxTree operator^(const Constant& constant2) const;

	Napis toNapis() const override;
};