#pragma once
#include "Operation.h"

class BinaryOperation : public Operation
{
public:
	BinaryOperation(const Napis& opr);

	TreeNode* clone() const override;
	TreeNode* shift() override;

	TreeNode* nextFreeNode() override;

	void arrange(AbstractSyntaxTree& ast) override;

	void evaluate(AbstractSyntaxTree& ast) override;
	AbstractSyntaxTree compute1(const Napis& opr, TreeNode* right) const override;

	Napis toNapis() const override;
	Napis toNapisExpand() const override;

	bool lowerPrecedenceThan(const Napis& opr2) const override;
	bool equalPrecedenceAs(const Napis& opr1, bool replace = false) override;
	bool isNegationSignificant() const override;
};