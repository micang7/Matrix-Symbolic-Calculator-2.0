#pragma once
#include "Operation.h"

class BinaryOperation : public Operation
{
public:
	BinaryOperation(const Napis& opr);

	TreeNode* clone() const override;
	TreeNode* nextFreeNode() override;

	bool simplify(AbstractSyntaxTree& ast) override;
	bool simplify_L(AbstractSyntaxTree& ast, UnaryOperation* opr, bool leftIsNegated = false) override;
	bool simplify_L(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated = false) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Constant* left, bool rightIsNegated = false) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Matrix* left, bool rightIsNegated = false) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Variable* left, bool rightIsNegated = false) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, BinaryOperation* left, bool rightIsNegated = false) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, UnaryOperation* left, bool rightIsNegated = false) override;

	Napis toNapis() const override;

	bool lowerPrecedenceThan(const Napis& opr2) const override;
	bool isNegationSignificant() const override;
};