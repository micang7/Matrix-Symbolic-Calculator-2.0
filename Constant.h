#pragma once
#include "TreeNode.h"

class Constant : public TreeNode
{
	int m_val;
public:
	Constant(int val);

	TreeNode* clone() const override;

	bool simplify_L(AbstractSyntaxTree& ast, UnaryOperation* opr) override;
	bool simplify_L(AbstractSyntaxTree& ast, BinaryOperation* opr) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Constant* left) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Matrix* left) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Variable* left) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, BinaryOperation* left) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, UnaryOperation* left) override;

	AbstractSyntaxTree operator+(const Constant& constant2) const;
	AbstractSyntaxTree operator-(const Constant& constant2) const;
	AbstractSyntaxTree operator*(const Constant& constant2) const;
	AbstractSyntaxTree operator/(const Constant& constant2) const;
	AbstractSyntaxTree operator^(const Constant& constant2) const;

	Napis toNapis() const override;
};