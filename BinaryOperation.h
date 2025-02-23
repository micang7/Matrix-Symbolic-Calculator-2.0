#pragma once
#include "Operation.h"

class BinaryOperation : public Operation
{
public:
	BinaryOperation(const Napis& opr);

	TreeNode* clone() const override;
	TreeNode* nextFreeNode() override;

	bool simplify(AbstractSyntaxTree& ast) override;

	Napis toNapis() const override;

	bool lowerPrecedenceThan(const Napis& opr2) const override;
	bool isNegationSignificant() const override;
};