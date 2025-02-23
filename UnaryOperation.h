#pragma once
#include "Operation.h"

class UnaryOperation : public Operation
{
public:
	UnaryOperation(const Napis& opr);

	TreeNode* clone() const override;
	TreeNode* nextFreeNode() override;

	bool simplify(AbstractSyntaxTree& ast) override;



	Napis toNapis() const override;
};

