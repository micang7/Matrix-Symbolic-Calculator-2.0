#pragma once
#include "TreeNode.h"

class Constant : public TreeNode
{
	int m_val;
public:
	Constant(int val);

	TreeNode* clone() const override;

	bool evaluate(AbstractSyntaxTree& ast, UnaryOperation* opr) override;

	bool evaluate_part1(AbstractSyntaxTree& ast, BinaryOperation* opr) override;
	bool evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, Constant* left) override;
	bool evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, Matrix* left) override;
	bool evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, Variable* left) override;
	bool evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, BinaryOperation* left) override;
	bool evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, UnaryOperation* left) override;

	AbstractSyntaxTree operator+(const Constant& constant2) const;
	AbstractSyntaxTree operator-(const Constant& constant2) const;
	AbstractSyntaxTree operator*(const Constant& constant2) const;
	AbstractSyntaxTree operator/(const Constant& constant2) const;
	AbstractSyntaxTree operator^(const Constant& constant2) const;

	Napis toNapis() const override;
};