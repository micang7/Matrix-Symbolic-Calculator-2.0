#pragma once
#include "Operation.h"

class UnaryOperation : public Operation
{
public:
	UnaryOperation(const Napis& opr);

	TreeNode* clone() const override;
	TreeNode* shift() override;

	TreeNode* nextFreeNode() override;

	void simplify(AbstractSyntaxTree& ast) override;

	AbstractSyntaxTree evaluate(const Napis& opr) const override;

	AbstractSyntaxTree evaluate1(const Napis& opr, TreeNode* right) const override;
	AbstractSyntaxTree evaluate2(const Constant& left, const Napis& opr) const override;
	AbstractSyntaxTree evaluate2(const Matrix& left, const Napis& opr) const override;
	AbstractSyntaxTree evaluate2(const Variable& left, const Napis& opr) const override;
	AbstractSyntaxTree evaluate2(const BinaryOperation& left, const Napis& opr) const override;
	AbstractSyntaxTree evaluate2(const UnaryOperation& left, const Napis& opr) const override;

	bool isNegation() const override;

	Napis toNapis() const override;
};

