#pragma once
#include "Operation.h"

class UnaryOperation : public Operation
{
public:
	UnaryOperation(const Napis& opr);

	TreeNode* clone() const override;
	TreeNode* shift() override;

	TreeNode* nextFreeNode() override;

	bool isNegation() const override;

	void reorganise(AbstractSyntaxTree& ast) override;

	void evaluate(AbstractSyntaxTree& ast) override;
	AbstractSyntaxTree compute(const Napis& opr) const override;
	AbstractSyntaxTree compute1(const Napis& opr, TreeNode* right) const override;
	AbstractSyntaxTree compute2(const Constant& left, const Napis& opr) const override;
	AbstractSyntaxTree compute2(const Matrix& left, const Napis& opr) const override;
	AbstractSyntaxTree compute2(const Variable& left, const Napis& opr) const override;
	AbstractSyntaxTree compute2(const BinaryOperation& left, const Napis& opr) const override;
	AbstractSyntaxTree compute2(const UnaryOperation& left, const Napis& opr) const override;

	Napis toNapis() const override;
};

